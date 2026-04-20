#ifndef NONG_RENDERER_H
#define NONG_RENDERER_H

#include "NONG/render_context.h"
#include "NONG/material.h"
#include "NONG/mesh.h"
#include "NONG/camera.h"
#include <vector>

namespace NONG {

    class Material;

    struct RenderCommand {
        Material* material;
        Mesh* mesh;
        const float* modelMatrix;
    };

    class Renderer {
    private:
        static std::vector<RenderCommand> commandQueue;
        static const Camera* activeCamera;

    public:
        static void BeginScene(Camera* camera); 

        static void Submit(Material* material, Mesh* mesh, const float* modelMatrix);

        static void Flush(const RenderContext& ctx); 
    };
}

#endif