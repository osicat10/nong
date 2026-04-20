#include "NONG/mesh.h"
#include <stdexcept>
#include <cstring>

namespace NONG {
    
    SDL_GPUDevice* Mesh::device = nullptr;

    void Mesh::SetGPUDevice(SDL_GPUDevice* d) { device = d; }
    SDL_GPUDevice* Mesh::GetGPUDevice() { return device; }

    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices) 
        : vertexBuffer(nullptr), indexBuffer(nullptr) 
    {
        if (!device) throw std::runtime_error("Mesh GPU Device not set!");

        indexCount = static_cast<uint32_t>(indices.size());

        Uint32 vertexDataSize = static_cast<Uint32>(vertices.size() * sizeof(Vertex));
        Uint32 indexDataSize = static_cast<Uint32>(indices.size() * sizeof(uint16_t));

        // 1. Create the final destination buffers in GPU VRAM
        SDL_GPUBufferCreateInfo vertexBufferInfo = {};
        vertexBufferInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
        vertexBufferInfo.size = vertexDataSize;
        vertexBuffer = SDL_CreateGPUBuffer(device, &vertexBufferInfo);

        SDL_GPUBufferCreateInfo indexBufferInfo = {};
        indexBufferInfo.usage = SDL_GPU_BUFFERUSAGE_INDEX;
        indexBufferInfo.size = indexDataSize;
        indexBuffer = SDL_CreateGPUBuffer(device, &indexBufferInfo);

        if (!vertexBuffer || !indexBuffer) {
            throw std::runtime_error("Failed to create GPU buffers: " + std::string(SDL_GetError()));
        }

        // 2. Create a temporary Transfer Buffer (Bridges CPU and GPU memory)
        SDL_GPUTransferBufferCreateInfo transferBufferInfo = {};
        transferBufferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
        transferBufferInfo.size = vertexDataSize + indexDataSize;
        SDL_GPUTransferBuffer* transferBuffer = SDL_CreateGPUTransferBuffer(device, &transferBufferInfo);

        // 3. Map the transfer buffer and copy our vectors into it
        void* map = SDL_MapGPUTransferBuffer(device, transferBuffer, false);
        std::memcpy(map, vertices.data(), vertexDataSize);
        std::memcpy(static_cast<uint8_t*>(map) + vertexDataSize, indices.data(), indexDataSize);
        SDL_UnmapGPUTransferBuffer(device, transferBuffer);

        // 4. Command the GPU to copy from the Transfer Buffer into the actual VRAM buffers
        SDL_GPUCommandBuffer* cmdBuf = SDL_AcquireGPUCommandBuffer(device);
        SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmdBuf);

        SDL_GPUTransferBufferLocation vertSource = { transferBuffer, 0 };
        SDL_GPUBufferRegion vertDest = { vertexBuffer, 0, vertexDataSize };
        SDL_UploadToGPUBuffer(copyPass, &vertSource, &vertDest, false);

        SDL_GPUTransferBufferLocation idxSource = { transferBuffer, vertexDataSize };
        SDL_GPUBufferRegion idxDest = { indexBuffer, 0, indexDataSize };
        SDL_UploadToGPUBuffer(copyPass, &idxSource, &idxDest, false);

        SDL_EndGPUCopyPass(copyPass);
        SDL_SubmitGPUCommandBuffer(cmdBuf);

        // 5. Clean up the temporary transfer buffer
        SDL_ReleaseGPUTransferBuffer(device, transferBuffer);
    }

    Mesh::~Mesh() 
    {
        if (vertexBuffer) SDL_ReleaseGPUBuffer(device, vertexBuffer);
        if (indexBuffer) SDL_ReleaseGPUBuffer(device, indexBuffer);
    }

    Mesh* Mesh::CreateQuad() 
    {
        std::vector<Vertex> vertices = {
            // Position                 // UV
            { {-0.5f, -0.5f, 0.0f},     {0.0f, 1.0f} }, // Top Left
            { { 0.5f, -0.5f, 0.0f},     {1.0f, 1.0f} }, // Top Right
            { { 0.5f,  0.5f, 0.0f},     {1.0f, 0.0f} }, // Bottom Right
            { {-0.5f,  0.5f, 0.0f},     {0.0f, 0.0f} }  // Bottom Left
        };
        std::vector<uint16_t> indices = {
            0, 1, 2,  // First Triangle
            2, 3, 0   // Second Triangle
        };
        return new Mesh(vertices, indices);
    }
}