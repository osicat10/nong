#include "NONG/texture.h"
#include <stdexcept>

namespace NONG {

    SDL_GPUDevice* Texture::device = nullptr;
    void Texture::SetGPUDevice(SDL_GPUDevice* device) { Texture::device = device; }
    SDL_GPUDevice* Texture::GetGPUDevice() { return device; }

    SDL_GPUTexture* Texture::GetGPUTexture() const {return gpuTexture;}
    SDL_GPUSampler* Texture::GetNativeSampler() const {return nativeSampler;}

    Texture::Texture()
    {
        if(!device) throw std::runtime_error("GPU Device not set!");
    }

    Texture::~Texture()
    {
        if (device) 
        {
            if(gpuTexture) SDL_ReleaseGPUTexture(device, gpuTexture);
            if(nativeSampler) SDL_ReleaseGPUSampler(device, nativeSampler);
        }
    }

    int Texture::GetWidth() const { return width; }
    int Texture::GetHeight() const { return height; }
}