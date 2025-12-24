#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glfw-cxx/Window.hpp"
#include "glfw-cxx/Error.hpp"

#include "gloo/Texture.h"
#include "gloo/Shader.h"
#include "gloo/Program.h"
#include "gloo/Mesh.h"
#include "path.h"

const unsigned int width = 800;
const unsigned int height = 800;

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

    glfw::Window window(width, height, "02 Simple Mesh");
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
    gloo::Program pyramidShaderProgram(gloo::Shader(gloo::getFileContents(SRC_ROOT "/02_simple_mesh/pyramid.vert"), gloo::Shader::Type::Vertex), gloo::Shader(gloo::getFileContents(SRC_ROOT "/02_simple_mesh/pyramid.frag"), gloo::Shader::Type::Fragment));
    gloo::Mesh pyramidMesh(std::vector<gloo::Vertex>(PYRAMID_VERTICES), std::vector<GLuint>(PYRAMID_INDICES));
    pyramidMesh.LinkPositionToLocation(0);
    pyramidMesh.LinkTextureUVToLocation(1);
    // ------------------------------------------------------------------------------------------------------------------
    // Stores the width, height, and the number of color channels of the image
    int brickTexWidth, brickTexHeight, _brickTexNCh;
    unsigned char *brickTexBytes = stbi_load(SRC_ROOT "/02_simple_mesh/brick.png", &brickTexWidth, &brickTexHeight, &_brickTexNCh, 0);
    gloo::Texture brickTex(brickTexBytes, brickTexWidth, brickTexHeight, gloo::Texture::Type::UnsignedByte, gloo::Texture::InternalFormat::RGBA, gloo::Texture::Format::RGBA, gloo::Texture::Slot::slot00, gloo::Texture::Target::Texture2D);
    pyramidShaderProgram.Uniform("in_tex0", brickTex);
    brickTex.Bind();
    // ------------------------------------------------------------------------------------------------------------------

    // Variables that help the rotation of the pyramid
    float rotation = 0.0f;
    double prevTime = glfwGetTime();

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
        double crntTime = glfwGetTime();
        if (crntTime - prevTime >= 1 / 60)
        {
            rotation += 0.5f;
            prevTime = crntTime;
        }
        // ---- Simple timer ----------------------------------------------------------------------------------------------
        // Initializes matrices so they are not the null matrix
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        // Assigns different transformations to each matrix
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

        pyramidShaderProgram.Activate();
        pyramidShaderProgram.Uniform("in_model", model);
        pyramidShaderProgram.Uniform("in_view", view);
        pyramidShaderProgram.Uniform("in_projection", projection);
        pyramidMesh.Draw();
        // ----------------------------------------------------------------------------------------------------------------

        // Swap the back buffer with the front buffer
        window.SwapBuffers();
        // Take care of all GLFW events
        window.PollEvents();
    }

    stbi_image_free(brickTexBytes);

    return 0;
}