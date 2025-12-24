#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

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

#include "path.h"
// ====================================================================================================================

// ====================================================================================================================
// screen vertices ----------------------------------------------------------------------------------------------------
#define SCREEN_VERTICES                                                                                       \
    {                                                                                                         \
        gloo::Vertex{glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0), glm::vec3(0), glm::vec2(0.0f, 0.0f)},        \
            gloo::Vertex{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0), glm::vec3(0), glm::vec2(1.0f, 0.0f)},     \
            gloo::Vertex{glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0), glm::vec3(0), glm::vec2(1.0f, 1.0f)},    \
            gloo::Vertex { glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0), glm::vec3(0), glm::vec2(0.0f, 1.0f) } \
    }
// screen indices -----------------------------------------------------------------------------------------------------
#define SCREEN_INDICES \
    {                  \
        0, 1, 2,       \
            2, 3, 0    \
    }
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

    glfw::Window window(1920, 1200, "09 Multiple Textures Imgui");
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
    int texWidth, texHeight, _gridTexNCh;
    unsigned char *gridTexBytes = stbi_load(SRC_ROOT "/09_multiple_textures_imgui/grid.png", &texWidth, &texHeight, &_gridTexNCh, 0);
    gloo::Texture gridTex(gridTexBytes, texWidth, texHeight, gloo::Texture::Type::UnsignedByte, gloo::Texture::InternalFormat::RGBA, gloo::Texture::Format::RGBA, gloo::Texture::Slot::slot00, gloo::Texture::Target::Texture2D);
    // ------------------------------------------------------------------------------------------------------------------
    uint8_t *overlayTexBytes = new uint8_t[texWidth * texHeight]();
    for (int y = 0; y < texHeight; y++)
        for (int x = 0; x < texWidth; x++)
            overlayTexBytes[x + y * texWidth] = (uint8_t)ImPow(2.0, 16) * (float)x / (float)texWidth;
    gloo::Texture overlayTex(overlayTexBytes, texWidth, texHeight, gloo::Texture::Type::UnsignedByte, gloo::Texture::InternalFormat::Red, gloo::Texture::Format::Red, gloo::Texture::Slot::slot01, gloo::Texture::Target::Texture2D);
    // ------------------------------------------------------------------------------------------------------------------
    float norm_x2, norm_y2, norm_r;
    float center_x = 0.5, center_y = 0.5;
    uint8_t *maskTexBytes = new uint8_t[texWidth * texHeight]();
    for (int y = 0; y < texHeight; y++)
    {
        for (int x = 0; x < texWidth; x++)
        {
            norm_x2 = ImPow((float)x / (float)texWidth - center_x, 2);
            norm_y2 = ImPow((float)y / (float)texWidth - center_y * (float)texHeight / (float)texWidth, 2);
            norm_r = ImSqrt(norm_x2 + norm_y2);
            maskTexBytes[x + y * texWidth] = (unsigned char)ImPow(2.0, 8) * ((norm_r > 0.25) ? 0 : 1);
        }
    }
    gloo::Texture maskTex(maskTexBytes, texWidth, texHeight, gloo::Texture::Type::UnsignedByte, gloo::Texture::InternalFormat::Red, gloo::Texture::Format::Red, gloo::Texture::Slot::slot02, gloo::Texture::Target::Texture2D);
    // ==================================================================================================================
    gloo::Program screenShaderProgram(gloo::Shader(gloo::getFileContents(SRC_ROOT "/09_multiple_textures_imgui/direct.vert"), gloo::Shader::Type::Vertex), gloo::Shader(gloo::getFileContents(SRC_ROOT "/09_multiple_textures_imgui/direct.frag"), gloo::Shader::Type::Fragment));
    gloo::Mesh screenMesh(std::vector<gloo::Vertex>(SCREEN_VERTICES), std::vector<GLuint>(SCREEN_INDICES));
    screenMesh.LinkPositionToLocation(0);
    screenMesh.LinkTextureUVToLocation(1);
    // ==================================================================================================================
    screenShaderProgram.Uniform("in_texture0", gridTex);
    screenShaderProgram.Uniform("in_texture1", overlayTex);
    screenShaderProgram.Uniform("in_texture2", maskTex);
    // ------------------------------------------------------------------------------------------------------------------
    static float zoomMultiplier = 1.0, zoomMinMultiplier = 1.0, zoomMaxMultiplier = 8.0;
    ImVec2 viewWindowSize, viewSize = ImVec2(texWidth, texHeight);
    // ------------------------------------------------------------------------------------------------------------------

    // ------------------------------------------------------------------------------------------------------------------
    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    gloo::Framebuffer framebuffer;
    framebuffer.Bind();
    gloo::Texture renderTex(texWidth, texHeight, gloo::Texture::Type::UnsignedByte, gloo::Texture::InternalFormat::RGBA, gloo::Texture::Format::RGBA, gloo::Texture::Slot::slot03, gloo::Texture::Target::Texture2D);
    gloo::Renderbuffer renderbuffer;
    renderbuffer.RenderToTexture(renderTex);
    gloo::Framebuffer::UNBIND();
    // ------------------------------------------------------------------------------------------------------------------

    // Main loop
    while (!window.ShouldClose())
    {

        // ================================================================================================================
        framebuffer.Bind();
        glViewport(0, 0, texWidth, texHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // ----------------------------------------------------------------------------------------------------------------
        center_x = center_x + 0.005;
        center_x = ImFmod(center_x, 1.0);
        for (int y = 0; y < texHeight; y++)
        {
            for (int x = 0; x < texWidth; x++)
            {
                norm_x2 = ImPow((float)x / (float)texWidth - center_x, 2);
                norm_y2 = ImPow((float)y / (float)texWidth - center_y * (float)texHeight / (float)texWidth, 2);
                norm_r = ImSqrt(norm_x2 + norm_y2);
                maskTexBytes[x + y * texWidth] = (uint8_t)ImPow(2.0, 8) * ((norm_r > 0.25) ? 0 : 1);
            }
        }
        maskTex.Update(maskTexBytes);
        screenShaderProgram.Activate();
        screenMesh.Draw();
        // ================================================================================================================
        gloo::Framebuffer::UNBIND();

        glfwPollEvents();

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
                // ImGui::Image((ImTextureID)(GLuint)gridTex, viewSize);
                // ImGui::Image((ImTextureID)(GLuint)overlayTex, viewSize);
                // ImGui::Image((ImTextureID)(GLuint)maskTex, viewSize);
                renderTex.Update();
                ImGui::Image((ImTextureID)(GLuint)renderTex, viewSize);
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
        // Take care of all GLFW events
        window.PollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();

    delete overlayTexBytes;

    delete maskTexBytes;
    
    stbi_image_free(gridTexBytes);

    return 0;
}
// ====================================================================================================================