#include "NONG/renderer.h"
#include <algorithm>

namespace NONG {

    std::vector<RenderCommand> Renderer::commandQueue;
    const Camera* Renderer::activeCamera = nullptr;

    void Renderer::BeginScene(Camera* camera) 
    {
        activeCamera = camera;
        commandQueue.clear();
    }

    void Renderer::Submit(Material* material, Mesh* mesh, const float* modelMatrix) 
    {
        commandQueue.push_back({material, mesh, modelMatrix});
    }

    void Renderer::Flush(const RenderContext& ctx) 
    {
        if (commandQueue.empty() || !activeCamera) return;

        SDL_PushGPUVertexUniformData(
            ctx.cmdBuf, 
            0, // Slot 0
            activeCamera->GetViewProjectionMatrix(), 
            16 * sizeof(float) // Exactly 64 bytes
        );

        // 1. THE SORT: Group by Pipeline, then by Material
        std::sort(commandQueue.begin(), commandQueue.end(), [](const RenderCommand& a, const RenderCommand& b) 
        {
            if (a.material->GetPipeline() != b.material->GetPipeline()) {
                return a.material->GetPipeline() < b.material->GetPipeline(); // Sort by Pipeline memory address
            }
            return a.material < b.material; // Then sort by Material memory address
        });

        // 2. STATE TRACKERS: Keep track of what is currently on the GPU
        GraphicsPipeline* currentPipeline = nullptr;
        Material* currentMaterial = nullptr;
        Mesh* currentMesh = nullptr;

        // 3. THE BATCH DRAW
        for (const auto& cmd : commandQueue) 
        {
            
            // A. Only bind the Pipeline if it changed!
            GraphicsPipeline* newPipeline = cmd.material->GetPipeline();
            if (newPipeline != currentPipeline) 
            {
                SDL_BindGPUGraphicsPipeline(ctx.renderPass, newPipeline->GetNative());
                currentPipeline = newPipeline;
            }

            // B. Only bind Textures & Uniforms if the Material changed!
            if (cmd.material != currentMaterial) 
            {
                cmd.material->Bind(ctx);
                currentMaterial = cmd.material;
            }

            // C. Only bind Vertex/Index buffers if the Mesh changed!
            if (cmd.mesh != currentMesh) 
            {
                SDL_GPUBufferBinding vertexBinding = { cmd.mesh->GetVertexBuffer(), 0 };
                SDL_BindGPUVertexBuffers(ctx.renderPass, 0, &vertexBinding, 1);
                
                SDL_GPUBufferBinding indexBinding = { cmd.mesh->GetIndexBuffer(), 0 };
                SDL_BindGPUIndexBuffer(ctx.renderPass, &indexBinding, SDL_GPU_INDEXELEMENTSIZE_16BIT);
                
                currentMesh = cmd.mesh;
            }
            SDL_PushGPUVertexUniformData(ctx.cmdBuf, 1, cmd.modelMatrix, 64);

            // D. Issue the actual draw call
            SDL_DrawGPUIndexedPrimitives(ctx.renderPass, cmd.mesh->GetIndexCount(), 1, 0, 0, 0);
        }
    }
}