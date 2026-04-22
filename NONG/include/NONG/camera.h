#ifndef NONG_CAMERA_H
#define NONG_CAMERA_H

#include "NONG/component.h"
#include "NONG/transform.h"
#include "NONG/color.h"
#include "NONG/render_texture.h"
#include "NONG/types.h"

#include <SDL3/SDL.h>

namespace NONG {

    enum class ProjectionMode {
        Orthographic, // 2D flat view
        Perspective,  // 3D depth view
        ScreenSpace   // (0, 0) -> top left corner
    };

    class Camera : public Component {
        DECLARE_COMPONENT(Camera);
    private:
        Transform* transform;

        float viewProjection[16];

        static std::vector<Camera*> allCameras;

    public:
        // Settings
        ProjectionMode mode;
        float orthoZoom; // How "zoomed in" the 2D view is
        float fov;       // Field of view for 3D (in degrees)
        float nearPlane;
        float farPlane;

        int renderOrder = 0;

        // Normalized viewport
        float viewportX = 0.0f;
        float viewportY = 0.0f;
        float viewportW = 1.0f;
        float viewportH = 1.0f;

        // Culling mask
        uint32_t cullingMask = LAYER_WORLD; 

        // Clear color
        Color clearColor = Color::white();

        // Optional render texture
        RenderTexture* renderTexture = nullptr;  


        Camera(Transform* transform, ProjectionMode mode = ProjectionMode::Orthographic);
        ~Camera();

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

        static std::vector<Camera*> GetAllCameras();
    };
}

#endif