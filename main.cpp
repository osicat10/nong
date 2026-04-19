#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <NONG.h>

#include <iostream>

using namespace NONG;

class TestComponent : public MonoBehaviour
{
    DECLARE_COMPONENT(TestComponent);
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
    Window w("NONG Demo", {1280, 720}, SDL_WINDOW_RESIZABLE);
    try
    {
        w.Initialize();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    std::cout << "Using: " << w.GetGPUDriverName() << "\n";
    w.SetClearColor(Color(0.1, 0.1, 0.1, .5f));

    while(!w.shouldQuit)
    {
        w.HandleEvents();
        TimeController::UpdateTime();
        MonoBehaviourController::StartNewMonoBehaviours();
        MonoBehaviourController::RunMonoBehaviours();

        // Render
        if(Input::GetKey(SDL_SCANCODE_ESCAPE)) w.shouldQuit = true;

        auto renderPass = w.BeginRenderPass();
        
        w.EndRenderPass(renderPass);        
    }


    return 0;
}