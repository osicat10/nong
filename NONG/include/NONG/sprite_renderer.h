#ifndef NONG_SPRITE_RENDERER_H
#define NONG_SPRITE_RENDERER_H

#include "NONG/component.h"
#include "NONG/material.h"
#include "NONG/mesh.h"
#include "NONG/transform.h"

namespace NONG {
    class SpriteRenderer : public MonoBehaviour {
        DECLARE_COMPONENT(SpriteRenderer);
    private:
        Material* material;
        Mesh* mesh;
        Transform* transform;
        
        // Dimensions of the original image/sprite in pixels
        float baseWidth;
        float baseHeight; 
        
        // The final 64-byte array sent to the GPU
        float modelMatrix[16]; 

    public:
        SpriteRenderer(Material* mat, Mesh* m, float width, float height, bool enabled = true);

        void Start() override;

        void Update() override;
    };
}

#endif