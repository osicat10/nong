#ifndef NONG_TEXTURE2D_H
#define NONG_TEXTURE2D_H

#include "NONG/texture.h"
#include "NONG/image.h"

namespace NONG {
    class Texture2D : public Texture {
    public:
        Texture2D(const Image& image);
    };
}

#endif