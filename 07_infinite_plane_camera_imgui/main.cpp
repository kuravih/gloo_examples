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
#include "gloo/Mesh.h"
#include "gloo/Framebuffer.h"
#include "gloo/Renderbuffer.h"

#include "ImguiCamera.h"

const unsigned int windowWidth = 1000;
const unsigned int windowHeight = 1000;

// ====================================================================================================================
// screen vertices ----------------------------------------------------------------------------------------------------
#define PLANE_VERTICES                                                                                        \
    {                                                                                                         \
        gloo::Vertex{glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0), glm::vec3(0), glm::vec2(0.0f, 0.0f)},        \
            gloo::Vertex{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0), glm::vec3(0), glm::vec2(1.0f, 0.0f)},     \
            gloo::Vertex{glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0), glm::vec3(0), glm::vec2(1.0f, 1.0f)},    \
            gloo::Vertex { glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0), glm::vec3(0), glm::vec2(0.0f, 1.0f) } \
    }
// screen indices -----------------------------------------------------------------------------------------------------
#define PLANE_INDICES \
    {                 \
        0, 1, 2,      \
            2, 3, 0   \
    }
// ====================================================================================================================
// pyramid vertices ---------------------------------------------------------------------------------------------------
//             position                          normal                          color                             texUV
#define PYRAMID_VERTICES                                                                                                                      \
    {                                                                                                                                         \
        gloo::Vertex{glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(0.0f, 0.0f)},      \
            gloo::Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(0.0f, 5.0f)}, \
            gloo::Vertex{glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(5.0f, 5.0f)},  \
            gloo::Vertex{glm::vec3(0.5f, 0.0f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(5.0f, 0.0f)},   \
            gloo::Vertex{glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec3(-0.8f, 0.5f, 0.0f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(0.0f, 0.0f)},  \
            gloo::Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(-0.8f, 0.5f, 0.0f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(5.0f, 0.0f)}, \
            gloo::Vertex{glm::vec3(0.0f, 0.8f, 0.0f), glm::vec3(-0.8f, 0.5f, 0.0f), glm::vec3(0.92f, 0.86f, 0.76f), glm::vec2(2.5f, 5.0f)},   \
            gloo::Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 0.5f, -0.8f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(5.0f, 0.0f)}, \
            gloo::Vertex{glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 0.5f, -0.8f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(0.0f, 0.0f)},  \
            gloo::Vertex{glm::vec3(0.0f, 0.8f, 0.0f), glm::vec3(0.0f, 0.5f, -0.8f), glm::vec3(0.92f, 0.86f, 0.76f), glm::vec2(2.5f, 5.0f)},   \
            gloo::Vertex{glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.8f, 0.5f, 0.0f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(0.0f, 0.0f)},   \
            gloo::Vertex{glm::vec3(0.5f, 0.0f, 0.5f), glm::vec3(0.8f, 0.5f, 0.0f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(5.0f, 0.0f)},    \
            gloo::Vertex{glm::vec3(0.0f, 0.8f, 0.0f), glm::vec3(0.8f, 0.5f, 0.0f), glm::vec3(0.92f, 0.86f, 0.76f), glm::vec2(2.5f, 5.0f)},    \
            gloo::Vertex{glm::vec3(0.5f, 0.0f, 0.5f), glm::vec3(0.0f, 0.5f, 0.8f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(5.0f, 0.0f)},    \
            gloo::Vertex{glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec3(0.0f, 0.5f, 0.8f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(0.0f, 0.0f)},   \
            gloo::Vertex { glm::vec3(0.0f, 0.8f, 0.0f), glm::vec3(0.0f, 0.5f, 0.8f), glm::vec3(0.92f, 0.86f, 0.76f), glm::vec2(2.5f, 5.0f) }  \
    }

// index order for pyramid vertices -----------------------------------------------------------------------------------
#define PYRAMID_INDICES \
    {                   \
        0, 1, 2,        \
            0, 2, 3,    \
            4, 6, 5,    \
            7, 9, 8,    \
            10, 12, 11, \
            13, 15, 14  \
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

   glfw::Window window(windowWidth, windowHeight, "07 Infinite Plane Camera Imgui");
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
    ImVec4 clear_darkblue = ImVec4(0.07f, 0.13f, 0.17f, 1.0f);

    static float zoomMultiplier = 1.0, zoomMinMultiplier = 1.0, zoomMaxMultiplier = 8.0;
    ImVec2 viewWindowSize, viewSize = ImVec2(640, 480);

    // ==================================================================================================================
    // plane shader -----------------------------------------------------------------------------------------------------
    gloo::Program planeShaderProgram(gloo::Shader(gloo::getFileContents("./plane.vert"), gloo::Shader::Type::Vertex), gloo::Shader(gloo::getFileContents("./plane.frag"), gloo::Shader::Type::Fragment));
    // plane mesh -------------------------------------------------------------------------------------------------------
    gloo::Mesh planeMesh(std::vector<gloo::Vertex>(PLANE_VERTICES), std::vector<GLuint>(PLANE_INDICES));
    planeMesh.LinkPositionToLocation(0);
    // pyramid shader ---------------------------------------------------------------------------------------------------
    gloo::Program pyramidShaderProgram(gloo::Shader(gloo::getFileContents("./pyramid.vert"), gloo::Shader::Type::Vertex), gloo::Shader(gloo::getFileContents("./pyramid.frag"), gloo::Shader::Type::Fragment));
    // pyramid mesh -----------------------------------------------------------------------------------------------------
    gloo::Mesh pyramidMesh(std::vector<gloo::Vertex>(PYRAMID_VERTICES), std::vector<GLuint>(PYRAMID_INDICES));
    pyramidMesh.LinkPositionToLocation(0);
    pyramidMesh.LinkTextureUVToLocation(1);
    // brick texture ----------------------------------------------------------------------------------------------------
    int brickTexWidth, brickTexHeight, _brickTexNCh;
    unsigned char *brickTexBytes = stbi_load("./brick.png", &brickTexWidth, &brickTexHeight, &_brickTexNCh, 0);
    gloo::Texture brickTex(brickTexBytes, brickTexWidth, brickTexHeight, gloo::Texture::Type::UnsignedByte, gloo::Texture::InternalFormat::RGBA, gloo::Texture::Format::RGBA, gloo::Texture::Slot::slot00, gloo::Texture::Target::Texture2D);
    pyramidShaderProgram.Uniform("in_texture", brickTex);
    brickTex.Bind();
    // framebuffer ------------------------------------------------------------------------------------------------------
    gloo::Framebuffer framebuffer;
    framebuffer.Bind();
    gloo::Texture renderTex(windowWidth, windowHeight, gloo::Texture::Type::UnsignedByte, gloo::Texture::InternalFormat::RGB, gloo::Texture::Format::RGB, gloo::Texture::Slot::slot00, gloo::Texture::Target::Texture2D);
    gloo::Renderbuffer renderbuffer;
    renderbuffer.RenderToTexture(renderTex);
    gloo::Framebuffer::UNBIND();
    // ==================================================================================================================

    // Variables that help the rotation of the pyramid
    float rotation = 0.0f;
    double prevTime = glfwGetTime();
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 eye = glm::vec3(0.0f);
    glm::vec3 lookat = glm::vec3(0.0f);
    glm::vec3 up = glm::vec3(0.0f, -1.0f, 0.0f);

    // Enables the Depth Buffer
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ImGui::Camera camera(eye, lookat, up);
    camera.setPosition(glm::vec3(0.0f + 2 * glm::cos(glm::radians(rotation)), 1.0f, 0.0f + 2 * glm::sin(glm::radians(rotation))));
    // ==================================================================================================================

    // Main loop
    while (!window.ShouldClose())
    {

        // ================================================================================================================
        framebuffer.Bind();
        glViewport(0, 0, windowWidth, windowHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right
        glClearColor(clear_darkblue.x, clear_darkblue.y, clear_darkblue.z, clear_darkblue.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ================================================================================================================
        double crntTime = glfwGetTime();
        if (crntTime - prevTime >= 1 / 60)
        {
            rotation += 0.5f;
            prevTime = crntTime;
        }
        // ----------------------------------------------------------------------------------------------------------------
        model = glm::mat4(1.0f);
        // draw pyramid ---------------------------------------------------------------------------------------------------
        brickTex.Bind();
        pyramidShaderProgram.Uniform("in_model", model);
        pyramidShaderProgram.Uniform("in_view", camera.view);
        pyramidShaderProgram.Uniform("in_projection", camera.projection);
        pyramidMesh.Draw();
        // draw plane -----------------------------------------------------------------------------------------------------
        planeShaderProgram.Uniform("in_view", camera.view);
        planeShaderProgram.Uniform("in_projection", camera.projection);
        planeMesh.Draw();
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
                viewSize.y = (int)(viewSize.x * renderTex.aspectRatio);
                viewWindowSize.y = viewSize.y + 14;

                viewSize.x = zoomMultiplier * viewSize.x;
                viewSize.y = zoomMultiplier * viewSize.y;

                ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6, 6));
                ImGui::BeginChild("canvas_child_window", viewWindowSize, true, ImGuiWindowFlags_None | ImGuiWindowFlags_HorizontalScrollbar);
                camera.Inputs(viewSize);
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

    stbi_image_free(brickTexBytes);

    return 0;
}
// ====================================================================================================================