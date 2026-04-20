#ifndef NONG_VERTEX_H
#define NONG_VERTEX_H

#include <cstdint>

namespace NONG {
    struct Vertex {
        float position[3];
        float uv[2];
    };

    const Vertex quadVertices[] = {
        // Position                 // UV
        {{-0.5f, -0.5f, 0.0f},      {0.0f, 0.0f}}, // Top Left
        {{ 0.5f, -0.5f, 0.0f},      {1.0f, 0.0f}}, // Top Right
        {{ 0.5f,  0.5f, 0.0f},      {1.0f, 1.0f}}, // Bottom Right
        {{-0.5f,  0.5f, 0.0f},      {0.0f, 1.0f}}  // Bottom Left
    };

    const uint16_t quadIndices[] = {
        0, 1, 2, 
        0, 2, 3  
    };
}

#endif