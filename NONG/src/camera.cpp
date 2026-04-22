#include "NONG/camera.h"
#include <cmath>
#include <cstring>
#include <algorithm>

namespace NONG {

    // =========================================================================
    // MATH BLACK BOX (Raw float[16] operations to avoid needing a Matrix class)
    // =========================================================================
    namespace {
        void Identity(float* m) {
            for (int i = 0; i < 16; i++) m[i] = (i % 5 == 0) ? 1.0f : 0.0f;
        }

        // Multiplies two 4x4 column-major matrices (out = a * b)
        void Multiply(const float* a, const float* b, float* out) {
            float temp[16];
            for (int c = 0; c < 4; ++c) {
                for (int r = 0; r < 4; ++r) {
                    temp[c * 4 + r] = 
                        a[0 * 4 + r] * b[c * 4 + 0] +
                        a[1 * 4 + r] * b[c * 4 + 1] +
                        a[2 * 4 + r] * b[c * 4 + 2] +
                        a[3 * 4 + r] * b[c * 4 + 3];
                }
            }
            std::memcpy(out, temp, sizeof(float) * 16);
        }

        void MakeOrthographic(float* m, float width, float height, float zoom, float zNear, float zFar) {
            Identity(m);
            float right = (width / 2.0f) / zoom;
            float top = (height / 2.0f) / zoom;
            
            m[0] = 1.0f / right;
            m[5] = 1.0f / top;
            m[10] = 1.0f / (zNear - zFar);
            m[14] = zNear / (zNear - zFar);
        }

        void MakePerspective(float* m, float fovDegrees, float aspect, float zNear, float zFar) {
            Identity(m);
            float fovRad = fovDegrees * (3.1415926535f / 180.0f);
            float tanHalfFovy = std::tan(fovRad / 2.0f);

            m[0] = 1.0f / (aspect * tanHalfFovy);
            m[5] = 1.0f / tanHalfFovy;
            m[10] = zFar / (zNear - zFar);
            m[11] = -1.0f;
            m[14] = -(zFar * zNear) / (zFar - zNear);
            m[15] = 0.0f;
        }

        void MakeScreenSpace(float* m, float width, float height) {
            Identity(m);
            m[0] = 2.0f / width;
            m[5] = -2.0f / height; 
            m[10] = 0.0f; 
            m[12] = -1.0f;
            m[13] = 1.0f;
            m[14] = 0.5f;
        }

        void MakeView(float* m, const Vector3& pos, const Vector3& rot) {
            Identity(m);
            
            m[12] = -pos.x;
            m[13] = -pos.y;
            m[14] = -pos.z;
        }
    }
    // =========================================================================

    Camera::Camera(Transform* t, ProjectionMode m) : transform(t), mode(m), orthoZoom(1.0f), fov(90.0f), nearPlane(0.1f), farPlane(1000.0f) 
    {
        Identity(viewProjection);
        allCameras.push_back(this);
    }

    Camera::~Camera()
    {
        allCameras.erase(std::find(allCameras.begin(), allCameras.end(), this));
    }

    void Camera::SetMode(ProjectionMode newMode) { mode = newMode; }
    ProjectionMode Camera::GetMode() { return mode; }
    void Camera::SetOrthoZoom(float zoom) { orthoZoom = zoom; }
    float Camera::GetOrthoZoom() { return orthoZoom; }
    void Camera::SetFOV(float degrees) { fov = degrees; }
    float Camera::GetFOV() { return fov; }
    
    const float* Camera::GetViewProjectionMatrix() const { return viewProjection; }

    void Camera::UpdateMatrix(float screenWidth, float screenHeight) {
        if (!transform) return;

        float view[16];
        float proj[16];

        // 1. Where is the camera?
        MakeView(view, transform->position, transform->rotation);

        // 2. What kind of lens is it using?
        if (mode == ProjectionMode::Orthographic) 
        {
            MakeOrthographic(proj, screenWidth, screenHeight, orthoZoom, nearPlane, farPlane);
        } 
        else if (mode == ProjectionMode::Perspective) 
        {
            float aspect = screenWidth / screenHeight;
            MakePerspective(proj, fov, aspect, nearPlane, farPlane);
        } 
        else if (mode == ProjectionMode::ScreenSpace)
        {
            MakeScreenSpace(proj, screenWidth, screenHeight);
            // UI doesn't need the View matrix (Camera position), it's glued to the screen!
            std::memcpy(viewProjection, proj, sizeof(float) * 16); 
            return; // Skip the Multiply(proj, view) step!
        }

        // 3. Combine them! (ViewProjection = Projection * View)
        Multiply(proj, view, viewProjection);
    }

    std::vector<Camera*> Camera::allCameras;

    std::vector<Camera*> Camera::GetAllCameras()
    {
        return allCameras;
    }
}