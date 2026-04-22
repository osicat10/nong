#include "NONG/text_renderer.h"
#include "NONG/renderer.h"
#include "NONG/object.h"

namespace NONG {
    TextRenderer::TextRenderer(Font* f, Material* mat, Mesh* m, const std::string& t)
        : font(f), material(mat), quadMesh(m), text(t) {}

    void TextRenderer::Start() {
        transform = GetObject()->GetComponent<Transform>();
    }

    void TextRenderer::Update() {
        if (!transform || text.empty()) return;

        float currentX = transform->position.x;
        float currentY = transform->position.y;

        for (char c : text) 
        {
            if (c < 32 || c > 126) continue; 

            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(font->GetCharData(), font->GetAtlasWidth(), font->GetAtlasHeight(), c - 32, &currentX, &currentY, &q, 1);

            float width = q.x1 - q.x0;
            float height = q.y1 - q.y0;
            float xCenter = q.x0 + (width / 2.0f);
            float yCenter = q.y0 + (height / 2.0f);

            float modelMatrix[16] = {
                width, 0.0f,   0.0f, 0.0f,
                0.0f,  height, 0.0f, 0.0f,
                0.0f,  0.0f,   1.0f, 0.0f,
                xCenter, yCenter, (float)transform->position.z, 1.0f 
            };

            float uvData[4] = { q.s0, q.t1, q.s1, q.t0 };

            Renderer::Submit(material, quadMesh, modelMatrix, 0, LAYER_UI, uvData); 
        }
    }
}