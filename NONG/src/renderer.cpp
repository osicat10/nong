#include "NONG/renderer.h"
#include <algorithm>

namespace NONG {

    std::vector<RenderCommand> Renderer::commandQueue;

    void Renderer::BeginScene() 
    {
        commandQueue.clear();
    }

    void Renderer::Submit(Material* material, Mesh* mesh, const float* modelMatrix, int zIndex, uint32_t layer) 
    {
        commandQueue.push_back({material, mesh, modelMatrix, zIndex, layer});
    }

    void Renderer::DrawCamera(const FrameData& frame, Camera* camera, SDL_GPURenderPass* renderPass, 
                              GraphicsPipeline*& currentPipeline, Material*& currentMaterial, Mesh*& currentMesh)
    {
        float targetWidth = (camera->renderTexture) ? camera->renderTexture->GetWidth() : frame.width;
        float targetHeight = (camera->renderTexture) ? camera->renderTexture->GetHeight() : frame.height;

        float viewPixelWidth = camera->viewportW * targetWidth;
        float viewPixelHeight = camera->viewportH * targetHeight;

        camera->UpdateMatrix(viewPixelWidth, viewPixelHeight);

        SDL_GPUViewport viewport = {};
        viewport.x = camera->viewportX * targetWidth;
        viewport.y = camera->viewportY * targetHeight;
        viewport.w = viewPixelWidth;  
        viewport.h = viewPixelHeight; 
        viewport.min_depth = 0.0f;
        viewport.max_depth = 1.0f;
        SDL_SetGPUViewport(renderPass, &viewport);  

        SDL_PushGPUVertexUniformData(frame.cmdBuf, 0, camera->GetViewProjectionMatrix(), 64);


        for (const auto& cmd : commandQueue) 
        {
            if ((camera->cullingMask & cmd.layer) == 0) continue;
            
            GraphicsPipeline* newPipeline = cmd.material->GetPipeline();
            if (newPipeline != currentPipeline) 
            {
                SDL_BindGPUGraphicsPipeline(renderPass, newPipeline->GetNative());
                currentPipeline = newPipeline;
            }

            if (cmd.material != currentMaterial) 
            {
                cmd.material->Bind(frame.cmdBuf, renderPass);
                currentMaterial = cmd.material;
            }

            if (cmd.mesh != currentMesh) 
            {
                SDL_GPUBufferBinding vertexBinding = { cmd.mesh->GetVertexBuffer(), 0 };
                SDL_BindGPUVertexBuffers(renderPass, 0, &vertexBinding, 1);
                
                SDL_GPUBufferBinding indexBinding = { cmd.mesh->GetIndexBuffer(), 0 };
                SDL_BindGPUIndexBuffer(renderPass, &indexBinding, SDL_GPU_INDEXELEMENTSIZE_16BIT);
                
                currentMesh = cmd.mesh;
            }

            SDL_PushGPUVertexUniformData(frame.cmdBuf, 1, cmd.modelMatrix, 64);
            SDL_DrawGPUIndexedPrimitives(renderPass, cmd.mesh->GetIndexCount(), 1, 0, 0, 0);
        }
    }

    void Renderer::Flush(const FrameData& frame) 
    {
        if (commandQueue.empty()) return;

        std::vector<Camera*> cameras = Camera::GetAllCameras();
        std::sort(cameras.begin(), cameras.end(), [](Camera* a, Camera* b) {
            return a->renderOrder < b->renderOrder; 
        });

        std::sort(commandQueue.begin(), commandQueue.end(), [](const RenderCommand& a, const RenderCommand& b) 
        {
            if (a.zIndex != b.zIndex) 
            {
                return a.zIndex < b.zIndex;
            }
            if (a.material->GetPipeline() != b.material->GetPipeline()) 
            {
                return a.material->GetPipeline() < b.material->GetPipeline();
            }
            return a.material < b.material;
        });

        SDL_GPURenderPass* activePass = nullptr;        
        RenderTexture* currentTarget = (RenderTexture*)0x1; 

        GraphicsPipeline* currentPipeline = nullptr;
        Material* currentMaterial = nullptr;
        Mesh* currentMesh = nullptr;

        for(Camera* camera : cameras)
        {
            if (camera->renderTexture != currentTarget) 
            {
                if (activePass) SDL_EndGPURenderPass(activePass);

                SDL_GPUColorTargetInfo colorTarget = {};
                colorTarget.clear_color = {camera->clearColor.r, camera->clearColor.g, camera->clearColor.b, camera->clearColor.a};
                colorTarget.load_op = SDL_GPU_LOADOP_CLEAR;
                colorTarget.store_op = SDL_GPU_STOREOP_STORE;
                colorTarget.texture = (camera->renderTexture) ? camera->renderTexture->GetGPUTexture() : frame.swapchainColor;

                SDL_GPUDepthStencilTargetInfo depthTarget = {};
                depthTarget.clear_depth = 1.0f;
                depthTarget.load_op = SDL_GPU_LOADOP_CLEAR;
                depthTarget.store_op = SDL_GPU_STOREOP_DONT_CARE;
                depthTarget.stencil_load_op = SDL_GPU_LOADOP_DONT_CARE; 
                depthTarget.stencil_store_op = SDL_GPU_STOREOP_DONT_CARE;
                depthTarget.cycle = true;
                depthTarget.texture = (camera->renderTexture) ? camera->renderTexture->GetDepthTexture() : frame.swapchainDepth;

                activePass = SDL_BeginGPURenderPass(frame.cmdBuf, &colorTarget, 1, &depthTarget);                
                currentTarget = camera->renderTexture;

                currentPipeline = nullptr;
                currentMaterial = nullptr;
                currentMesh = nullptr;
            }

            DrawCamera(frame, camera, activePass, currentPipeline, currentMaterial, currentMesh);
        }

        if (activePass) SDL_EndGPURenderPass(activePass);
    }
}