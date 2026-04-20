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

    Shader fragShader(BakedShaders::sprite_frag, SDL_GPU_SHADERSTAGE_FRAGMENT, 1, 0);
    Shader vertShader(BakedShaders::sprite_vert, SDL_GPU_SHADERSTAGE_VERTEX, 0, 2);
    GraphicsPipeline pipeline(vertShader, fragShader, VertexLayout::CreateSpriteLayout());

    Image image("image.jpg");
    Texture texture(image);


    Material material(pipeline);
    material.SetTexture(0, texture);

    Mesh* quadMesh = Mesh::CreateQuad();

    Object mainCameraObject("Main Camera");
    Transform* cameraTransform = mainCameraObject.AddComponent<Transform>(0, 0, 5);
    Camera* mainCamera = mainCameraObject.AddComponent<Camera>(cameraTransform);

    Object spriteObject("Sprite Object");
    spriteObject.AddComponent<Transform>();
    spriteObject.AddComponent<SpriteRenderer>(&material, quadMesh, 100, 100);
    Object spriteObject2("Sprite Object2");
    spriteObject2.AddComponent<Transform>(500,0,0);
    spriteObject2.AddComponent<SpriteRenderer>(&material, quadMesh, 100, 100);
    Object spriteObject3("Sprite Object3");
    spriteObject3.AddComponent<Transform>(Vector3(-100,0,0), Vector3(30,30,30), Vector3(.5,.5,.5));
    spriteObject3.AddComponent<SpriteRenderer>(&material, quadMesh, 100, 100);

    while(!w.shouldQuit)
    {
        w.HandleEvents();

        TimeController::UpdateTime();

        auto [windowWidth, windowHeight] = w.GetSize();
        mainCamera->UpdateMatrix(windowWidth, windowHeight);

        std::cout << "\r" << Time::deltaTime() << std::flush;

        if(Input::GetKey(SDL_SCANCODE_RIGHT)) (*cameraTransform).position.x += 100.0f * Time::deltaTime();
        if(Input::GetKey(SDL_SCANCODE_LEFT))  (*cameraTransform).position.x -= 100.0f * Time::deltaTime();
        if(Input::GetKey(SDL_SCANCODE_UP))  (*cameraTransform).position.y += 100.0f * Time::deltaTime();
        if(Input::GetKey(SDL_SCANCODE_DOWN))  (*cameraTransform).position.y -= 100.0f * Time::deltaTime();
        if(Input::GetKey(SDL_SCANCODE_LSHIFT))  (*cameraTransform).position.z += 100.0f * Time::deltaTime();
        if(Input::GetKey(SDL_SCANCODE_LCTRL))  (*cameraTransform).position.z -= 100.0f * Time::deltaTime();
        if(Input::GetKeyPressed(SDL_SCANCODE_E)) 
        {
            if(mainCamera->GetMode() == ProjectionMode::Orthographic)
            {
                mainCamera->SetMode(ProjectionMode::Perspective);
            }
            else
            {
                mainCamera->SetMode(ProjectionMode::Orthographic);
            }
        }

        
        Renderer::BeginScene(mainCamera);

        MonoBehaviourController::StartNewMonoBehaviours();
        MonoBehaviourController::RunMonoBehaviours();

        if(Input::GetKey(SDL_SCANCODE_ESCAPE)) w.shouldQuit = true;

        auto renderContext = w.BeginRenderPass();
        if (renderContext.renderPass != nullptr)
        {
            SDL_GPUViewport viewport = { 0.0f, 0.0f, (float)windowWidth, (float)windowHeight, 0.0f, 1.0f };
            SDL_SetGPUViewport(renderContext.renderPass, &viewport);
            
            SDL_Rect scissor = { 0, 0, windowWidth, windowHeight };
            SDL_SetGPUScissor(renderContext.renderPass, &scissor);

            Renderer::Flush(renderContext);
            
            w.EndRenderPass();  
        }              
    }

    delete quadMesh;


    return 0;
}