#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <chrono>

#include "glfw-cxx/Window.hpp"
#include "glfw-cxx/Error.hpp"
#include "glfw-cxx/Context.hpp"

#include "gloo/Texture.h"
#include "gloo/Shader.h"
#include "gloo/Program.h"
#include "gloo/Mesh.h"

#include "path.h"

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

std::vector<glm::vec2> positionStops{
    glm::vec2(0.25, 0.25),
    glm::vec2(0.25, 0.75),
    glm::vec2(0.75, 0.75),
    glm::vec2(0.75, 0.25)};
size_t nPositionStops = positionStops.size();

std::vector<float> radiusStops{
    0.1,
    0.05,
    0.2};
size_t nRadiusStops = radiusStops.size();

// ====================================================================================================================
static void glfw_error_callback(int error, const char *description)
{
    std::cerr << "GLFW Error" << error << ":" << description << "\n";
}
// ====================================================================================================================

int main()
{

    glfw::Error::SetErrorCallback(glfw_error_callback);

    // Decide GL+GLSL versions
    // GL 3.0 + GLSL 130
    glfw::Window window(windowWidth, windowHeight, "11 GPU Test");
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
    gloo::Program screenShaderProgram(gloo::Shader(gloo::getFileContents(SRC_ROOT "/11_gpu_cpu_compare/gpu_test/direct.vert"), gloo::Shader::Type::Vertex), gloo::Shader(gloo::getFileContents(SRC_ROOT "/11_gpu_cpu_compare/gpu_test/direct.frag"), gloo::Shader::Type::Fragment));
    gloo::Mesh screenMesh(std::vector<gloo::Vertex>(SCREEN_VERTICES), std::vector<GLuint>(SCREEN_INDICES));
    screenMesh.LinkPositionToLocation(0);
    screenMesh.LinkTextureUVToLocation(1);
    // ------------------------------------------------------------------------------------------------------------------
    // Stores the width, height, and the number of color channels of the image
    int texWidth, texHeight, _gridTexNCh;
    unsigned char *gridTexBytes = stbi_load(SRC_ROOT "/11_gpu_cpu_compare/gpu_test/grid.png", &texWidth, &texHeight, &_gridTexNCh, 0);
    gloo::Texture in_gridTex(gridTexBytes, texWidth, texHeight, gloo::Texture::Type::UnsignedByte, gloo::Texture::InternalFormat::RGBA, gloo::Texture::Format::RGBA, gloo::Texture::Slot::slot00, gloo::Texture::Target::Texture2D);
    static glm::vec2 in_circle_center(0.5, 0.5);
    static float in_circle_radius = 0.25;
    // ------------------------------------------------------------------------------------------------------------------
    screenShaderProgram.Uniform("in_image", in_gridTex);
    // ------------------------------------------------------------------------------------------------------------------
    size_t iStop_1 = 0;
    std::vector<glm::vec2> position_m(nPositionStops);
    std::vector<glm::vec2> position_c(nPositionStops);
    for (size_t iStop = 0; iStop < nPositionStops; iStop++)
    {
        iStop_1 = (iStop + 1) % nPositionStops;
        position_m[iStop] = glm::vec2(positionStops[iStop_1].x - positionStops[iStop].x, positionStops[iStop_1].y - positionStops[iStop].y);
        position_c[iStop] = glm::vec2(positionStops[iStop].x, positionStops[iStop].y);
    }
    // ------------------------------------------------------------------------------------------------------------------
    std::function<glm::vec2(float)> getPosition = [position_c, position_m](float _time_s)
    {
        int _int_time_s = (int)_time_s;
        float _segment_time_s = _time_s - _int_time_s;
        int _segment = _int_time_s % nPositionStops;
        return glm::vec2(position_c[_segment].x + position_m[_segment].x * _segment_time_s, position_c[_segment].y + position_m[_segment].y * _segment_time_s);
    };
    // ------------------------------------------------------------------------------------------------------------------
    iStop_1 = 0;
    std::vector<float> radius_m(nRadiusStops);
    std::vector<float> radius_c(nRadiusStops);
    for (size_t iStop = 0; iStop < nRadiusStops; iStop++)
    {
        iStop_1 = (iStop + 1) % nRadiusStops;
        radius_m[iStop] = radiusStops[iStop_1] - radiusStops[iStop];
        radius_c[iStop] = radiusStops[iStop];
    }
    // ------------------------------------------------------------------------------------------------------------------
    std::function<float(float)> getRadius = [radius_c, radius_m](float _time_s)
    {
        int _int_time_s = (int)_time_s;
        float _segment_time_s = _time_s - _int_time_s;
        int _segment = _int_time_s % nRadiusStops;
        return radius_c[_segment] + radius_m[_segment] * _segment_time_s;
    };
    // ------------------------------------------------------------------------------------------------------------------
    const std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
    std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
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
        now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_s = now - start;
        in_circle_center = getPosition(time_s.count());
        in_circle_radius = getRadius(time_s.count());

        screenShaderProgram.Uniform("in_center", in_circle_center);
        screenShaderProgram.Uniform("in_radius", in_circle_radius);
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