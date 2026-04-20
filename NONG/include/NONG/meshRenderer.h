#ifndef NONG_MESH_RENDERER_H
#define NONG_MESH_RENDERER_H

#include "NONG/component.h"
#include "NONG/mesh.h"
#include "NONG/material.h"
#include "NONG/utility.h"

namespace NONG {
    class MeshRenderer : public Component {
        DECLARE_COMPONENT(MeshRenderer)
    public:
        Mesh* mesh;
        Material* material;
        Int2 size;

        MeshRenderer(Mesh* mesh, Material* material, Vector2 size) : mesh(mesh), material(material), size(size) { }
    };
}

#endif