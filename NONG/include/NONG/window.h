#ifndef NONG_WINDOW_H
#define NONG_WINDOW_H

#include <SDL3/SDL.h>

#include "NONG/utility.h"
#include "NONG/color.h"
#include "NONG/render_context.h"

#include <string>
#include <functional>

namespace NONG {
    class Window
    {
    private:
        std::string title;
        Int2 size;
        SDL_WindowFlags flags;
        std::string gpuDriverName;

        Color clearColor;

        SDL_Window* window = nullptr;
        SDL_GPUDevice* device = nullptr;

        SDL_GPUCommandBuffer* cmdBuf = nullptr;      
        SDL_GPURenderPass* renderPass = nullptr;

        SDL_GPUTexture* depthTexture = nullptr;
        int currentDepthWidth = 0;
        int currentDepthHeight = 0;

        void RecreateDepthTexture(Uint32 width, Uint32 height);
    public:
        Window();
        Window(const std::string& title, Int2 size, SDL_WindowFlags flags = 0, std::string gpuDriverName = "");
        Window(SDL_Window* window, SDL_GPUDevice* device);

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        ~Window();

        bool shouldQuit = false;

        void SetTitle(const std::string& title);
        std::string GetTitle();

        void SetSize(Int2 size);
        Int2 GetSize();

        void SetFlags(SDL_WindowFlags flags);
        SDL_WindowFlags GetFlags() const;

        void SetGPUDriverName(std::string gpuDriverName);
        std::string GetGPUDriverName();

        void SetClearColor(const Color& c);
        Color GetClearColor();

        SDL_GPUDevice* GetGPUDevice() const;
        SDL_Window* GetWindow() const;

        [[nodiscard]] RenderContext BeginRenderPass();
        void EndRenderPass();

        void HandleEvents(std::function<void(SDL_Event)> customHandler = nullptr);

        void Initialize(bool debug = false);
    };
}

#endif