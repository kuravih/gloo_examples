#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glm/glm.hpp>

#include "glfw-cxx/Window.hpp"
#include "glfw-cxx/Error.hpp"

#include "gloo/Texture.h"
#include "gloo/Shader.h"
#include "gloo/Program.h"
#include "gloo/Mesh.h"

const unsigned int width = 800;
const unsigned int height = 800;

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
int main()
{

    glfw::Error::SetErrorCallback(glfw_error_callback);

    glfw::Window window(width, height, "01 Simple Texture");
    window.Hint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    window.Hint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window.Hint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window.MakeContextCurrent();

    // Load GLAD so it configures OpenGL
    gladLoadGL();

    // Specify the viewport of OpenGL in the Window. In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
    glViewport(0, 0, width, height);

    // ------------------------------------------------------------------------------------------------------------------
    // Generates Shader object using shaders shape.vert and shape.frag
    gloo::Program screenShaderProgram(gloo::Shader(gloo::getFileContents("./direct.vert"), gloo::Shader::Type::Vertex), gloo::Shader(gloo::getFileContents("./direct.frag"), gloo::Shader::Type::Fragment));
    gloo::Mesh screenMesh(std::vector<gloo::Vertex>(SCREEN_VERTICES), std::vector<GLuint>(SCREEN_INDICES));
    screenMesh.LinkPositionToLocation(0);
    screenMesh.LinkTextureUVToLocation(1);
    // ------------------------------------------------------------------------------------------------------------------
    // Stores the width, height, and the number of color channels of the image
    int gridTexWidth, gridTexHeight, _gridTexNCh;
    unsigned char *gridTexBytes = stbi_load("./grid.png", &gridTexWidth, &gridTexHeight, &_gridTexNCh, 0);
    gloo::Texture gridTex(gridTexBytes, gridTexWidth, gridTexHeight, gloo::Texture::Type::UnsignedByte, gloo::Texture::InternalFormat::RGBA, gloo::Texture::Format::RGBA, gloo::Texture::Slot::slot00, gloo::Texture::Target::Texture2D);
    screenShaderProgram.Uniform("in_texture", gridTex);
    gridTex.Bind();
    // ------------------------------------------------------------------------------------------------------------------

    // Enables the Depth Buffer
    glEnable(GL_DEPTH_TEST);

    // Main while loop
    while (!window.ShouldClose())
    {
        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ---- Simple timer ----------------------------------------------------------------------------------------------
        screenShaderProgram.Activate();
        screenMesh.Draw();
        // ----------------------------------------------------------------------------------------------------------------

        // Swap the back buffer with the front buffer
        window.SwapBuffers();
        // Take care of all GLFW events
        window.PollEvents();
    }

    stbi_image_free(gridTexBytes);

    return 0;
}