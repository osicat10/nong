#ifndef NONG_RENDERER_H
#define NONG_RENDERER_H

#include <SDL3/SDL.h>
#include <SDL3_shadercross/SDL_shadercross.h>
#include <vector>

namespace NONG {
    class Renderer {
    private:
        SDL_GPUDevice* device;
        SDL_GPUGraphicsPipeline* spritePipeline;
        SDL_GPUSampler* defaultSampler;

        // Helper to load shader files into SDL_GPU
        SDL_GPUShader* LoadShader(const char* filepath, SDL_ShaderCross_ShaderStage stage);

    public:
        Renderer(SDL_GPUDevice* gpuDevice);
        ~Renderer();

        // Called once to set up the shaders and pipeline
        void Initialize(); 

        // Called every frame
        //void DrawSprites(SDL_GPURenderPass* renderPass, const std::vector<Sprite*>& sprites);
    };
}

#endif