#ifndef NONG_WINDOW_H
#define NONG_WINDOW_H

#include <SDL3/SDL.h>

#include "NONG/color.h"

#include <string>
#include <functional>

namespace NONG {
    using Int2 = std::pair<int, int>;

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

        [[nodiscard]] SDL_GPURenderPass* BeginRenderPass();
        void EndRenderPass(SDL_GPURenderPass* renderPass);

        void HandleEvents(std::function<void(SDL_Event)> customHandler = nullptr);

        void Initialize();
    };
}

#endif