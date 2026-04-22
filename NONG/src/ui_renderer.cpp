#include "NONG/ui_renderer.h"
#include "NONG/renderer.h"

namespace NONG {
    UIRenderer::UIRenderer(Material* mat, Mesh* m, float w, float h) 
            : material(mat), mesh(m), width(w), height(h) {}

    void UIRenderer::Start() 
    {
        transform = GetObject()->GetComponent<Transform>();
    }

    void UIRenderer::Update() 
    {
        if (!transform) return;

        float xPos = transform->position.x + (width / 2.0f);
        float yPos = transform->position.y + (height / 2.0f);

        float modelMatrix[16] = {
            width, 0.0f,   0.0f, 0.0f,
            0.0f,  height, 0.0f, 0.0f,
            0.0f,  0.0f,   1.0f, 0.0f,
            xPos,  yPos,   (float)transform->position.z, 1.0f 
        };

        Renderer::Submit(material, mesh, modelMatrix, 0, LAYER_UI); 
    }
}