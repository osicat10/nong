#include "NONG/texture2d.h"
#include <cstring>
#include <stdexcept>

namespace NONG {

    Texture2D::Texture2D(const Image& image) 
    {
        this->width = image.getWidth();
        this->height = image.getHeight();
        const unsigned char* cpuPixels = image.getCpuPixels();

        // 1. Create the empty Texture on the GPU
        SDL_GPUTextureCreateInfo texInfo = {};
        texInfo.type = SDL_GPU_TEXTURETYPE_2D;
        texInfo.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
        texInfo.width = width;
        texInfo.height = height;
        texInfo.layer_count_or_depth = 1;
        texInfo.num_levels = 1;
        texInfo.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER; 

        gpuTexture = SDL_CreateGPUTexture(device, &texInfo);

        // 2. Create a Transfer Buffer
        SDL_GPUTransferBufferCreateInfo tbInfo = {};
        tbInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
        tbInfo.size = width * height * 4; 

        SDL_GPUTransferBuffer* transferBuffer = SDL_CreateGPUTransferBuffer(device, &tbInfo);

        // 3. Map the buffer, copy our CPU pixels into it, and Unmap it
        void* map = SDL_MapGPUTransferBuffer(device, transferBuffer, false);
        std::memcpy(map, cpuPixels, width * height * 4);
        SDL_UnmapGPUTransferBuffer(device, transferBuffer);

        // 4. Record a "Copy Pass"
        SDL_GPUCommandBuffer* cmdBuf = SDL_AcquireGPUCommandBuffer(device);
        SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmdBuf);

        // THE FIX: SRC is the TransferInfo (The Buffer)
        SDL_GPUTextureTransferInfo src = {};
        src.transfer_buffer = transferBuffer;
        
        // THE FIX: DEST is the Region (The Texture dimensions)
        SDL_GPUTextureRegion dest = {};
        dest.texture = gpuTexture;
        dest.w = width;
        dest.h = height;
        dest.d = 1;

        // Dispatch the copy command!
        SDL_UploadToGPUTexture(copyPass, &src, &dest, false);

        SDL_EndGPUCopyPass(copyPass);
        SDL_SubmitGPUCommandBuffer(cmdBuf);

        // 5. The truck has made its delivery, destroy it.
        SDL_ReleaseGPUTransferBuffer(device, transferBuffer);

        SDL_GPUSamplerCreateInfo samplerInfo = {};
        samplerInfo.min_filter = SDL_GPU_FILTER_NEAREST;
        samplerInfo.mag_filter = SDL_GPU_FILTER_NEAREST;
        samplerInfo.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;
        samplerInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
        samplerInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
        samplerInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;

        nativeSampler = SDL_CreateGPUSampler(device, &samplerInfo);
    }
}