#ifndef NONG_SHADER_H
#define NONG_SHADER_H

#include <SDL3/SDL.h>
#include <string>

namespace NONG {

    struct EmbeddedShaderData {
        const unsigned char* spv; size_t spvSize;
        const unsigned char* dxil; size_t dxilSize;
    };

    class Shader 
    {
    private:
        static SDL_GPUDevice* device;
        SDL_GPUShader* nativeShader;

        SDL_GPUShader* CompileNative(const unsigned char* code, size_t size, const std::string& backend, SDL_GPUShaderStage stage, int num_samplers, int num_uniform_buffers);
        
    public:
        // Option A: File Path (e.g., "compiled_shaders/sprite.vert")
        Shader(const std::string& basePath, SDL_GPUShaderStage stage, int num_samplers = 0, int num_uniform_buffers = 0);
        
        // Option B: Embedded Data (e.g., BakedShaders::sprite_vert)
        Shader(const EmbeddedShaderData& data, SDL_GPUShaderStage stage, int num_samplers = 0, int num_uniform_buffers = 0);
        
        ~Shader();

        SDL_GPUShader* GetNative() const { return nativeShader; }

        static void SetGPUDevice(SDL_GPUDevice* d);
        static SDL_GPUDevice* GetGPUDevice();
    };
}

#define DECLARE_EMBEDDED_SHADER(Name) namespace NONG::BakedShaders { extern const EmbeddedShaderData Name; }

DECLARE_EMBEDDED_SHADER(sprite_vert);
DECLARE_EMBEDDED_SHADER(sprite_frag);

#endif