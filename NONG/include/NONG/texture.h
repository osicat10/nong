#ifndef NONG_TEXTURE_H
#define NONG_TEXTURE_H

#include <SDL3/SDL.h>

namespace NONG {
    class Texture {
    protected:
        SDL_GPUTexture* gpuTexture = nullptr;
        SDL_GPUSampler* nativeSampler = nullptr;
        static SDL_GPUDevice* device;
        
        int width, height;

    public:
        Texture();
        virtual ~Texture();

        static void SetGPUDevice(SDL_GPUDevice* dev);
        static SDL_GPUDevice* GetGPUDevice();

        SDL_GPUTexture* GetGPUTexture() const;
        SDL_GPUSampler* GetNativeSampler() const;

        int GetWidth() const;
        int GetHeight() const;
    };
}

#endif