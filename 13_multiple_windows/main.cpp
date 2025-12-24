#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glm/gtc/constants.hpp>

#include "imgui.h"
#include "imgui_internal.h"
#include "backend/imgui_impl_glfw.h"
#include "backend/imgui_impl_opengl3.h"

#include "glfw-cxx/Window.hpp"
#include "glfw-cxx/Error.hpp"
#include "glfw-cxx/Context.hpp"

#include "gloo/Texture.h"
#include "gloo/Shader.h"
#include "gloo/Program.h"
#include "gloo/Framebuffer.h"
#include "gloo/Renderbuffer.h"
#include "gloo/Mesh.h"

#include "kato/function.hpp"

#include "ImguiWindow.h"

#include "path.h"
// ====================================================================================================================

// ====================================================================================================================
static void glfw_error_callback(int error, const char *description)
{
    std::cerr << "GLFW Error" << error << ":" << description << "\n";
}
uint ImGui::BaseWindow::COUNT = 0;
// ====================================================================================================================

// ====================================================================================================================
int main(int argc, char **argv)
{

    glfw::Error::SetErrorCallback(glfw_error_callback);

    // Decide GL+GLSL versions
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 330";
    // **** main window ***********************************************************************************************
    glfw::Window mainWindow(1920, 1200, "13 Multiple Windows | Main Window");
    mainWindow.Hint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    mainWindow.Hint(GLFW_CONTEXT_VERSION_MINOR, 3);
    mainWindow.Hint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // **** main window ***********************************************************************************************

    // **** sub window ************************************************************************************************
    glfw::Window subWindow(800, 800, "13 Multiple Windows | Sub Window", mainWindow);
    subWindow.Hint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    subWindow.Hint(GLFW_CONTEXT_VERSION_MINOR, 3);
    subWindow.Hint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // **** sub window ************************************************************************************************

    glfwMakeContextCurrent(mainWindow);
    // Load GLAD so it configures OpenGL
    gladLoadGL();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking

    ImGui::StyleColorsDark(); // Setup Dear ImGui style

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    static AppData appData;
    appData.ui.showBoxWindow = true;
    appData.ui.showCircleWindow = true;
    ImGui::BoxWindow boxWindow(appData.ui.showBoxWindow);
    ImGui::CircleWindow circleWindow(appData.ui.showCircleWindow);
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    glfwMakeContextCurrent(subWindow);
    gloo::Program screenShaderProgram(gloo::Shader(gloo::getFileContents(SRC_ROOT "/13_multiple_windows/shaders/direct.vert"), gloo::Shader::Type::Vertex), gloo::Shader(gloo::getFileContents(SRC_ROOT "/13_multiple_windows/shaders/direct.frag"), gloo::Shader::Type::Fragment));
    gloo::Mesh screenMesh(std::vector<gloo::Vertex>(SCREEN_VERTICES), std::vector<GLuint>(SCREEN_INDICES));
    screenMesh.LinkPositionToLocation(0);
    screenMesh.LinkTextureUVToLocation(1);
    // ----------------------------------------------------------------------------------------------------------------
    int gridTexWidth, gridTexHeight, _gridTexNCh;
    unsigned char *gridTexBytes = stbi_load(SRC_ROOT "/13_multiple_windows/grid.png", &gridTexWidth, &gridTexHeight, &_gridTexNCh, 0);
    gloo::Texture gridTex(gridTexBytes, gridTexWidth, gridTexHeight, gloo::Texture::Type::UnsignedByte, gloo::Texture::InternalFormat::RGBA, gloo::Texture::Format::RGBA, gloo::Texture::Slot::slot00, gloo::Texture::Target::Texture2D);
    screenShaderProgram.Uniform("in_texture", gridTex);
    gridTex.Bind();
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // Main loop
    while ((!mainWindow.ShouldClose()) && (!subWindow.ShouldClose()))
    {

        { // **** paint main window ***********************************************************************************
            mainWindow.MakeContextCurrent();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Take care of all GLFW events
            mainWindow.PollEvents();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            circleWindow.Show(appData);
            boxWindow.Show(appData);
            // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

            // Rendering
            ImGui::Render();
            glfw::FramebufferSize display_size = mainWindow.GetFramebufferSize();
            glViewport(0, 0, display_size.x, display_size.y);
            glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Swap the back buffer with the front buffer
            mainWindow.SwapBuffers();
        } // **** paint main window ***********************************************************************************

        { // **** paint sub window ************************************************************************************
            subWindow.MakeContextCurrent();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            screenShaderProgram.Activate();
            screenMesh.Draw();
            // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            // Swap the back buffer with the front buffer
            subWindow.SwapBuffers();
        } // **** paint sub window ************************************************************************************
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();

    return 0;
}
// ====================================================================================================================