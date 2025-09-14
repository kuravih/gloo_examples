#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "glfw-cxx/Window.hpp"
#include "glfw-cxx/Error.hpp"

#include "gloo/Texture.h"
#include "gloo/Shader.h"
#include "gloo/Program.h"
#include "gloo/Mesh.h"
#include "gloo/Framebuffer.h"
#include "gloo/Renderbuffer.h"
#include "gloo/Camera.h"

const unsigned int windowWidth = 800;
const unsigned int windowHeight = 800;

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
int main()
{

    glfw::Error::SetErrorCallback(glfw_error_callback);

    glfw::Window window;

    window.Create(windowWidth, windowHeight, "03 Render Texture");
    window.Hint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    window.Hint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window.Hint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window.MakeContextCurrent();

    // Load GLAD so it configures OpenGL
    gladLoadGL();

    // Specify the viewport of OpenGL in the Window. In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
    glViewport(0, 0, windowWidth, windowHeight);

    glm::vec4 clear(0.45f, 0.55f, 0.60f, 1.00f);
    glm::vec4 darkBlue(0.07f, 0.13f, 0.17f, 1.0f);

    // ------------------------------------------------------------------------------------------------------------------
    // Generates Shader object using shaders shape.vert and shape.frag
    gloo::Program pyramidShaderProgram(gloo::Shader(gloo::getFileContents("./pyramid.vert"), gloo::Shader::Type::Vertex), gloo::Shader(gloo::getFileContents("./pyramid.frag"), gloo::Shader::Type::Fragment));
    gloo::Mesh pyramidMesh(std::vector<gloo::Vertex>(PYRAMID_VERTICES), std::vector<GLuint>(PYRAMID_INDICES));
    pyramidMesh.LinkPositionToLocation(0);
    pyramidMesh.LinkTextureUVToLocation(1);
    // ------------------------------------------------------------------------------------------------------------------
    // Stores the width, height, and the number of color channels of the image
    int brickTexWidth, brickTexHeight, _brickTexNCh;
    unsigned char *brickTexBytes = stbi_load("./brick.png", &brickTexWidth, &brickTexHeight, &_brickTexNCh, 0);
    gloo::Texture brickTex(brickTexBytes, brickTexWidth, brickTexHeight, gloo::Texture::Type::UnsignedByte, gloo::Texture::InternalFormat::RGBA, gloo::Texture::Format::RGBA, gloo::Texture::Slot::slot01, gloo::Texture::Target::Texture2D);
    pyramidShaderProgram.Uniform("in_texture", brickTex);
    brickTex.Bind();
    // ------------------------------------------------------------------------------------------------------------------

    // ------------------------------------------------------------------------------------------------------------------
    // Generates Shader object using shaders shape.vert and shape.frag
    gloo::Program screenShaderProgram(gloo::Shader(gloo::getFileContents("./screen.vert"), gloo::Shader::Type::Vertex), gloo::Shader(gloo::getFileContents("./screen.frag"), gloo::Shader::Type::Fragment));
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

    // ------------------------------------------------------------------------------------------------------------------
    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    gloo::Framebuffer framebuffer;
    framebuffer.Bind();
    gloo::Texture renderTex(windowWidth, windowHeight, gloo::Texture::Type::UnsignedByte, gloo::Texture::InternalFormat::RGB, gloo::Texture::Format::RGB, gloo::Texture::Slot::slot00, gloo::Texture::Target::Texture2D);
    gloo::Renderbuffer renderbuffer;
    renderbuffer.RenderToTexture(renderTex);
    // ------------------------------------------------------------------------------------------------------------------

    // Variables that help the rotation of the pyramid
    float rotation = 0.0f;
    double prevTime = glfwGetTime();
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 eye = glm::vec3(0.0f);
    glm::vec3 lookat = glm::vec3(0.0f, 0.2f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, -1.0f, 0.0f);

    // Enables the Depth Buffer
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Specify the color of the background
    glClearColor(clear.x, clear.y, clear.y, clear.w);

    gloo::Camera camera(eye, lookat, up);

    // Main while loop
    while (!window.ShouldClose())
    {

        // ================================================================================================================
        framebuffer.Bind();
        glViewport(0, 0, windowWidth, windowHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right
        glClearColor(darkBlue.x, darkBlue.y, darkBlue.y, darkBlue.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ---- Simple timer ----------------------------------------------------------------------------------------------
        double crntTime = glfwGetTime();
        if (crntTime - prevTime >= 1 / 60)
        {
            rotation += 0.5f;
            prevTime = crntTime;
        }
        // ----------------------------------------------------------------------------------------------------------------
        camera.setPosition(glm::vec3(0.0f + 2 * glm::cos(glm::radians(rotation)), 1.0f, 0.0f + 2 * glm::sin(glm::radians(rotation))));
        // ----------------------------------------------------------------------------------------------------------------
        model = glm::mat4(1.0f);
        // draw pyramid ---------------------------------------------------------------------------------------------------
        pyramidMesh.vao.Bind();
        pyramidShaderProgram.Uniform("in_model", model);
        pyramidShaderProgram.Uniform("in_view", camera.view);
        pyramidShaderProgram.Uniform("in_projection", camera.projection);
        pyramidMesh.Draw();
        // ================================================================================================================
        gloo::Framebuffer::UNBIND();

        glViewport(0, 0, windowWidth, windowHeight);
        glClearColor(clear.x, clear.y, clear.y, clear.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ---- Simple timer ----------------------------------------------------------------------------------------------
        screenMesh.vao.Bind();
        screenShaderProgram.Activate();
        renderTex.Bind();
        renderTex.Update();
        screenMesh.Draw();
        // ----------------------------------------------------------------------------------------------------------------

        // Swap the back buffer with the front buffer
        window.SwapBuffers();
        // Take care of all GLFW events
        window.PollEvents();
    }

    stbi_image_free(brickTexBytes);
    stbi_image_free(gridTexBytes);
    return 0;
}