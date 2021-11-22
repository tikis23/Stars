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
#include "Input.h"

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
    *ImGuiWindow::Variable_int("LOD") = 6;

    // create window
    Window window(1280, 900, "Stars", WINDOW_WINDOWED);
    window.MakeCurrent();
    window.SetVSync(0);
    Input::AddWindow(window.GetHandle());
    ImGuiWindow::Setup(&window);

    // init glew
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Could not init GLEW\n";
        throw std::invalid_argument("Could not init GLEW\n");
    }
    Renderer renderer(&window);

    //glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // FBO
    int width, height;
    window.GetSize(&width, &height);
    unsigned int fbo;
    unsigned int fbovbo, fbovao;
    unsigned int fboTexture;
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, *ImGuiWindow::Variable_int("LOD"));
    glGenerateMipmap(GL_TEXTURE_2D);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, width, height);
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
    float screenQuadData[] = {
        -1,  1, 0.0f, 1.0f,
        -1, -1, 0.0f, 0.0f,
         1,  1, 1.0f, 1.0f,
         1,  1, 1.0f, 1.0f,
        -1, -1, 0.0f, 0.0f,
         1, -1, 1.0f, 0.0f
    };
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), screenQuadData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glEnableVertexAttribArray(0);

    // main loop
    int frames = 0;
    float timer = 0;
    float avgFrameTime = 0;
    int avgFPS = 0;
    *ImGuiWindow::Variable_int("SelectedFileName") = -1;
    while (!glfwWindowShouldClose(window.GetHandle()))
    {
        float timeStart = glfwGetTime();
        window.GetSize(&width, &height);
        ImGuiWindow::StartFrame();
        glfwSwapInterval(*ImGuiWindow::Variable_bool("Vsync"));
        if (Window::updated)
        {
            window.GetSize(&width, &height);
            glDeleteFramebuffers(1, &fbo);
            glGenFramebuffers(1, &fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            glDeleteTextures(1, &fboTexture);
            glGenTextures(1, &fboTexture);
            glBindTexture(GL_TEXTURE_2D, fboTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, *ImGuiWindow::Variable_int("LOD"));
            glGenerateMipmap(GL_TEXTURE_2D);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);

            glDeleteRenderbuffers(1, &rbo);
            glGenRenderbuffers(1, &rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
            renderer.UpdateFBO();
            Window::updated = false;
        }
        // IMGUI
        ImGui::SetNextWindowPos({ -1, 0 });
        ImGui::SetNextWindowSize({ (float)width + 2, - 10 });
        if(ImGui::Begin("Main", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar |
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground))
            ImGui::End();

        renderer.Draw(&fboDrawShader, fbo, fbovao, fboTexture);

        // frame time
        frames++;
        if ((float)glfwGetTime() - timer >= 0.5f)
        {
            avgFPS = frames / ((float)glfwGetTime() - timer);
            avgFrameTime = (glfwGetTime() - timer) / frames;
            frames = 0;
            timer = (float)glfwGetTime();
        }
        if (ImGui::Begin("Main"))
        {
            ImGui::BeginMenuBar();
            ImGui::Text(("|Frame Time: " + std::to_string(avgFrameTime * 1000) + " ms| " + std::to_string(avgFPS) + " fps|").c_str());
            ImGui::EndMenuBar();
        }
        ImGuiWindow::EndFrame();
        window.Update();
        Global::time += (glfwGetTime() - timeStart) * Global::timeSpeed;
    }

    // exit program
    ImGuiWindow::Destroy();
    glfwSetErrorCallback(nullptr);
    glfwTerminate();
}
