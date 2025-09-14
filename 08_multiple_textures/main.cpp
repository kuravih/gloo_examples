#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "gloo/Texture.h"
#include "gloo/Shader.h"
#include "gloo/Program.h"
#include "gloo/Mesh.h"

#include "glfw-cxx/Window.hpp"
#include "glfw-cxx/Error.hpp"
#include "glfw-cxx/Context.hpp"

const unsigned int windowWidth = 1000;
const unsigned int windowHeight = 1000;

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

    // Decide GL+GLSL versions
    // GL 3.0 + GLSL 130
    glfw::Window window(windowWidth, windowHeight, "08 Multiple Textures");
    window.Hint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    window.Hint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window.Hint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window.MakeContextCurrent();

    // Load GLAD so it configures OpenGL
    gladLoadGL();

    // Specify the viewport of OpenGL in the Window. In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
    glViewport(0, 0, windowWidth, windowHeight);

    // ------------------------------------------------------------------------------------------------------------------
    // Generates Shader object using shaders shape.vert and shape.frag
    gloo::Program screenShaderProgram(gloo::Shader(gloo::getFileContents("./direct.vert"), gloo::Shader::Type::Vertex), gloo::Shader(gloo::getFileContents("./direct.frag"), gloo::Shader::Type::Fragment));
    gloo::Mesh screenMesh(std::vector<gloo::Vertex>(SCREEN_VERTICES), std::vector<GLuint>(SCREEN_INDICES));
    screenMesh.LinkPositionToLocation(0);
    screenMesh.LinkTextureUVToLocation(1);
    // ------------------------------------------------------------------------------------------------------------------
    // Stores the width, height, and the number of color channels of the image
    int texWidth, texHeight, _gridTexNCh;
    unsigned char *gridTexBytes = stbi_load("./grid.png", &texWidth, &texHeight, &_gridTexNCh, 0);
    gloo::Texture gridTex(gridTexBytes, texWidth, texHeight, gloo::Texture::Type::UnsignedByte, gloo::Texture::InternalFormat::RGBA, gloo::Texture::Format::RGBA, gloo::Texture::Slot::slot00, gloo::Texture::Target::Texture2D);
    // ------------------------------------------------------------------------------------------------------------------
    uint8_t *overlayTexBytes = new uint8_t[texWidth * texHeight]();
    for (int y = 0; y < texHeight; y++)
        for (int x = 0; x < texWidth; x++)
            overlayTexBytes[x + y * texWidth] = (uint8_t)pow(2.0, 16) * (float)x / (float)texWidth;
    gloo::Texture overlayTex(overlayTexBytes, texWidth, texHeight, gloo::Texture::Type::UnsignedByte, gloo::Texture::InternalFormat::Red, gloo::Texture::Format::Red, gloo::Texture::Slot::slot01, gloo::Texture::Target::Texture2D);
    // ------------------------------------------------------------------------------------------------------------------
    float norm_x2, norm_y2, norm_r;
    float center_x = 0.5, center_y = 0.5;
    uint8_t *maskTexBytes = new uint8_t[texWidth * texHeight]();
    for (int y = 0; y < texHeight; y++)
    {
        for (int x = 0; x < texWidth; x++)
        {
            norm_x2 = pow((float)x / (float)texWidth - center_x, 2);
            norm_y2 = pow((float)y / (float)texWidth - center_y * (float)texHeight / (float)texWidth, 2);
            norm_r = sqrt(norm_x2 + norm_y2);
            maskTexBytes[x + y * texWidth] = (unsigned char)pow(2.0, 8) * ((norm_r > 0.25) ? 0 : 1);
        }
    }
    gloo::Texture maskTex(maskTexBytes, texWidth, texHeight, gloo::Texture::Type::UnsignedByte, gloo::Texture::InternalFormat::Red, gloo::Texture::Format::Red, gloo::Texture::Slot::slot02, gloo::Texture::Target::Texture2D);
    // ------------------------------------------------------------------------------------------------------------------
    screenShaderProgram.Uniform("in_texture0", gridTex);
    screenShaderProgram.Uniform("in_texture1", overlayTex);
    screenShaderProgram.Uniform("in_texture2", maskTex);

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
        center_x = center_x + 0.005;
        center_x = fmod(center_x, 1.0);
        for (int y = 0; y < texHeight; y++)
        {
            for (int x = 0; x < texWidth; x++)
            {
                norm_x2 = pow((float)x / (float)texWidth - center_x, 2);
                norm_y2 = pow((float)y / (float)texWidth - center_y * (float)texHeight / (float)texWidth, 2);
                norm_r = sqrt(norm_x2 + norm_y2);
                maskTexBytes[x + y * texWidth] = (uint8_t)pow(2.0, 8) * ((norm_r > 0.25) ? 0 : 1);
            }
        }
        maskTex.Update(maskTexBytes);
        screenShaderProgram.Activate();
        screenMesh.Draw();
        // ----------------------------------------------------------------------------------------------------------------

        // Swap the back buffer with the front buffer
        window.SwapBuffers();
        // Take care of all GLFW events
        window.PollEvents();
    }

    stbi_image_free(gridTexBytes);

    delete overlayTexBytes;

    delete maskTexBytes;

    return 0;
}