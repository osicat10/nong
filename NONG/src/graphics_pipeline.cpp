#include "NONG/graphics_pipeline.h"
#include "NONG/vertex.h"
#include <stdexcept>

namespace NONG {

    SDL_GPUDevice* GraphicsPipeline::device = nullptr;
    SDL_GPUTextureFormat GraphicsPipeline::screenFormat = SDL_GPU_TEXTUREFORMAT_B8G8R8A8_UNORM;

    void GraphicsPipeline::SetGPUDevice(SDL_GPUDevice* d) { device = d; }
    void GraphicsPipeline::SetGPUTextureFormat(SDL_GPUTextureFormat format) { screenFormat = format; }

    VertexLayout VertexLayout::CreateSpriteLayout() {
        VertexLayout layout;
        layout.stride = sizeof(Vertex);

        SDL_GPUVertexAttribute posAttr = {};
        posAttr.location = 0;
        posAttr.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
        posAttr.offset = offsetof(Vertex, position); 

        SDL_GPUVertexAttribute uvAttr = {};
        uvAttr.location = 1;
        uvAttr.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
        uvAttr.offset = offsetof(Vertex, uv);

        layout.attributes.push_back(posAttr);
        layout.attributes.push_back(uvAttr);
        return layout;
    }

    GraphicsPipeline::GraphicsPipeline(const Shader& vertShader, const Shader& fragShader, const VertexLayout& layout, bool depthTest, bool alphaBlend) 
        : nativePipeline(nullptr) 
    {
        if (!device) throw std::runtime_error("GraphicsPipeline GPU Device not set!");

        SDL_GPUGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.vertex_shader = vertShader.GetNative();
        pipelineInfo.fragment_shader = fragShader.GetNative();
        pipelineInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;

        SDL_GPUVertexBufferDescription vertexBufferDesc = {};
        vertexBufferDesc.slot = 0;
        vertexBufferDesc.pitch = layout.stride;
        vertexBufferDesc.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;

        pipelineInfo.vertex_input_state.num_vertex_buffers = 1;
        pipelineInfo.vertex_input_state.vertex_buffer_descriptions = &vertexBufferDesc;
        pipelineInfo.vertex_input_state.num_vertex_attributes = layout.attributes.size();
        pipelineInfo.vertex_input_state.vertex_attributes = layout.attributes.data();

        SDL_GPUColorTargetDescription colorTargetDesc = {};
        colorTargetDesc.format = screenFormat;

        if (alphaBlend) 
        {
            colorTargetDesc.blend_state.enable_blend = true;
            colorTargetDesc.blend_state.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
            colorTargetDesc.blend_state.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
            colorTargetDesc.blend_state.color_blend_op = SDL_GPU_BLENDOP_ADD;
            
            colorTargetDesc.blend_state.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
            colorTargetDesc.blend_state.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
            colorTargetDesc.blend_state.alpha_blend_op = SDL_GPU_BLENDOP_ADD;
        }
        else 
        {
            colorTargetDesc.blend_state.enable_blend = false;
        }

        pipelineInfo.target_info.num_color_targets = 1;
        pipelineInfo.target_info.color_target_descriptions = &colorTargetDesc;

        pipelineInfo.target_info.has_depth_stencil_target = true;
        pipelineInfo.target_info.depth_stencil_format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT;

        pipelineInfo.depth_stencil_state.enable_depth_test = depthTest;
        pipelineInfo.depth_stencil_state.enable_depth_write = depthTest;
        
        pipelineInfo.depth_stencil_state.compare_op = SDL_GPU_COMPAREOP_LESS_OR_EQUAL;

        nativePipeline = SDL_CreateGPUGraphicsPipeline(device, &pipelineInfo);
        if (!nativePipeline) {
            throw std::runtime_error("Failed to create graphics pipeline: " + std::string(SDL_GetError()));
        }
    }

    GraphicsPipeline::~GraphicsPipeline() {
        if (nativePipeline) SDL_ReleaseGPUGraphicsPipeline(device, nativePipeline);
    }
}