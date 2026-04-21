#include "NONG/material.h"

namespace NONG {
    
    Material::Material(GraphicsPipeline& pipeline) : pipeline(&pipeline) {}
    Material::~Material() {}

    GraphicsPipeline* Material::GetPipeline() const { return pipeline; }

    void Material::SetTexture(Uint32 slot, Texture& texture) {
        textures[slot] = &texture;
    }

    void Material::Bind(SDL_GPUCommandBuffer* cmdBuf, SDL_GPURenderPass* renderPass) {
        if (!pipeline) return;

        // 1. Push Arbitrary Uniform Variables (Colors, Time, Settings)
        if (!fragmentUniformData.empty()) 
        {
            SDL_PushGPUFragmentUniformData(
                cmdBuf, 
                0, // Slot 0
                fragmentUniformData.data(), 
                fragmentUniformData.size()
            );
        }

        // 2. Bind Arbitrary Textures
        for (const auto& [slot, texture] : textures) {
            SDL_GPUTextureSamplerBinding binding = {};
            binding.texture = texture->GetGPUTexture();
            binding.sampler = texture->GetNativeSampler();
            
            SDL_BindGPUFragmentSamplers(renderPass, slot, &binding, 1);
        }
    }
}