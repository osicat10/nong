#ifndef NONG_RENDERER_H
#define NONG_RENDERER_H

#include "NONG/window.h"
#include "NONG/material.h"
#include "NONG/mesh.h"
#include "NONG/camera.h"
#include "NONG/types.h"
#include <vector>

namespace NONG {

    class Material;
    class Mesh;

    struct RenderCommand {
        Material* material;
        Mesh* mesh;
        const float* modelMatrix;
        int zIndex;
        uint32_t layer;
    };    

    class Renderer {
    private:
        static std::vector<RenderCommand> commandQueue;

        static void DrawCamera(const FrameData& frame, Camera* camera, SDL_GPURenderPass* renderPass, 
                              GraphicsPipeline*& currentPipeline, Material*& currentMaterial, Mesh*& currentMesh);

    public:
        static void BeginScene(); 

        static void Submit(Material* material, Mesh* mesh, const float* modelMatrix, int zIndex, uint32_t layer = LAYER_WORLD);

        static void Flush(const FrameData& frame); 
    };
}

#endif