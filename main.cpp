#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <NONG.h>

#include <iostream>
#include <cmath>

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

#ifdef NDEBUG
const bool debug = false;
#else
const bool debug = true;
#endif       

int main(int argc, char* argv[]) 
{
    Window w("NONG Demo", {1280, 720}, SDL_WINDOW_RESIZABLE);

    {
        try
        {
            w.Initialize(debug);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return 1;
        }

        std::cout << "Using: " << w.GetGPUDriverName() << "\n";

        Shader circleFrag(BakedShaders::circle_frag, SDL_GPU_SHADERSTAGE_FRAGMENT, 1, 1);
        Shader fragShader(BakedShaders::sprite_frag, SDL_GPU_SHADERSTAGE_FRAGMENT, 1, 1);
        Shader vertShader(BakedShaders::sprite_vert, SDL_GPU_SHADERSTAGE_VERTEX, 0, 3);
        GraphicsPipeline pipeline(vertShader, fragShader, VertexLayout::CreateSpriteLayout());
        GraphicsPipeline circlePipeline(vertShader, circleFrag, VertexLayout::CreateSpriteLayout());
        GraphicsPipeline uiPipeline(vertShader, fragShader, VertexLayout::CreateSpriteLayout(), false, true);

        Image image("image.jpg");
        Texture2D texture(image);
        Material material(pipeline);
        material.SetTexture(0, texture);
        material.SetFragmentUniforms(Color::white());

        Material experimentMaterial(circlePipeline);
        experimentMaterial.SetTexture(0, texture);
        //experimentMaterial.SetFragmentUniforms(Color::red());

        RenderTexture renderTexture1(500, 500);
        RenderTexture renderTexture2(500, 500);
        RenderTexture* currentReadTexture = &renderTexture1;
        RenderTexture* currentWriteTexture = &renderTexture1;
        Material renderMaterial(pipeline);

        Mesh* quadMesh = Mesh::CreateQuad();

        Object cameraObject1("Camera1");
        Transform* cameraTransform = cameraObject1.AddComponent<Transform>(0,0,5);
        Camera* camera1 = cameraObject1.AddComponent<Camera>(cameraTransform);
        camera1->clearColor = Color(.1, .1, .1, 1);

        Object cameraObject2("Camera2");
        Camera* camera2 = cameraObject2.AddComponent<Camera>(cameraObject2.AddComponent<Transform>(0,0,5));    
        camera2->clearColor = Color(.5, 0, 0, 1); 

        Camera* camera3 = cameraObject2.AddComponent<Camera>(cameraObject2.AddComponent<Transform>(0,0,5));
        camera3->clearColor = Color(.5, 0, 0, 1);
        camera3->renderOrder = -1;

        camera1->viewportW = camera2->viewportW = camera2->viewportX = .5;

        Object uiCamObj("UICamera");
        Camera* uiCam = uiCamObj.AddComponent<Camera>(uiCamObj.AddComponent<Transform>());
        uiCam->SetMode(ProjectionMode::ScreenSpace);
        uiCam->cullingMask = LAYER_UI;
        uiCam->renderOrder = 20;
        uiCam->clearColor.a = 0.0f;
        
        Object spriteObject("Sprite Object");
        spriteObject.AddComponent<Transform>();
        spriteObject.AddComponent<SpriteRenderer>(&experimentMaterial, quadMesh, 100, 100);
        Object spriteObject2("Sprite Object2");
        spriteObject2.AddComponent<Transform>(500,0,0);
        spriteObject2.AddComponent<SpriteRenderer>(&material, quadMesh, 100, 100);
        Object spriteObject3("Sprite Object3");
        spriteObject3.AddComponent<Transform>(Vector3(-100,0,0), Vector3(30,30,30));
        spriteObject3.AddComponent<SpriteRenderer>(&renderMaterial, quadMesh, 300, 300);

        Font font((unsigned char*)EmbeddedFonts::openSansBytes, sizeof(EmbeddedFonts::openSansLength), 32.0f);
        Material fontMaterial(uiPipeline);
        fontMaterial.SetTexture(0, *font.GetAtlas());
        Object textDisplay("FPS Counter");
        textDisplay.AddComponent<Transform>(100,100,0);
        TextRenderer* tr = textDisplay.AddComponent<TextRenderer>(&font, &fontMaterial, quadMesh);
        tr->text = "Hello world!";

        Image imageBar("bar.png");
        Texture2D textureBar(imageBar);
        Material uiMaterial(uiPipeline);
        uiMaterial.SetTexture(0, textureBar);

        Object healthBar("HealthBar");
        healthBar.AddComponent<Transform>(50, 50, 0);
        healthBar.AddComponent<UIRenderer>(&uiMaterial, quadMesh, 200, 30);

        while(!w.shouldQuit)
        {
            w.HandleEvents();

            TimeController::UpdateTime();

            std::cout << "\r" << Time::deltaTime() << std::flush;

            currentWriteTexture = currentReadTexture;
            currentReadTexture = currentReadTexture == &renderTexture1? &renderTexture2 : &renderTexture1;
            renderMaterial.SetTexture(0, *currentReadTexture);
            camera3->renderTexture = currentWriteTexture;   

            if(Input::GetKey(SDL_SCANCODE_RIGHT)) (*cameraTransform).position.x += 100.0f * Time::deltaTime();
            if(Input::GetKey(SDL_SCANCODE_LEFT))  (*cameraTransform).position.x -= 100.0f * Time::deltaTime();
            if(Input::GetKey(SDL_SCANCODE_UP))  (*cameraTransform).position.y += 100.0f * Time::deltaTime();
            if(Input::GetKey(SDL_SCANCODE_DOWN))  (*cameraTransform).position.y -= 100.0f * Time::deltaTime();
            if(Input::GetKey(SDL_SCANCODE_LSHIFT))  (*cameraTransform).position.z += 100.0f * Time::deltaTime();
            if(Input::GetKey(SDL_SCANCODE_LCTRL))  (*cameraTransform).position.z -= 100.0f * Time::deltaTime();
            if(Input::GetKeyPressed(SDL_SCANCODE_E)) 
            {
                if(camera1->GetMode() == ProjectionMode::Orthographic)
                {
                    camera1->SetMode(ProjectionMode::Perspective);
                }
                else
                {
                    camera1->SetMode(ProjectionMode::Orthographic);
                }
            }

            tr->text = std::to_string((int)(1 / Time::deltaTime()));

            
            Renderer::BeginScene();

            MonoBehaviourController::StartNewMonoBehaviours();
            MonoBehaviourController::RunMonoBehaviours();

            if(Input::GetKey(SDL_SCANCODE_ESCAPE)) w.shouldQuit = true;

            auto frame = w.BeginFrame();

            Renderer::Flush(frame);
            
            w.EndFrame(frame);           
        }

        delete quadMesh;
    }


    return 0;
}