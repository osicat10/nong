#include "NONG/shader.h"
#include <fstream>
#include <stdexcept>
#include <vector>

namespace NONG {

    SDL_GPUDevice* Shader::device = nullptr;

    void Shader::SetGPUDevice(SDL_GPUDevice* d) { device = d; }
    SDL_GPUDevice* Shader::GetGPUDevice() { return device; }

    std::vector<Uint8> ReadBinaryFile(const std::string& filepath) {
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);
        if (!file.is_open()) throw std::runtime_error("Failed to open shader: " + filepath);
        size_t fileSize = (size_t)file.tellg();
        std::vector<Uint8> buffer(fileSize);
        file.seekg(0);
        file.read((char*)buffer.data(), fileSize);
        return buffer;
    }

    SDL_GPUShader* Shader::CompileNative(const unsigned char* code, size_t size, const std::string& backend, SDL_GPUShaderStage stage, int numSamplers, int numUniformBuffers) 
    {
        SDL_GPUShaderCreateInfo shaderInfo = {};
        shaderInfo.code = code;
        shaderInfo.code_size = size;
        shaderInfo.entrypoint = (stage == SDL_GPU_SHADERSTAGE_VERTEX) ? "VSMain" : "PSMain";
        shaderInfo.stage = stage;
        shaderInfo.num_samplers = numSamplers;
        shaderInfo.num_uniform_buffers = numUniformBuffers;

        if (backend == "vulkan") {
            shaderInfo.format = SDL_GPU_SHADERFORMAT_SPIRV;
        } else if (backend == "direct3d12" || backend == "direct3d11") {
            shaderInfo.format = SDL_GPU_SHADERFORMAT_DXIL; 
        } else {
            throw std::runtime_error("Unsupported GPU Backend: " + backend);
        }

        SDL_GPUShader* shader = SDL_CreateGPUShader(device, &shaderInfo);
        if (!shader) throw std::runtime_error("Failed to create GPU shader: " + std::string(SDL_GetError()));
        return shader;
    }

    // =========================================================
    // OPTION A: Load from File Path (e.g., "compiled_shaders/sprite_vert")
    // =========================================================
    Shader::Shader(const std::string& basePath, SDL_GPUShaderStage stage, int numSamplers, int numUniformBuffers) : nativeShader(nullptr) 
    {
        if (!device) throw std::runtime_error("Shader GPU Device not set!");
        std::string backend = SDL_GetGPUDeviceDriver(device);
        std::string ext = (backend == "vulkan") ? ".spv" : ".dxil";

        std::vector<Uint8> buffer = ReadBinaryFile(basePath + ext);
        nativeShader = CompileNative(buffer.data(), buffer.size(), backend, stage, numSamplers, numUniformBuffers);
    }

    // =========================================================
    // OPTION B: Load from Embedded Memory Array
    // =========================================================
    Shader::Shader(const EmbeddedShaderData& data, SDL_GPUShaderStage stage, int numSamplers, int numUniformBuffers) : nativeShader(nullptr) 
    {
        if (!device) throw std::runtime_error("Shader GPU Device not set!");
        std::string backend = SDL_GetGPUDeviceDriver(device);

        // Cleanly picking the exact binary format needed from the isolated struct
        if (backend == "vulkan") {
            nativeShader = CompileNative(data.spv, data.spvSize, backend, stage, numSamplers, numUniformBuffers);
        } else {
            nativeShader = CompileNative(data.dxil, data.dxilSize, backend, stage, numSamplers, numUniformBuffers);
        }
    }

    Shader::~Shader() { 
        if (nativeShader) SDL_ReleaseGPUShader(device, nativeShader); 
    }
}