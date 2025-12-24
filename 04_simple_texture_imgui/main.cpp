#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "glfw-cxx/Window.hpp"
#include "glfw-cxx/Error.hpp"
#include "glfw-cxx/Context.hpp"

#include "imgui.h"
#include "imgui_internal.h"
#include "backend/imgui_impl_glfw.h"
#include "backend/imgui_impl_opengl3.h"

#include "gloo/Texture.h"
#include "path.h"

// ====================================================================================================================

// ====================================================================================================================
static void glfw_error_callback(int error, const char *description)
{
    std::cerr << "GLFW Error" << error << ":" << description << "\n";
}
// ====================================================================================================================

// ====================================================================================================================
int main(int argc, char **argv)
{
    glfw::Error::SetErrorCallback(glfw_error_callback);

    // Decide GL+GLSL versions
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 330";

    glfw::Window window(1920, 1200, "04 Simple Texture Imgui");
    window.Hint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    window.Hint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window.Hint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window.MakeContextCurrent();

    // Load GLAD so it configures OpenGL
    gladLoadGL();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking

    ImGui::StyleColorsDark(); // Setup Dear ImGui style

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // ==================================================================================================================
    static float zoomMultiplier = 1.0, zoomMinMultiplier = 1.0, zoomMaxMultiplier = 8.0;
    ImVec2 viewWindowSize, viewSize = ImVec2(640, 480);

    // Stores the width, height, and the number of color channels of the image
    int gridTexWidth, gridTexHeight, _gridTexNCh;
    unsigned char *gridTexBytes = stbi_load(SRC_ROOT "/04_simple_texture_imgui/grid.png", &gridTexWidth, &gridTexHeight, &_gridTexNCh, 0);
    gloo::Texture gridTex(gridTexBytes, gridTexWidth, gridTexHeight, gloo::Texture::Type::UnsignedByte, gloo::Texture::InternalFormat::RGBA, gloo::Texture::Format::RGBA, gloo::Texture::Slot::slot00, gloo::Texture::Target::Texture2D);
    // ==================================================================================================================

    // Main loop
    while (!window.ShouldClose())
    {

        // Take care of all GLFW events
        window.PollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ================================================================================================================
        if (ImGui::Begin("imgui_canvas demo"))
        {
            ImGui::Separator(); // ==========================================================================================

            // ---- zoom slider ---------------------------------------------------------------------------------------------
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Zoom                ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(200);
            ImGui::SliderScalar("##zoom_SliderScalar", ImGuiDataType_Float, &zoomMultiplier, &zoomMinMultiplier, &zoomMaxMultiplier, "%.2f X");
            // ---- zoom slider ---------------------------------------------------------------------------------------------

            {
                viewWindowSize.x = ImGui::GetContentRegionAvail().x;
                viewSize.x = viewWindowSize.x - 14;
                viewSize.y = (int)(viewSize.x * gridTex.aspectRatio);
                viewWindowSize.y = viewSize.y + 14;

                viewSize.x = zoomMultiplier * viewSize.x;
                viewSize.y = zoomMultiplier * viewSize.y;

                ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6, 6));
                ImGui::BeginChild("canvas_child_window", viewWindowSize, true, ImGuiWindowFlags_None | ImGuiWindowFlags_HorizontalScrollbar);
                ImGui::Image((ImTextureID)(GLuint)gridTex, viewSize);
                ImGui::EndChild();
                ImGui::PopStyleVar();
                ImGui::PopStyleVar();
            }
        }
        ImGui::End();
        // ================================================================================================================

        // Rendering
        ImGui::Render();

        glfw::FramebufferSize display_size = window.GetFramebufferSize();
        glViewport(0, 0, display_size.x, display_size.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap the back buffer with the front buffer
        window.SwapBuffers();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    stbi_image_free(gridTexBytes);

    ImGui::DestroyContext();

    return 0;
}
// ====================================================================================================================