#ifndef NONG_MESH_H
#define NONG_MESH_H

#include <SDL3/SDL.h>
#include "NONG/vertex.h"
#include <vector>

namespace NONG {
    class Mesh {
    private:
        static SDL_GPUDevice* device;
        SDL_GPUBuffer* vertexBuffer;
        SDL_GPUBuffer* indexBuffer;
        uint32_t indexCount;

    public:
        // Takes your CPU data and uploads it to GPU VRAM
        Mesh(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);
        ~Mesh();

        SDL_GPUBuffer* GetVertexBuffer() const { return vertexBuffer; }
        SDL_GPUBuffer* GetIndexBuffer() const { return indexBuffer; }
        uint32_t GetIndexCount() const { return indexCount; }

        static void SetGPUDevice(SDL_GPUDevice* d);
        static SDL_GPUDevice* GetGPUDevice();
        
        static Mesh* CreateQuad(); 
    };
}
#endif