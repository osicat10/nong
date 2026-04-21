#include "NONG/render_texture.h"

namespace NONG {

    RenderTexture::RenderTexture(int width, int height)
    {
        this->width = width;
        this->height = height;
        
        // 1. Define the Off-Screen Color Texture
        SDL_GPUTextureCreateInfo colorInfo = {};
        colorInfo.type = SDL_GPU_TEXTURETYPE_2D;
        colorInfo.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
        colorInfo.usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER; 
        
        colorInfo.width = width;
        colorInfo.height = height;
        colorInfo.layer_count_or_depth = 1;
        colorInfo.num_levels = 1;

        this->gpuTexture = SDL_CreateGPUTexture(device, &colorInfo);


        // 2. Define the Off-Screen Depth Texture
        SDL_GPUTextureCreateInfo depthInfo = {};
        depthInfo.type = SDL_GPU_TEXTURETYPE_2D;
        depthInfo.format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT;
        depthInfo.usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET;
        depthInfo.width = width;
        depthInfo.height = height;
        depthInfo.layer_count_or_depth = 1;
        depthInfo.num_levels = 1;

        this->depth = SDL_CreateGPUTexture(device, &depthInfo);

        // 3. Create sampler
        SDL_GPUSamplerCreateInfo samplerInfo = {};
        samplerInfo.min_filter = SDL_GPU_FILTER_LINEAR;
        samplerInfo.mag_filter = SDL_GPU_FILTER_LINEAR;
        samplerInfo.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;
        samplerInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
        samplerInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
        samplerInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;

        this->nativeSampler = SDL_CreateGPUSampler(device, &samplerInfo);
    }

    RenderTexture::~RenderTexture()
    {
        if(depth) SDL_ReleaseGPUTexture(device, depth);
        if(gpuTexture) SDL_ReleaseGPUTexture(device, gpuTexture);
    }

    SDL_GPUTexture* RenderTexture::GetDepthTexture() const {return depth;}
}