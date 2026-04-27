#include "NONG/shader.h"

namespace NONG::BakedShaders {

    const unsigned char circle_frag_spv[] = { 
        #embed "C:/Projects/nong/NONG/src/compiled_shaders/circle.frag.spv" 
    };
    const unsigned char circle_frag_dxil[] = { 
        #embed "C:/Projects/nong/NONG/src/compiled_shaders/circle.frag.dxil" 
    };

    extern const EmbeddedShaderData circle_frag = {
        circle_frag_spv, sizeof(circle_frag_spv),
        circle_frag_dxil, sizeof(circle_frag_dxil)
    };

    const unsigned char sprite_frag_spv[] = { 
        #embed "C:/Projects/nong/NONG/src/compiled_shaders/sprite.frag.spv" 
    };
    const unsigned char sprite_frag_dxil[] = { 
        #embed "C:/Projects/nong/NONG/src/compiled_shaders/sprite.frag.dxil" 
    };

    extern const EmbeddedShaderData sprite_frag = {
        sprite_frag_spv, sizeof(sprite_frag_spv),
        sprite_frag_dxil, sizeof(sprite_frag_dxil)
    };

    const unsigned char sprite_vert_spv[] = { 
        #embed "C:/Projects/nong/NONG/src/compiled_shaders/sprite.vert.spv" 
    };
    const unsigned char sprite_vert_dxil[] = { 
        #embed "C:/Projects/nong/NONG/src/compiled_shaders/sprite.vert.dxil" 
    };

    extern const EmbeddedShaderData sprite_vert = {
        sprite_vert_spv, sizeof(sprite_vert_spv),
        sprite_vert_dxil, sizeof(sprite_vert_dxil)
    };
}
