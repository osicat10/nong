#ifndef NONG_TEXTRENDERER_H
#define NONG_TEXTRENDERER_H

#include "NONG/component.h"
#include "NONG/font.h"
#include "NONG/material.h"
#include "NONG/mesh.h"
#include "NONG/transform.h"

namespace NONG {
    class TextRenderer : public MonoBehaviour {
        DECLARE_COMPONENT(TextRenderer);
    private:
        Font* font;
        Material* material;
        Mesh* quadMesh;
        Transform* transform;

    public:
        std::string text;

        TextRenderer(Font* f, Material* mat, Mesh* m, const std::string& t = "");
        void Start() override;
        void Update() override;
    };
}
#endif