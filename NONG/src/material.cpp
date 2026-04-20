#include "NONG/material.h"

namespace NONG {
    
    Material::Material(GraphicsPipeline& pipeline) : pipeline(&pipeline) {}
    Material::~Material() {}

    GraphicsPipeline* Material::GetPipeline() const { return pipeline; }

    void Material::SetTexture(Uint32 slot, Texture& texture) {
        textures[slot] = &texture;
    }

    void Material::Bind(const RenderContext& renderContext) {
        if (!pipeline) return;

        // 1. Push Arbitrary Uniform Variables (Colors, Time, Settings)
        if (!fragmentUniformData.empty()) {
            // SDL3 pushes this directly to space1 (Fragment Uniforms) automatically!
            SDL_PushGPUFragmentUniformData(
                renderContext.cmdBuf, 
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
            
            // Binds to the specific slot defined in your HLSL
            SDL_BindGPUFragmentSamplers(renderContext.renderPass, slot, &binding, 1);
        }
    }
}