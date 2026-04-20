#include "NONG/window.h"
#include "NONG/input.h"
#include "NONG/time.h"
#include "NONG/shader.h"
#include "NONG/texture.h"
#include "NONG/graphics_pipeline.h"
#include "NONG/mesh.h"

#include <stdexcept>

namespace NONG {
    Window::Window() : Window("Application", {1280, 720}, SDL_WINDOW_RESIZABLE) { }
    Window::Window(SDL_Window* window, SDL_GPUDevice* device) : window(window), device(device) { }
    Window::Window(const std::string& title, Int2 size, SDL_WindowFlags flags, std::string gpuDriverName) : title(title), size(size), flags(flags), gpuDriverName(gpuDriverName) { }
    Window::~Window()
    {
        if(window && device) SDL_ReleaseWindowFromGPUDevice(device, window);
        if(device) SDL_DestroyGPUDevice(device);
        if(window) SDL_DestroyWindow(window);
        if (depthTexture) SDL_ReleaseGPUTexture(device, depthTexture);
        SDL_Quit();
    }

    void Window::SetTitle(const std::string& title)
    {
        this->title = title;
        if(window)
        {
            SDL_SetWindowTitle(window, title.c_str());
        }
    }
    std::string Window::GetTitle() 
    { 
        if(window)
        {
            title = std::string(SDL_GetWindowTitle(window));            
        }
        return title;
    }

    void Window::SetSize(Int2 size)
    {
        this->size = size;
        if(window)
        {
            SDL_SetWindowSize(window, size.first, size.second);
        }
    }
    Int2 Window::GetSize()
    {
        if(window)
        {
            SDL_GetWindowSize(window, &size.first, &size.second);
        } 
        return size;
    }

    void Window::SetFlags(SDL_WindowFlags flags) { this->flags = flags; }
    SDL_WindowFlags Window::GetFlags() const { return flags; }

    void Window::SetGPUDriverName(std::string gpuDriverName)
    {
        this->gpuDriverName = gpuDriverName;
    }
    std::string Window::GetGPUDriverName()
    {
        if(window)
        {
            gpuDriverName = SDL_GetGPUDeviceDriver(device);
        } 
        return gpuDriverName;
    }

    void Window::Initialize(bool debug)
    {
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            throw std::runtime_error("Couldn't initialize SDL: " + std::string(SDL_GetError()));
        }

        window = SDL_CreateWindow(title.c_str(), size.first, size.second, flags);
        if (!window) {
            throw std::runtime_error("Couldn't create window: " + std::string(SDL_GetError()));
        } 

        device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | 
            SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL, 
            debug, (gpuDriverName.empty()?NULL:gpuDriverName.c_str()));
        if (!device) {
            throw std::runtime_error("Couldn't create GPU device: " + std::string(SDL_GetError()));
        }

        if (!SDL_ClaimWindowForGPUDevice(device, window)) {
            throw std::runtime_error("Couldn't claim window: " + std::string(SDL_GetError()));
        }

        Texture::SetGPUDevice(device);
        Shader::SetGPUDevice(device);
        GraphicsPipeline::SetGPUDevice(device);
        GraphicsPipeline::SetGPUTextureFormat(SDL_GetGPUSwapchainTextureFormat(device, window));
        Mesh::SetGPUDevice(device);
    }

    void Window::SetClearColor(const Color& color) { clearColor = color; }
    Color Window::GetClearColor() { return clearColor; }

    SDL_GPUDevice* Window::GetGPUDevice() const { return device; }
    SDL_Window* Window::GetWindow() const { return window; }

    RenderContext Window::BeginRenderPass()
    {
        cmdBuf = SDL_AcquireGPUCommandBuffer(device);
        
        SDL_GPUTexture* swapchainTexture;
        Uint32 swapchainWidth, swapchainHeight;
        bool acquired = SDL_AcquireGPUSwapchainTexture(cmdBuf, window, &swapchainTexture, &swapchainWidth, &swapchainHeight);

        if (!acquired || swapchainTexture == nullptr) {
            SDL_SubmitGPUCommandBuffer(cmdBuf);
            return { nullptr, nullptr }; 
        }

        if (swapchainWidth != currentDepthWidth || swapchainHeight != currentDepthHeight) {
            RecreateDepthTexture(swapchainWidth, swapchainHeight);
        }

        // 1. The Color Target (What you see)
        SDL_GPUColorTargetInfo colorTargetInfo = {};
        colorTargetInfo.texture = swapchainTexture;
        colorTargetInfo.clear_color.r = clearColor.r;
        colorTargetInfo.clear_color.g = clearColor.g;
        colorTargetInfo.clear_color.b = clearColor.b;
        colorTargetInfo.clear_color.a = clearColor.a;
        colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
        colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

        SDL_GPUDepthStencilTargetInfo depthTargetInfo = {};
        depthTargetInfo.texture = depthTexture;
        depthTargetInfo.clear_depth = 1.0f; 
        depthTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
        depthTargetInfo.store_op = SDL_GPU_STOREOP_DONT_CARE;
        depthTargetInfo.stencil_load_op = SDL_GPU_LOADOP_DONT_CARE;
        depthTargetInfo.stencil_store_op = SDL_GPU_STOREOP_DONT_CARE;
        depthTargetInfo.cycle = true;
        
        renderPass = SDL_BeginGPURenderPass(cmdBuf, &colorTargetInfo, 1, &depthTargetInfo);

        return RenderContext{
            .cmdBuf = cmdBuf,
            .renderPass = renderPass
        };
    }
    void Window::EndRenderPass()
    {
        if(renderPass) SDL_EndGPURenderPass(renderPass);
        if(cmdBuf) SDL_SubmitGPUCommandBuffer(cmdBuf);

        renderPass = nullptr;
        cmdBuf = nullptr;
    }

    void Window::HandleEvents(std::function<void(SDL_Event)> customHandler)
    {
        static SDL_Event event;
        InputController::UpdateInput();

        while (SDL_PollEvent(&event)) {
            if(customHandler) customHandler(event);

            if (event.type == SDL_EVENT_QUIT) {
                shouldQuit = true;
            }
        }
    }

    void Window::RecreateDepthTexture(Uint32 width, Uint32 height) 
    {
        if (width == currentDepthWidth && height == currentDepthHeight) return;
        
        if (width == 0 || height == 0) return; 

        if (depthTexture) {
            SDL_ReleaseGPUTexture(device, depthTexture);
            depthTexture = nullptr;
        }

        SDL_GPUTextureCreateInfo createInfo = {};
        createInfo.type = SDL_GPU_TEXTURETYPE_2D;
        createInfo.format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT; 
        createInfo.usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET;
        createInfo.width = width;
        createInfo.height = height;
        createInfo.layer_count_or_depth = 1;
        createInfo.num_levels = 1;

        depthTexture = SDL_CreateGPUTexture(device, &createInfo);
        
        currentDepthWidth = width;
        currentDepthHeight = height;
    }
}