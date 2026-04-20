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


DECLARE_EMBEDDED_SHADER(sprite_vert);
DECLARE_EMBEDDED_SHADER(sprite_frag);

int main(int argc, char* argv[]) 
{
    std::cout << "Main\n";

    Window w("NONG Demo", {1280, 720}, SDL_WINDOW_RESIZABLE);
    try
    {
        #ifdef NDEBUG
        const bool debug = false;
        #else
        const bool debug = true;
        #endif       
        w.Initialize(debug);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    std::cout << "Using: " << w.GetGPUDriverName() << "\n";
    w.SetClearColor(Color(0.1, 0.1, 0.1, .5f));

    Shader fragShader(BakedShaders::sprite_frag, SDL_GPU_SHADERSTAGE_FRAGMENT, 1);
    Shader vertShader(BakedShaders::sprite_vert, SDL_GPU_SHADERSTAGE_VERTEX);
    GraphicsPipeline pipeline(vertShader, fragShader, VertexLayout::CreateSpriteLayout());

    Image image("image.jpg");
    Texture texture(image);

    std::cout << image.getHeight() << " x " << image.getWidth() << "\n";

    Material material(pipeline);
    material.SetTexture(0, texture);

    Mesh* quadMesh = Mesh::CreateQuad();

    while(!w.shouldQuit)
    {
        w.HandleEvents();
        TimeController::UpdateTime();
        MonoBehaviourController::StartNewMonoBehaviours();
        MonoBehaviourController::RunMonoBehaviours();

        if(Input::GetKey(SDL_SCANCODE_ESCAPE)) w.shouldQuit = true;

        auto renderContext = w.BeginRenderPass();
        if (renderContext.renderPass != nullptr)
        {
            auto [windowWidth, windowHeight] = w.GetSize();
            SDL_GPUViewport viewport = { 0.0f, 0.0f, (float)windowWidth, (float)windowHeight, 0.0f, 1.0f };
            SDL_SetGPUViewport(renderContext.renderPass, &viewport);
            
            SDL_Rect scissor = { 0, 0, windowWidth, windowHeight };
            SDL_SetGPUScissor(renderContext.renderPass, &scissor);
            
            material.Bind(renderContext);
            SDL_GPUBufferBinding vertexBinding = { quadMesh->GetVertexBuffer(), 0 };
            SDL_BindGPUVertexBuffers(renderContext.renderPass, 0, &vertexBinding, 1);
            SDL_GPUBufferBinding indexBinding = { quadMesh->GetIndexBuffer(), 0 };
            SDL_BindGPUIndexBuffer(renderContext.renderPass, &indexBinding, SDL_GPU_INDEXELEMENTSIZE_16BIT);

            SDL_DrawGPUIndexedPrimitives(renderContext.renderPass, quadMesh->GetIndexCount(), 1, 0, 0, 0);
            
            w.EndRenderPass();  
        }              
    }

    delete quadMesh;


    return 0;
}