#ifndef NONG_CAMERA_H
#define NONG_CAMERA_H

#include "NONG/component.h"
#include "NONG/transform.h"

namespace NONG {

    enum class ProjectionMode {
        Orthographic, // 2D flat view
        Perspective   // 3D depth view
    };

    class Camera : public Component {
        DECLARE_COMPONENT(Camera);
    private:
        ProjectionMode mode;
        Transform* transform;

        // The final 64-byte array we send to the GPU
        float viewProjection[16];

        // Settings
        float orthoZoom; // How "zoomed in" the 2D view is
        float fov;       // Field of view for 3D (in degrees)
        float nearPlane;
        float farPlane;

    public:
        // A Camera requires a Transform to know where it is!
        Camera(Transform* transform, ProjectionMode mode = ProjectionMode::Orthographic);

        void SetMode(ProjectionMode newMode);
        ProjectionMode GetMode();

        void SetOrthoZoom(float zoom);
        float GetOrthoZoom();

        void SetFOV(float degrees);
        float GetFOV();

        // Call this in your game loop whenever the window resizes or the camera moves
        void UpdateMatrix(float screenWidth, float screenHeight);

        // What the Renderer pulls to send to the Vertex Shader
        const float* GetViewProjectionMatrix() const;
    };
}

#endif