// SkelAnimGL.cpp : Defines the entry point for the application.
//
#include "app.h"

#include <FreeCamera.hpp>

using namespace std;

class AnimatedObject : public GameObject
{
public:
    AnimatedObject(const std::string& filename) :
        m_Model(ModelImporter::Load(filename))
    {
        m_Animator = std::make_shared<Animator>(m_Model->skeleton);
        m_Renderer = std::make_shared<MeshRenderer>(m_Model->meshes);
        // Getting first animation of current model
        m_Animator->Play(m_Model->animations[0]);
    }

    void SetTransform(const Transform& transform) 
    {
        GameObject::SetTransform(transform);
        m_Renderer->SetTransform(GetTranform());
    }

    void Update(float_t deltaTime) override
    {
        m_Animator->Update(deltaTime);
    }

    void Render() override
    {
        m_Renderer->Render();
    }
private:
    std::shared_ptr<MeshRenderer> m_Renderer;
    std::shared_ptr<Animator> m_Animator;
    const std::shared_ptr<Model> m_Model;
};


void InputGLFWCallback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
void WindowGLFWResizeCallback(GLFWwindow* window, int32_t width, int32_t height);

GLFWwindow* CreateWindowGLFW(const std::string& title, int32_t width, int32_t height);
void DeleteWindowGLFW(GLFWwindow* window);
void SetGLFWWindowTitle(GLFWwindow* window, const std::string& title);

void RenderLoop(GLFWwindow* window, const std::shared_ptr<Scene>& scene)
{
    float lastTime = glfwGetTime();
    float deltaTime = 0.0f;

    glClearColor(0.4f, 0.f, 0.f, 0);
    glEnable(GL_DEPTH_TEST);
    
    while (!glfwWindowShouldClose(window))
    {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene->Update(deltaTime); 
        scene->Render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        int fps = static_cast<int>(1.0f / deltaTime);
        std::string windowTitle = "FPS: " + std::to_string(fps);
        glfwSetWindowTitle(window, windowTitle.c_str());
    }
}

static std::shared_ptr<Camera> s_Camera;
static std::shared_ptr<Light> s_Light;
static std::shared_ptr<AnimatedObject> s_Dragon, s_Helicopter;

int main()
{
    s_Camera = std::make_shared<FreeCamera>(45.f, 0.1f, 1000.f, glm::vec3{ 0, 15, 0 }, glm::vec3{ 0, 0, -90 });
    s_Light = std::make_shared<Light>(glm::vec3{ -5, 20, -10 }, glm::vec3{ 0.8f, 1.0f, 0.8f });
    GLFWwindow* window = CreateWindowGLFW("Skeletal animation test", 640, 360);

    s_Dragon = std::make_shared<AnimatedObject>(DEMO_ASSETS_DIR + std::string("dragon\\scene.gltf"));
    s_Dragon->SetTransform(Transform({ 0 , 0, -10 }, { 5, 5, 5 }));

    s_Helicopter = std::make_shared<AnimatedObject>(DEMO_ASSETS_DIR + std::string("helicopter\\scene.gltf"));
    s_Helicopter->SetTransform(Transform({ 10 , 5, -10 }, { 5, 5, 5 }));

    std::shared_ptr<Scene> scene = std::make_shared<Scene>("Scene_1");
    scene->SetCamera(s_Camera);
    scene->AddObject(s_Dragon);
    scene->AddObject(s_Helicopter);
    scene->AddLight(s_Light);

    RenderLoop(window, scene);
    DeleteWindowGLFW(window);

    LOG_INFO("Press enter to exit.\n");
	return getchar();
}

void WindowGLFWResizeCallback(GLFWwindow* window, int32_t width, int32_t height)
{
    glViewport(0, 0, width, height);
    if(s_Camera)
        s_Camera->OnWindowResized(width, height);
}

void InputGLFWCallback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
{
    s_Camera->OnKeyPressed(key, scancode, action, mods);
    if (key == GLFW_KEY_1)
          s_Light->SetColor(glm::vec3(1., 1.f, 0.f));
    else if (key == GLFW_KEY_2)
         s_Light->SetColor(glm::vec3(1., .0f, 1.f));
}


GLFWwindow* CreateWindowGLFW(const std::string& title, int32_t width, int32_t height)
{
    if (!glfwInit()) LOG_ERROR("Failed to initialize GLFW");

    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), 0, 0);
    if (!window) LOG_ERROR("Failed to create GLFW window");


    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) LOG_ERROR("Failed to initialize GLAD");
    // turn off vsync
    glfwSwapInterval(0);

    glfwSetWindowSizeCallback(window, &WindowGLFWResizeCallback);
    glfwSetKeyCallback(window, &InputGLFWCallback);

    WindowGLFWResizeCallback(window, width, height);
    return window;
}
void DeleteWindowGLFW(GLFWwindow* window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}
void SetGLFWWindowTitle(GLFWwindow* window, const std::string& title)
{
    glfwSetWindowTitle(window, title.c_str());
}
