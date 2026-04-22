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

        modelMatrix[0] = width;  modelMatrix[4] = 0.0f;   modelMatrix[8]  = 0.0f; modelMatrix[12] = xPos;
        modelMatrix[1] = 0.0f;   modelMatrix[5] = height; modelMatrix[9]  = 0.0f; modelMatrix[13] = yPos;
        modelMatrix[2] = 0.0f;   modelMatrix[6] = 0.0f;   modelMatrix[10] = 1.0f; modelMatrix[14] = 0.5f; 
        modelMatrix[3] = 0.0f;   modelMatrix[7] = 0.0f;   modelMatrix[11] = 0.0f; modelMatrix[15] = 1.0f;

        Renderer::Submit(material, mesh, modelMatrix, 0, LAYER_UI); 
    }
}