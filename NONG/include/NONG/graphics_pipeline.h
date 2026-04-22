#ifndef NONG_GRAPHICS_PIPELINE_H
#define NONG_GRAPHICS_PIPELINE_H

#include <SDL3/SDL.h>
#include "NONG/shader.h"
#include <vector>

namespace NONG {

    struct VertexLayout {
        std::vector<SDL_GPUVertexAttribute> attributes;
        Uint32 stride;
        
        static VertexLayout CreateSpriteLayout(); 
    };

    class GraphicsPipeline {
    private:
        static SDL_GPUDevice* device;
        static SDL_GPUTextureFormat screenFormat;
        SDL_GPUGraphicsPipeline* nativePipeline;

    public:
        GraphicsPipeline(const Shader& vertShader, const Shader& fragShader, const VertexLayout& layout, bool depthTest = true, bool alphaBlend = false);
        ~GraphicsPipeline();

        SDL_GPUGraphicsPipeline* GetNative() const { return nativePipeline; }

        static void SetGPUDevice(SDL_GPUDevice* d);
        static void SetGPUTextureFormat(SDL_GPUTextureFormat format);
    };
}

#endif