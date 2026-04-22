#ifndef NONG_UIRENDERER_H
#define NONG_UIRENDERER_H

#include "NONG/object.h"
#include "NONG/component.h"
#include "NONG/material.h"
#include "NONG/mesh.h"
#include "NONG/transform.h"

namespace NONG {
    class UIRenderer : public MonoBehaviour {
        DECLARE_COMPONENT(UIRenderer);
    private:
        Material* material;
        Mesh* mesh;
        float width, height;
        Transform* transform;

        float modelMatrix[16];

    public:
        UIRenderer(Material* mat, Mesh* m, float w, float h);

        void Start() override; 

        void Update() override; 
    };
}

#endif