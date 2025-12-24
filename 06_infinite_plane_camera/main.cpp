#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "glfw-cxx/Window.hpp"
#include "glfw-cxx/Error.hpp"
#include "glfw-cxx/Context.hpp"

#include "gloo/Texture.h"
#include "gloo/Shader.h"
#include "gloo/Program.h"
#include "gloo/Mesh.h"
#include "gloo/Camera.h"

#include "path.h"

const unsigned int width = 800;
const unsigned int height = 800;

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
int main()
{

    glfw::Error::SetErrorCallback(glfw_error_callback);

    // Decide GL+GLSL versions
    // GL 3.0 + GLSL 130
    glfw::Window window(width, height, "06 Infinite Plane Camera");
    window.Hint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    window.Hint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window.Hint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window.MakeContextCurrent();

    // Load GLAD so it configures OpenGL
    gladLoadGL();

    // Specify the viewport of OpenGL in the Window. In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
    glViewport(0, 0, width, height);

    // ================================================================================================================
    // plane shader ---------------------------------------------------------------------------------------------------
    gloo::Program planeShaderProgram(gloo::Shader(gloo::getFileContents(SRC_ROOT "/06_infinite_plane_camera/plane.vert"), gloo::Shader::Type::Vertex), gloo::Shader(gloo::getFileContents(SRC_ROOT "/06_infinite_plane_camera/plane.frag"), gloo::Shader::Type::Fragment));
    // plane mesh -----------------------------------------------------------------------------------------------------
    gloo::Mesh planeMesh(std::vector<gloo::Vertex>(PLANE_VERTICES), std::vector<GLuint>(PLANE_INDICES));
    planeMesh.LinkPositionToLocation(0);
    // pyramid shader -------------------------------------------------------------------------------------------------
    gloo::Program pyramidShaderProgram(gloo::Shader(gloo::getFileContents(SRC_ROOT "/06_infinite_plane_camera/pyramid.vert"), gloo::Shader::Type::Vertex), gloo::Shader(gloo::getFileContents(SRC_ROOT "/06_infinite_plane_camera/pyramid.frag"), gloo::Shader::Type::Fragment));
    // pyramid mesh ---------------------------------------------------------------------------------------------------
    gloo::Mesh pyramidMesh(std::vector<gloo::Vertex>(PYRAMID_VERTICES), std::vector<GLuint>(PYRAMID_INDICES));
    pyramidMesh.LinkPositionToLocation(0);
    pyramidMesh.LinkTextureUVToLocation(1);
    // brick texture --------------------------------------------------------------------------------------------------
    int brickTexWidth, brickTexHeight, _brickTexNCh;
    unsigned char *brickTexBytes = stbi_load(SRC_ROOT "/06_infinite_plane_camera/brick.png", &brickTexWidth, &brickTexHeight, &_brickTexNCh, 0);
    gloo::Texture brickTex(brickTexBytes, brickTexWidth, brickTexHeight, gloo::Texture::Type::UnsignedByte, gloo::Texture::InternalFormat::RGBA, gloo::Texture::Format::RGBA, gloo::Texture::Slot::slot00, gloo::Texture::Target::Texture2D);
    pyramidShaderProgram.Uniform("in_texture", brickTex);
    brickTex.Bind();
    // ================================================================================================================

    // Variables that help the rotation of the pyramid
    float rotation = 0.0f;
    double prevTime = glfwGetTime();
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 eye = glm::vec3(0.0f);
    glm::vec3 lookat = glm::vec3(0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    // Enables the Depth Buffer
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    gloo::Camera camera(eye, lookat, up);
    camera.setPosition(glm::vec3(0.0f + 2 * glm::cos(glm::radians(rotation)), 1.0f, 0.0f + 2 * glm::sin(glm::radians(rotation))));

    // Main while loop
    while (!window.ShouldClose())
    {
        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ============================================================================================================
        double crntTime = glfwGetTime();
        if (crntTime - prevTime >= 1 / 60)
        {
            rotation += 0.5f;
            prevTime = crntTime;
        }
        // ------------------------------------------------------------------------------------------------------------
        camera.Inputs(window);
        // camera.setPosition(glm::vec3(0.0f+2*glm::cos(glm::radians(rotation)), 1.0f, 0.0f+2*glm::sin(glm::radians(rotation))));
        // ------------------------------------------------------------------------------------------------------------
        model = glm::mat4(1.0f);
        // draw pyramid -----------------------------------------------------------------------------------------------
        pyramidShaderProgram.Uniform("in_model", model);
        pyramidShaderProgram.Uniform("in_view", camera.view);
        pyramidShaderProgram.Uniform("in_projection", camera.projection);
        pyramidMesh.Draw();
        // draw plane -------------------------------------------------------------------------------------------------
        planeShaderProgram.Uniform("in_view", camera.view);
        planeShaderProgram.Uniform("in_projection", camera.projection);
        planeMesh.Draw();
        // ============================================================================================================

        // Swap the back buffer with the front buffer
        window.SwapBuffers();
        // Take care of all GLFW events
        window.PollEvents();
    }

    stbi_image_free(brickTexBytes);

    return 0;
}