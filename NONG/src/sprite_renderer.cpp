#include "NONG/sprite_renderer.h"
#include "NONG/renderer.h"
#include "NONG/object.h"

#include <cmath>

namespace NONG {

    // =========================================================================
    // MATH BLACK BOX (Calculates Translation and Scale)
    // =========================================================================
    namespace {
        void MakeModelMatrix(float* m, const Transform* t, float width, float height) {
            if (!t) return;

            // 1. Convert all rotations from degrees to radians
            float radX = t->rotation.x * (3.1415926535f / 180.0f);
            float radY = t->rotation.y * (3.1415926535f / 180.0f);
            float radZ = t->rotation.z * (3.1415926535f / 180.0f);

            // 2. Pre-calculate Sine and Cosine for all 3 axes
            float cx = std::cos(radX), sx = std::sin(radX);
            float cy = std::cos(radY), sy = std::sin(radY);
            float cz = std::cos(radZ), sz = std::sin(radZ);

            // 3. Final scale (Sprites have 0 base depth, so Z just uses raw scale)
            float finalWidth = width * t->scale.x;
            float finalHeight = height * t->scale.y;
            float finalDepth = t->scale.z; 

            // 4. Build the full 3D Rotation + Scale Matrix (Column-Major)
            
            // Column 0 (X Axis)
            m[0] = (cy * cz) * finalWidth;
            m[1] = (cy * sz) * finalWidth;
            m[2] = (-sy) * finalWidth;
            m[3] = 0.0f;

            // Column 1 (Y Axis)
            m[4] = (sy * sx * cz - sz * cx) * finalHeight;
            m[5] = (sy * sx * sz + cx * cz) * finalHeight;
            m[6] = (cy * sx) * finalHeight;
            m[7] = 0.0f;

            // Column 2 (Z Axis)
            m[8]  = (sy * cx * cz + sz * sx) * finalDepth;
            m[9]  = (sy * cx * sz - cz * sx) * finalDepth;
            m[10] = (cy * cx) * finalDepth;
            m[11] = 0.0f;

            // 5. Apply Translation (Position)
            m[12] = t->position.x;
            m[13] = t->position.y;
            m[14] = t->position.z;
            m[15] = 1.0f;
        }
    }
    // =========================================================================

    SpriteRenderer::SpriteRenderer(Material* mat, Mesh* m, float width, float height, bool enabled) : MonoBehaviour(enabled), material(mat), mesh(m), baseWidth(width), baseHeight(height) 
    {

    }

    void SpriteRenderer::Start()
    {
        transform = GetObject()->GetComponent<Transform>();
        MakeModelMatrix(modelMatrix, transform, baseWidth, baseHeight);
    }

    void SpriteRenderer::Update() {
        if (!material || !mesh || !transform) return;

        // 1. Recalculate the matrix using the latest Transform data
        MakeModelMatrix(modelMatrix, transform, baseWidth, baseHeight);

        // 2. Submit this object to the render queue for this frame!
        Renderer::Submit(material, mesh, modelMatrix);
    }
}