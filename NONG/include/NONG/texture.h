#ifndef NONG_TEXTURE_H
#define NONG_TEXTURE_H

#include <SDL3/SDL.h>

#include "NONG/image.h"

namespace NONG {
    class Texture
    {
    private:
        SDL_GPUTexture* gpuTexture;
        SDL_GPUSampler* nativeSampler;

        static SDL_GPUDevice* device;
    public:
        Texture(const Image& image);

        static void SetGPUDevice(SDL_GPUDevice* device);
        static SDL_GPUDevice* GetGPUDevice();

        SDL_GPUTexture* GetGPUTexture() const;
        SDL_GPUSampler* GetNativeSampler() const;

        ~Texture();
    };
}

#endif 