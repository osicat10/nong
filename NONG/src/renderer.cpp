#include "NONG/renderer.h"

namespace NONG {
    // You would pass the Camera here, or nullptr if you are making a UI app!
    void Renderer::DrawScene(SDL_GPURenderPass* renderPass, const std::vector<MeshRenderer*>& renderers, Camera* camera) {
        
        // 1. Calculate the Camera's View & Projection Matrices (or use defaults if camera == nullptr)
        // Matrix4x4 viewProj = (camera) ? camera->GetViewProjection() : Matrix4x4::Identity();

        // 2. Loop through every object that needs to be drawn
        for (MeshRenderer* renderer : renderers) {
            
            Material* mat = renderer->material;
            Mesh* mesh = renderer->mesh;

            // 3. Bind the Shader Pipeline
            SDL_BindGPUGraphicsPipeline(renderPass, mat->GetShader()->GetPipeline());

            // 4. Bind the Geometry (Vertex & Index Buffers)
            SDL_GPUBufferBinding vertexBinding = { mesh->GetVertexBuffer(), 0 };
            SDL_BindGPUVertexBuffers(renderPass, 0, &vertexBinding, 1);
            
            SDL_GPUBufferBinding indexBinding = { mesh->GetIndexBuffer(), 0 };
            SDL_BindGPUIndexBuffer(renderPass, &indexBinding, SDL_GPU_INDEXELEMENTSIZE_16BIT);

            // 5. Bind the Material Data (Textures)
            // Note: In a real engine, you'd map "tex_main" to slot 0, "tex_normal" to slot 1, etc.
            if (Texture* mainTex = mat->GetTexture("main")) {
                SDL_GPUTextureSamplerBinding samplerBinding = { mainTex->GetGPUTexture(), defaultSampler };
                SDL_BindGPUFragmentSamplers(renderPass, 0, &samplerBinding, 1);
            }

            // 6. Push the Matrix Math (MVP) to the Shader
            // We calculate Model Matrix based on the Object's Transform + MeshRenderer's size.
            // SDL_PushGPUVertexUniformData(renderPass, 0, &finalMVPMatrix, sizeof(Matrix4x4));

            // 7. DRAW!
            SDL_DrawGPUIndexedPrimitives(renderPass, mesh->GetIndexCount(), 1, 0, 0, 0);
        }
    }
}