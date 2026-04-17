#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <NONG/NONG.h>

#include <iostream>

int tmp()
{
    // 1. INITIALIZATION
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Engine Demo", 1280, 720, SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Log("Couldn't create window: %s", SDL_GetError());
        return -1;
    }

    SDL_GPUDevice* device = SDL_CreateGPUDevice(
        SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL, 
        true, NULL
    );
    if (!device) {
        SDL_Log("Couldn't create GPU device: %s", SDL_GetError());
        return -1;
    }

    if (!SDL_ClaimWindowForGPUDevice(device, window)) {
        SDL_Log("Couldn't claim window: %s", SDL_GetError());
        return -1;
    }

    // 2. THE MAIN ENGINE LOOP
    bool isRunning = true;
    SDL_Event event;

    // Optional: Set up timing variables here (e.g., uint64_t lastTime = SDL_GetTicksNS();)

    while (isRunning) {
        
        // A. EVENT POLLING
        // We use a while loop here to process ALL pending events this frame
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                isRunning = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
                isRunning = false;
            }
        }

        // B. ENGINE LOGIC & PHYSICS
        // (This is where you would update your game state, calculate deltatime, etc.)


        // C. RENDERING
        SDL_GPUCommandBuffer* cmdBuf = SDL_AcquireGPUCommandBuffer(device);
        SDL_GPUTexture* swapchainTexture;
        
        if (SDL_WaitAndAcquireGPUSwapchainTexture(cmdBuf, window, &swapchainTexture, NULL, NULL)) {
            if (swapchainTexture != NULL) {
                
                SDL_GPUColorTargetInfo colorTargetInfo = {0};
                colorTargetInfo.texture = swapchainTexture;
                colorTargetInfo.clear_color.r = 0.1f; // A sleek dark gray for our engine demo
                colorTargetInfo.clear_color.g = 0.1f;
                colorTargetInfo.clear_color.b = 0.12f;
                colorTargetInfo.clear_color.a = 1.0f;
                colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
                colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

                SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(cmdBuf, &colorTargetInfo, 1, NULL);
                // Draw calls will go here!
                SDL_EndGPURenderPass(renderPass);
            }
        }
        
        // Submit instructions to the GPU
        SDL_SubmitGPUCommandBuffer(cmdBuf);
    }

    // 3. CLEANUP
    SDL_ReleaseWindowFromGPUDevice(device, window);
    SDL_DestroyGPUDevice(device);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

using namespace NONG;

class TestComponent : public MonoBehaviour
{
private:
    std::string customString;

    Object* holder;

public:
    TestComponent(std::string customString, bool enabled) : MonoBehaviour(enabled), customString(customString) { }

    void Start() override
    {
        holder = GetObject();
        std::cout << "[" << holder->GetName() << "] Hello from the Start()!\n" 
            << "Custom string: " << customString << "\n\n";
    }

    void Update() override
    {
        std::cout << "[" << holder->GetName() << "] Hello from the Update()!\n" 
            << "Custom string: " << customString << "\n\n";
    }

    
};

int main(int argc, char* argv[]) 
{
    Object a("Object A");
    Object b("Object B");

    MonoBehaviour& mba = *a.AddComponent<TestComponent>("Enabled on start", true);
    MonoBehaviour& mbb = *b.AddComponent<TestComponent>("Disabled on start", false);

    std::cout << "Iteration 1:\n";
    MonoBehaviourController::StartNewMonoBehaviours();
    MonoBehaviourController::RunMonoBehaviours();
    mbb.SetEnabled(true);

    std::cout << "Iteration 2:\n";
    MonoBehaviourController::StartNewMonoBehaviours();
    MonoBehaviourController::RunMonoBehaviours();
    mba.SetEnabled(false);

    std::cout << "Iteration 3:\n";
    MonoBehaviourController::StartNewMonoBehaviours();
    MonoBehaviourController::RunMonoBehaviours();


    return 0;
}