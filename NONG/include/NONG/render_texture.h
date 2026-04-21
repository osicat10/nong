#ifndef NONG_RENDERTEXTURE_H
#define NONG_RENDERTEXTURE_H

#include "NONG/texture.h"

namespace NONG {
    class RenderTexture : public Texture {
    private:
        SDL_GPUTexture* depth = nullptr;

    public:
        RenderTexture(int width, int height);
        ~RenderTexture() override;
        
        SDL_GPUTexture* GetDepthTexture() const;
    };
}

#endif