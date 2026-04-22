#ifndef NONG_TYPES_H
#define NONG_TYPES_H

enum RenderLayer {
    LAYER_WORLD = 1 << 0,
    LAYER_UI    = 1 << 1,
    LAYER_ALL   = ~0
};

#endif