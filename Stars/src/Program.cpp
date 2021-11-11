#include "Program.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_stdlib.h"

#include <stdexcept>

#include "Window.h"
#include "Renderer.h"
#include "ImGuiWindow.h"

void error_callback(int error, const char* msg) {
    std::string s;
    s = " [" + std::to_string(error) + "] " + msg + '\n';
    std::cerr << s << std::endl;
}
Program::Program()
{
    loop();
}

void Program::loop()
{
    glfwSetErrorCallback(error_callback);
    // init glfw
    if (glfwInit() != GLFW_TRUE)
    {
        std::cout << "Could not init GLFW\n";
        throw std::invalid_argument("Could not init GLFW\n");
    }

    // create window
    //glfwWindowHint(GLFW_SAMPLES, 8);
    //glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_FALSE);
    //Window window(1920, 1080, "Stars", WINDOW_FULLSCREEN);
    Window window(1280, 900, "Stars", WINDOW_WINDOWED);
    window.MakeCurrent();
    window.SetVSync(0);
    ImGuiWindow::Setup(&window);

    // init glew
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Could not init GLEW\n";
        throw std::invalid_argument("Could not init GLEW\n");
    }
    Renderer renderer(&window);

    glEnable(GL_MULTISAMPLE);
    glfwSwapInterval(0);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // FBO
    int width, height;
    window.GetSize(&width, &height);
    static unsigned int fbo;
    static unsigned int fbovbo, fbovao;
    static unsigned int fboTexture;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glGenTextures(1, &fboTexture);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, Global::lod);
    glGenerateMipmap(GL_TEXTURE_2D);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Framebuffer Error!\n";
        return;
    }

    Shader fboDrawShader;
    fboDrawShader.LoadShaders("fboDraw", "res/shaders/fboDraw.glsl.vs", "res/shaders/fboDraw.glsl.fs");
    glGenBuffers(1, &fbovbo);
    glGenVertexArrays(1, &fbovao);
    glBindVertexArray(fbovao);
    glBindBuffer(GL_ARRAY_BUFFER, fbovbo);


    float minX = 0.0f;
    float maxX = 1.0f;             
    float minY = 0.0f;
    float maxY = 1.0f;
    float screenQuadData[] = {
        -1,  1, minX, maxY,
        -1, -1, minX, minY,
         1,  1, maxX, maxY,
         1,  1, maxX, maxY,
        -1, -1, minX, minY,
         1, -1, maxX, minY
    };
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), screenQuadData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glEnableVertexAttribArray(0);

    // main loop
    while (!glfwWindowShouldClose(window.GetHandle()))
    {
        float timeStart = glfwGetTime();
        window.GetSize(&width, &height);
        ImGuiWindow::StartFrame();

        // IMGUI
        ImGui::SetNextWindowPos({ -1, 0 });
        ImGui::SetNextWindowSize({ (float)width + 2, - 10 });
        if(ImGui::Begin("Main", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar |
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground))
        {

            ImGui::End();
        }

        renderer.Draw(&fboDrawShader, fbo, fbovao, fboTexture);
        ImGuiWindow::EndFrame();
        window.Update();
        Global::time += (glfwGetTime() - timeStart) * Global::timeSpeed;
    }

    // exit program
    ImGuiWindow::Destroy();
    glfwSetErrorCallback(nullptr);
    glfwTerminate();
}
