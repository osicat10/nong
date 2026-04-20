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
        SDL_ReleaseWindowFromGPUDevice(device, window);
        SDL_DestroyGPUDevice(device);
        SDL_DestroyWindow(window);
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

        if (SDL_WaitAndAcquireGPUSwapchainTexture(cmdBuf, window, &swapchainTexture, NULL, NULL)) {
            if (swapchainTexture != NULL) {
                
                SDL_GPUColorTargetInfo colorTargetInfo = {0};
                colorTargetInfo.texture = swapchainTexture;
                colorTargetInfo.clear_color.r = clearColor.r;
                colorTargetInfo.clear_color.g = clearColor.g;
                colorTargetInfo.clear_color.b = clearColor.b;
                colorTargetInfo.clear_color.a = clearColor.a;
                colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
                colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

                renderPass = SDL_BeginGPURenderPass(cmdBuf, &colorTargetInfo, 1, NULL);       
            }
        }
        
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
}