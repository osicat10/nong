#ifndef NONG_MATERIAL_H
#define NONG_MATERIAL_H

#include <SDL3/SDL.h>
#include "NONG/graphics_pipeline.h"
#include "NONG/window.h"
#include "NONG/texture.h"
#include "NONG/renderer.h"
#include <unordered_map>
#include <vector>

namespace NONG {
    class Material {
    private:
        GraphicsPipeline* pipeline;
        
        // Maps the HLSL register slot (0, 1, 2...) to the Texture pointer
        std::unordered_map<Uint32, Texture*> textures;

        // A raw byte array to hold arbitrary uniform data (colors, time, etc.)
        std::vector<Uint8> fragmentUniformData;

        void Bind(SDL_GPUCommandBuffer* cmdBuf, SDL_GPURenderPass* renderPass);

    public:
        Material(GraphicsPipeline& pipeline);
        ~Material();

        GraphicsPipeline* GetPipeline() const;

        void SetTexture(Uint32 slot, Texture& texture);
        
        template <typename T>
        void SetFragmentUniforms(const T& data) {
            const Uint8* rawData = reinterpret_cast<const Uint8*>(&data);
            fragmentUniformData.assign(rawData, rawData + sizeof(T));
        }
        
        friend class Renderer;
    };
}

#endif