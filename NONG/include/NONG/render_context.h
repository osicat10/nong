#ifndef NONG_RENDERCTX_H
#define NONG_RENDERCTX_H

#include <SDL3/SDL.h>

namespace NONG {
    struct RenderContext {
        SDL_GPUCommandBuffer* cmdBuf;
        SDL_GPURenderPass* renderPass;
    };
}

#endif