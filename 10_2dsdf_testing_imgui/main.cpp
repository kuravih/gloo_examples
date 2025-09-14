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

#include "ImguiMouse.h"
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
    glfw::Window window(1920, 1200, "10 2dsdf Testing Imgui");
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

    std::string shaderNameArray[] = {"00_circle", "01_equitriangle", "02_isotriangle", "03_triangle", "04_box", "05_obox", "06_rhombus", "07_trapezoid", "08_parallelogram", "09_line", "10_capsule", "11_pentagon", "12_hexagon", "13_octagon", "14_hexagram", "15_pentagram", "16_ngram", "17_pie", "18_disk", "19_arc", "20_vesica", "21_crescent", "22_egg", "23_heart", "24_cross", "25_ex", "26_polygon", "27_ellipse", "28_tunnel"};
    static int shaderIndex = 0;

    // ==================================================================================================================
    gloo::Vertex screenVertices[] = {
        //             position                          normal           color            texUV
        gloo::Vertex{glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0), glm::vec3(0), glm::vec2(0.0f, 0.0f)}, // Top-left
        gloo::Vertex{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0), glm::vec3(0), glm::vec2(1.0f, 0.0f)},  // Top-right
        gloo::Vertex{glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0), glm::vec3(0), glm::vec2(1.0f, 1.0f)}, // Bottom-right
        gloo::Vertex{glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0), glm::vec3(0), glm::vec2(0.0f, 1.0f)} // Bottom-left
    };
    GLuint screenIndices[] = {
        0, 1, 2,
        2, 3, 0};
    std::vector<gloo::Vertex> screenVertexVector(screenVertices, screenVertices + sizeof(screenVertices) / sizeof(gloo::Vertex));
    std::vector<GLuint> screenIndexVector(screenIndices, screenIndices + sizeof(screenIndices) / sizeof(GLuint));

    gloo::Mesh screenMesh(screenVertexVector, screenIndexVector);
    screenMesh.LinkPositionToLocation(0);
    screenMesh.LinkTextureUVToLocation(1);
    // ==================================================================================================================
    gloo::Shader *p_vertShader = new gloo::Shader(gloo::getFileContents("./shaders/plane.vert"), gloo::Shader::Type::Vertex);
    gloo::Shader *p_fragShader = new gloo::Shader(gloo::getFileContents(("./shaders/" + shaderNameArray[shaderIndex] + ".frag").c_str()), gloo::Shader::Type::Fragment);
    gloo::Program *p_shaderProgram = new gloo::Program(*p_vertShader, *p_fragShader);
    // ==================================================================================================================
    glm::vec2 screenSize(1000, 1000);
    // ------------------------------------------------------------------------------------------------------------------
    gloo::Framebuffer framebuffer;
    framebuffer.Bind();
    gloo::Texture renderTex(screenSize.x, screenSize.y, gloo::Texture::Type::UnsignedByte, gloo::Texture::InternalFormat::RGBA, gloo::Texture::Format::RGBA, gloo::Texture::Slot::slot03, gloo::Texture::Target::Texture2D);
    gloo::Renderbuffer renderbuffer;
    renderbuffer.RenderToTexture(renderTex);
    gloo::Framebuffer::UNBIND();
    // ==================================================================================================================

    static ImVec4 color1 = ImVec4(0.90, 0.60, 0.30, 1.0), color2 = ImVec4(0.65, 0.85, 1.00, 1.0);
    static glm::vec3 in_color1(color1.x, color1.y, color1.z), in_color2(color2.x, color2.y, color2.z);
    static glm::vec2 in_mouse;

    // 00_circle parameters
    static glm::vec2 in_circle_center(0.5, 0.5);
    static float circle_center[] = {in_circle_center.x, in_circle_center.y};
    static float in_circle_radius = 0.25;

    // 01_equitriangle parameters
    static glm::vec2 in_equitriangle_circumcenter(0.5, 0.5);
    static float equitriangle_circumcenter[] = {in_equitriangle_circumcenter.x, in_equitriangle_circumcenter.y};
    static float in_equitriangle_circumradius = 0.25;

    // 02_isotriangle parameters
    static glm::vec2 in_isotriangle_center(0.5, 0.5);
    static float isotriangle_center[] = {in_isotriangle_center.x, in_isotriangle_center.y};
    static glm::vec2 in_isotriangle_lengths(0.25, 0.25);
    static float isotriangle_lengths[] = {in_isotriangle_lengths.x, in_isotriangle_lengths.y};

    // 03_triangle parameters
    static glm::vec2 in_triangle_point1(0.25, 0.25);
    static float triangle_point1[] = {in_triangle_point1.x, in_triangle_point1.y};
    static glm::vec2 in_triangle_point2(0.25, 0.5);
    static float triangle_point2[] = {in_triangle_point2.x, in_triangle_point2.y};
    static glm::vec2 in_triangle_point3(0.5, 0.75);
    static float triangle_point3[] = {in_triangle_point3.x, in_triangle_point3.y};

    // 04_box parameters
    static glm::vec2 in_box_center(0.5, 0.5);
    static float box_center[] = {in_box_center.x, in_box_center.y};
    static glm::vec2 in_box_sides(0.25, 0.25);
    static float box_sides[] = {in_box_sides.x, in_box_sides.y};

    // 05_obox parameters
    static glm::vec2 in_obox_point1(0.25, 0.25);
    static float obox_point1[] = {in_obox_point1.x, in_obox_point1.y};
    static glm::vec2 in_obox_point2(0.75, 0.75);
    static float obox_point2[] = {in_obox_point2.x, in_obox_point2.y};
    static float in_obox_thickness = 0.25;

    // 06_rhombus parameters
    static glm::vec2 in_rhombus_center(0.5, 0.5);
    static float rhombus_center[] = {in_rhombus_center.x, in_rhombus_center.y};
    static glm::vec2 in_rhombus_lengths(0.25, 0.25);
    static float rhombus_lengths[] = {in_rhombus_lengths.x, in_rhombus_lengths.y};

    // 07_trapezoid parameters
    static glm::vec2 in_trapezoid_center(0.5, 0.5);
    static float trapezoid_center[] = {in_trapezoid_center.x, in_trapezoid_center.y};
    static float in_trapezoid_length1 = 0.25;
    static float in_trapezoid_length2 = 0.25;
    static float in_trapezoid_height = 0.25;

    // 08_parallelogram parameters
    static glm::vec2 in_parallelogram_center(0.5, 0.5);
    static float parallelogram_center[] = {in_parallelogram_center.x, in_parallelogram_center.y};
    static float in_parallelogram_width = 0.25;
    static float in_parallelogram_height = 0.25;
    static float in_parallelogram_skewness = 0.25;

    // 09_line parameters
    static glm::vec2 in_line_point1(0.25, 0.25);
    static float line_point1[] = {in_line_point1.x, in_line_point1.y};
    static glm::vec2 in_line_point2(0.75, 0.75);
    static float line_point2[] = {in_line_point2.x, in_line_point2.y};

    // 10_capsule parameters
    static glm::vec2 in_capsule_point1(0.25, 0.25);
    static float capsule_point1[] = {in_capsule_point1.x, in_capsule_point1.y};
    static glm::vec2 in_capsule_point2(0.75, 0.75);
    static float capsule_point2[] = {in_capsule_point2.x, in_capsule_point2.y};
    static float in_capsule_radius1 = 0.25;
    static float in_capsule_radius2 = 0.25;

    // 11_pentagon parameters
    static glm::vec2 in_pentagon_center(0.5, 0.5);
    static float pentagon_center[] = {in_pentagon_center.x, in_pentagon_center.y};
    static float in_pentagon_radius = 0.25;

    // 12_hexagon parameters
    static glm::vec2 in_hexagon_center(0.5, 0.5);
    static float hexagon_center[] = {in_hexagon_center.x, in_hexagon_center.y};
    static float in_hexagon_radius = 0.25;

    // 13_octagon parameters
    static glm::vec2 in_octagon_center(0.5, 0.5);
    static float octagon_center[] = {in_octagon_center.x, in_octagon_center.y};
    static float in_octagon_radius = 0.25;

    // 14_hexagram parameters
    static glm::vec2 in_hexagram_center(0.5, 0.5);
    static float hexagram_center[] = {in_hexagram_center.x, in_hexagram_center.y};
    static float in_hexagram_radius = 0.25;

    // 15_pentagram parameters
    static glm::vec2 in_pentagram_center(0.5, 0.5);
    static float pentagram_center[] = {in_pentagram_center.x, in_pentagram_center.y};
    static float in_pentagram_radius = 0.25;

    // 16_ngram parameters
    static glm::vec2 in_ngram_center(0.5, 0.5);
    static float ngram_center[] = {in_ngram_center.x, in_ngram_center.y};
    static float in_ngram_radius = 0.2;
    static float in_ngram_angle = 2.5;
    static int in_ngram_npoints = 7;

    // 17_pie parameters
    static glm::vec2 in_pie_center(0.5, 0.5);
    static float pie_center[] = {in_pie_center.x, in_pie_center.y};
    static float in_pie_angle = 0.1;
    static float in_pie_radius = 0.25;

    // 18_disk parameters
    static glm::vec2 in_disk_center(0.5, 0.5);
    static float disk_center[] = {in_disk_center.x, in_disk_center.y};
    static float in_disk_radius = 0.1;
    static float in_disk_height = 0.25;

    // 19_arc parameters
    static glm::vec2 in_arc_center(0.5, 0.5);
    static float arc_center[] = {in_arc_center.x, in_arc_center.y};
    static float in_arc_angle = 0.1;
    static float in_arc_radius = 0.25;
    static float in_arc_thickness = 0.25;

    // 20_vesica parameters
    static glm::vec2 in_vesica_center(0.5, 0.5);
    static float vesica_center[] = {in_vesica_center.x, in_vesica_center.y};
    static float in_vesica_radius = 0.25;
    static float in_vesica_distance = 0.25;

    // 21_crescent parameters
    static glm::vec2 in_crescent_center(0.5, 0.5);
    static float crescent_center[] = {in_crescent_center.x, in_crescent_center.y};
    static float in_crescent_radius1 = 0.25;
    static float in_crescent_radius2 = 0.2;
    static float in_crescent_distance = 0.05;

    // 22_egg parameters
    static glm::vec2 in_egg_center(0.5, 0.5);
    static float egg_center[] = {in_egg_center.x, in_egg_center.y};
    static float in_egg_radius1 = 0.25;
    static float in_egg_radius2 = 0.2;

    // 23_heart parameters
    static glm::vec2 in_heart_center(0.5, 0.0);
    static float heart_center[] = {in_heart_center.x, in_heart_center.y};

    // 24_cross parameters
    static glm::vec2 in_cross_center(0.5, 0.5);
    static float cross_center[] = {in_cross_center.x, in_cross_center.y};
    static glm::vec2 in_cross_para(0.75, 0.25);
    static float cross_para[] = {in_cross_para.x, in_cross_para.y};
    static float in_cross_radius = 0.2;

    // 25_ex parameters
    static glm::vec2 in_ex_center(0.5, 0.5);
    static float ex_center[] = {in_ex_center.x, in_ex_center.y};
    static float in_ex_width = 0.2;
    static float in_ex_radius = 0.2;

    // 26_polygon parameters
    static glm::vec2 in_polygon_center(0.5, 0.5);
    static float polygon_center[] = {in_polygon_center.x, in_polygon_center.y};
#define POLYGON_NPOINTS 10
    static glm::vec2 in_polygon_point[POLYGON_NPOINTS];
    static float polygon_point[POLYGON_NPOINTS][2];

    // 27_ellipse parameters
    static glm::vec2 in_ellipse_center(0.5, 0.5);
    static float ellipse_center[] = {in_ellipse_center.x, in_ellipse_center.y};
    static glm::vec2 in_ellipse_axes(0.25 - 0.1, 0.25 + 0.1);
    static float ellipse_axes[] = {in_ellipse_axes.x, in_ellipse_axes.y};

    // 28_tunnel parameters
    static glm::vec2 in_tunnel_center(0.5, 0.5);
    static float tunnel_center[] = {in_tunnel_center.x, in_tunnel_center.y};
    static glm::vec2 in_tunnel_dims(0.5, 0.5);
    static float tunnel_dims[] = {in_tunnel_dims.x, in_tunnel_dims.y};

    ImVec2 viewWindowSize, viewSize = ImVec2(screenSize.x, screenSize.y);

    ImGui::Mouse mouse;

    // Main loop
    while (!window.ShouldClose())
    {

        // ================================================================================================================
        framebuffer.Bind();
        glViewport(0, 0, screenSize.x, screenSize.y); // Render on the whole framebuffer, complete from the lower left corner to the upper right
        // ----------------------------------------------------------------------------------------------------------------
        switch (shaderIndex)
        {
        case 0: // 00_circle
            p_shaderProgram->Uniform("in_radius", in_circle_radius);
            p_shaderProgram->Uniform("in_center", in_circle_center);
            break;
        case 1: // 01_equitriangle
            p_shaderProgram->Uniform("in_circumradius", in_equitriangle_circumradius);
            p_shaderProgram->Uniform("in_circumcenter", in_equitriangle_circumcenter);
            break;
        case 2: // 02_isotriangle
            p_shaderProgram->Uniform("in_center", in_isotriangle_center);
            p_shaderProgram->Uniform("in_lengths", in_isotriangle_lengths);
            break;
        case 3: // 03_triangle
            p_shaderProgram->Uniform("in_point1", in_triangle_point1);
            p_shaderProgram->Uniform("in_point2", in_triangle_point2);
            p_shaderProgram->Uniform("in_point3", in_triangle_point3);
            break;
        case 4: // 04_box
            p_shaderProgram->Uniform("in_sides", in_box_sides);
            p_shaderProgram->Uniform("in_center", in_box_center);
            break;
        case 5: // 05_obox
            p_shaderProgram->Uniform("in_point1", in_obox_point1);
            p_shaderProgram->Uniform("in_point2", in_obox_point2);
            p_shaderProgram->Uniform("in_thickness", in_obox_thickness);
            break;
        case 6: // 06_rhombus
            p_shaderProgram->Uniform("in_center", in_rhombus_center);
            p_shaderProgram->Uniform("in_lengths", in_rhombus_lengths);
            break;
        case 7: // 07_trapezoid
            p_shaderProgram->Uniform("in_center", in_trapezoid_center);
            p_shaderProgram->Uniform("in_length1", in_trapezoid_length1);
            p_shaderProgram->Uniform("in_length2", in_trapezoid_length2);
            p_shaderProgram->Uniform("in_height", in_trapezoid_height);
            break;
        case 8: // 08_parallelogram
            p_shaderProgram->Uniform("in_center", in_parallelogram_center);
            p_shaderProgram->Uniform("in_width", in_parallelogram_width);
            p_shaderProgram->Uniform("in_height", in_parallelogram_height);
            p_shaderProgram->Uniform("in_skewness", in_parallelogram_skewness);
            break;
        case 9: // 09_line
            p_shaderProgram->Uniform("in_point1", in_line_point1);
            p_shaderProgram->Uniform("in_point2", in_line_point2);
            break;
        case 10: // 10_capsule
            p_shaderProgram->Uniform("in_point1", in_capsule_point1);
            p_shaderProgram->Uniform("in_point2", in_capsule_point2);
            p_shaderProgram->Uniform("in_radius1", in_capsule_radius1);
            p_shaderProgram->Uniform("in_radius2", in_capsule_radius2);
            break;
        case 11: // 11_pentagon
            p_shaderProgram->Uniform("in_radius", in_pentagon_radius);
            p_shaderProgram->Uniform("in_center", in_pentagon_center);
            break;
        case 12: // 12_hexagon
            p_shaderProgram->Uniform("in_radius", in_hexagon_radius);
            p_shaderProgram->Uniform("in_center", in_hexagon_center);
            break;
        case 13: // 13_octagon
            p_shaderProgram->Uniform("in_radius", in_octagon_radius);
            p_shaderProgram->Uniform("in_center", in_octagon_center);
            break;
        case 14: // 14_hexagram
            p_shaderProgram->Uniform("in_radius", in_hexagram_radius);
            p_shaderProgram->Uniform("in_center", in_hexagram_center);
            break;
        case 15: // 15_pentagram
            p_shaderProgram->Uniform("in_radius", in_pentagram_radius);
            p_shaderProgram->Uniform("in_center", in_pentagram_center);
            break;
        case 16: // 16_ngram
            p_shaderProgram->Uniform("in_center", in_ngram_center);
            p_shaderProgram->Uniform("in_radius", in_ngram_radius);
            p_shaderProgram->Uniform("in_npoints", in_ngram_npoints);
            p_shaderProgram->Uniform("in_angle", in_ngram_angle);
            break;
        case 17: // 17_pie
            p_shaderProgram->Uniform("in_center", in_pie_center);
            p_shaderProgram->Uniform("in_angle", in_pie_angle);
            p_shaderProgram->Uniform("in_radius", in_pie_radius);
            break;
        case 18: // 18_disk
            p_shaderProgram->Uniform("in_center", in_disk_center);
            p_shaderProgram->Uniform("in_radius", in_disk_radius);
            p_shaderProgram->Uniform("in_height", in_disk_height);
            break;
        case 19: // 19_arc
            p_shaderProgram->Uniform("in_center", in_arc_center);
            p_shaderProgram->Uniform("in_angle", in_arc_angle);
            p_shaderProgram->Uniform("in_radius", in_arc_radius);
            p_shaderProgram->Uniform("in_thickness", in_arc_thickness);
            break;
        case 20: // 20_vesica
            p_shaderProgram->Uniform("in_center", in_vesica_center);
            p_shaderProgram->Uniform("in_radius", in_vesica_radius);
            p_shaderProgram->Uniform("in_distance", in_vesica_distance);
            break;
        case 21: // 21_crescent
            p_shaderProgram->Uniform("in_center", in_crescent_center);
            p_shaderProgram->Uniform("in_radius1", in_crescent_radius1);
            p_shaderProgram->Uniform("in_radius2", in_crescent_radius2);
            p_shaderProgram->Uniform("in_distance", in_crescent_distance);
            break;
        case 22: // 22_egg
            p_shaderProgram->Uniform("in_center", in_egg_center);
            p_shaderProgram->Uniform("in_radius1", in_egg_radius1);
            p_shaderProgram->Uniform("in_radius2", in_egg_radius2);
            break;
        case 23: // 23_heart
            p_shaderProgram->Uniform("in_center", in_heart_center);
            break;
        case 24: // 24_cross
            p_shaderProgram->Uniform("in_center", in_cross_center);
            p_shaderProgram->Uniform("in_para", in_cross_para);
            p_shaderProgram->Uniform("in_radius", in_cross_radius);
            break;
        case 25: // 25_ex
            p_shaderProgram->Uniform("in_center", in_ex_center);
            p_shaderProgram->Uniform("in_width", in_ex_width);
            p_shaderProgram->Uniform("in_radius", in_ex_radius);
            break;
        case 26: // 26_polygon
            p_shaderProgram->Uniform("in_center", in_polygon_center);
            // p_shaderProgram->Uniform("in_points", in_polygon_point, POLYGON_NPOINTS);
            break;
        case 27: // 27_ellipse
            p_shaderProgram->Uniform("in_center", in_ellipse_center);
            p_shaderProgram->Uniform("in_axes", in_ellipse_axes);
            break;
        case 28: // 28_tunnel
            p_shaderProgram->Uniform("in_center", in_tunnel_center);
            p_shaderProgram->Uniform("in_dims", in_tunnel_dims);
            break;
        default:
            break;
        }
        p_shaderProgram->Uniform("in_color1", in_color1);
        p_shaderProgram->Uniform("in_color2", in_color2);
        p_shaderProgram->Uniform("in_mouse", in_mouse);
        p_shaderProgram->Uniform("in_time", (float)ImGui::GetTime());
        // ----------------------------------------------------------------------------------------------------------------
        screenMesh.Draw();
        // ================================================================================================================
        gloo::Framebuffer::UNBIND();

        // Take care of all GLFW events
        window.PollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ================================================================================================================
        if (ImGui::Begin("2d signed distance function demo"))
        {

            if (ImGui::BeginCombo("Shader", shaderNameArray[shaderIndex].c_str()))
            {
                for (int index = 0; index < IM_ARRAYSIZE(shaderNameArray); index++)
                {
                    const bool is_selected = (shaderIndex == index);

                    if (ImGui::Selectable(shaderNameArray[index].c_str(), is_selected))
                    {
                        shaderIndex = index;
                        delete p_shaderProgram;
                        delete p_fragShader;
                        p_fragShader = new gloo::Shader(gloo::getFileContents(("./shaders/" + shaderNameArray[shaderIndex] + ".frag").c_str()), gloo::Shader::Type::Fragment);
                        p_shaderProgram = new gloo::Program(*p_vertShader, *p_fragShader);
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            switch (shaderIndex)
            {
            case 0: // 00_circle
                ImGui::SliderFloat("Radius##circle", &in_circle_radius, 0.0f, 1.0f, "%.3f");
                if (ImGui::SliderFloat2("Center##circle", circle_center, 0.0f, 1.0f))
                {
                    in_circle_center.x = circle_center[0];
                    in_circle_center.y = circle_center[1];
                }
                break;
            case 1: // 01_equitriangle
                ImGui::SliderFloat("Circumradius##equitriangle", &in_equitriangle_circumradius, 0.0f, 1.0f, "%.3f");
                if (ImGui::SliderFloat2("Circumcenter##equitriangle", equitriangle_circumcenter, 0.0f, 1.0f))
                {
                    in_equitriangle_circumcenter.x = equitriangle_circumcenter[0];
                    in_equitriangle_circumcenter.y = equitriangle_circumcenter[1];
                }
                break;
            case 2: // 02_isotriangle
                if (ImGui::SliderFloat2("Peak##isotriangle", isotriangle_center, 0.0f, 1.0f))
                {
                    in_isotriangle_center.x = isotriangle_center[0];
                    in_isotriangle_center.y = isotriangle_center[1];
                }
                if (ImGui::SliderFloat2("Lengths##isotriangle", isotriangle_lengths, 0.0f, 1.0f))
                {
                    in_isotriangle_lengths.x = isotriangle_lengths[0];
                    in_isotriangle_lengths.y = isotriangle_lengths[1];
                }
                break;
            case 3: // 03_triangle
                if (ImGui::SliderFloat2("Point 1##triangle", triangle_point1, 0.0f, 1.0f))
                {
                    in_triangle_point1.x = triangle_point1[0];
                    in_triangle_point1.y = triangle_point1[1];
                }
                if (ImGui::SliderFloat2("Point 2##triangle", triangle_point2, 0.0f, 1.0f))
                {
                    in_triangle_point2.x = triangle_point2[0];
                    in_triangle_point2.y = triangle_point2[1];
                }
                if (ImGui::SliderFloat2("Point 3##triangle", triangle_point3, 0.0f, 1.0f))
                {
                    in_triangle_point3.x = triangle_point3[0];
                    in_triangle_point3.y = triangle_point3[1];
                }
                break;
            case 4: // 04_box
                if (ImGui::SliderFloat2("Sides##box", box_sides, 0.0f, 1.0f))
                {
                    in_box_sides.x = box_sides[0];
                    in_box_sides.y = box_sides[1];
                }
                if (ImGui::SliderFloat2("Center##box", box_center, 0.0f, 1.0f))
                {
                    in_box_center.x = box_center[0];
                    in_box_center.y = box_center[1];
                }
                break;
            case 5: // 05_obox
                ImGui::SliderFloat("Thickness##obox", &in_obox_thickness, 0.0f, 1.0f, "%.3f");
                if (ImGui::SliderFloat2("Point 1##obox", obox_point1, 0.0f, 1.0f))
                {
                    in_obox_point1.x = obox_point1[0];
                    in_obox_point1.y = obox_point1[1];
                }
                if (ImGui::SliderFloat2("Point 2##obox", obox_point2, 0.0f, 1.0f))
                {
                    in_obox_point2.x = obox_point2[0];
                    in_obox_point2.y = obox_point2[1];
                }
                break;
            case 6: // 06_rhombus
                if (ImGui::SliderFloat2("Center##rhombus", rhombus_center, 0.0f, 1.0f))
                {
                    in_rhombus_center.x = rhombus_center[0];
                    in_rhombus_center.y = rhombus_center[1];
                }
                if (ImGui::SliderFloat2("Lengths##rhombus", rhombus_lengths, 0.0f, 1.0f))
                {
                    in_rhombus_lengths.x = rhombus_lengths[0];
                    in_rhombus_lengths.y = rhombus_lengths[1];
                }
                break;
            case 7: // 07_trapezoid
                if (ImGui::SliderFloat2("Center##trapezoid", trapezoid_center, 0.0f, 1.0f))
                {
                    in_trapezoid_center.x = trapezoid_center[0];
                    in_trapezoid_center.y = trapezoid_center[1];
                }
                ImGui::SliderFloat("Height##trapezoid", &in_trapezoid_height, 0.0f, 1.0f, "%.3f");
                ImGui::SliderFloat("Length 1##trapezoid", &in_trapezoid_length1, 0.0f, 1.0f, "%.3f");
                ImGui::SliderFloat("Length 2##trapezoid", &in_trapezoid_length2, 0.0f, 1.0f, "%.3f");
                break;
            case 8: // 08_parallelogram
                if (ImGui::SliderFloat2("Center##parallelogram", parallelogram_center, 0.0f, 1.0f))
                {
                    in_parallelogram_center.x = parallelogram_center[0];
                    in_parallelogram_center.y = parallelogram_center[1];
                }
                ImGui::SliderFloat("Width##parallelogram", &in_parallelogram_width, 0.0f, 1.0f, "%.3f");
                ImGui::SliderFloat("Height##parallelogram", &in_parallelogram_height, 0.0f, 1.0f, "%.3f");
                ImGui::SliderFloat("Skewness##parallelogram", &in_parallelogram_skewness, 0.0f, 1.0f, "%.3f");
                break;
            case 9: // 09_line
                if (ImGui::SliderFloat2("Point 1##line", line_point1, 0.0f, 1.0f))
                {
                    in_line_point1.x = line_point1[0];
                    in_line_point1.y = line_point1[1];
                }
                if (ImGui::SliderFloat2("Point 2##line", line_point2, 0.0f, 1.0f))
                {
                    in_line_point2.x = line_point2[0];
                    in_line_point2.y = line_point2[1];
                }
                break;
            case 10: // 10_capsule
                if (ImGui::SliderFloat2("Point 1##capsule", capsule_point1, 0.0f, 1.0f))
                {
                    in_capsule_point1.x = capsule_point1[0];
                    in_capsule_point1.y = capsule_point1[1];
                }
                if (ImGui::SliderFloat2("Point 2##capsule", capsule_point2, 0.0f, 1.0f))
                {
                    in_capsule_point2.x = capsule_point2[0];
                    in_capsule_point2.y = capsule_point2[1];
                }
                ImGui::SliderFloat("Radius 1##capsule", &in_capsule_radius1, 0.0f, 1.0f, "%.3f");
                ImGui::SliderFloat("Radius 2##capsule", &in_capsule_radius2, 0.0f, 1.0f, "%.3f");
                break;
            case 11: // 11_pentagon
                ImGui::SliderFloat("Radius##pentagon", &in_pentagon_radius, 0.0f, 1.0f, "%.3f");
                if (ImGui::SliderFloat2("Center##pentagon", pentagon_center, 0.0f, 1.0f))
                {
                    in_pentagon_center.x = pentagon_center[0];
                    in_pentagon_center.y = pentagon_center[1];
                }
                break;
            case 12: // 12_hexagon
                ImGui::SliderFloat("Radius##hexagon", &in_hexagon_radius, 0.0f, 1.0f, "%.3f");
                if (ImGui::SliderFloat2("Center##hexagon", hexagon_center, 0.0f, 1.0f))
                {
                    in_hexagon_center.x = hexagon_center[0];
                    in_hexagon_center.y = hexagon_center[1];
                }
                break;
            case 13: // 13_octagon
                ImGui::SliderFloat("Radius##octagon", &in_octagon_radius, 0.0f, 1.0f, "%.3f");
                if (ImGui::SliderFloat2("Center##octagon", octagon_center, 0.0f, 1.0f))
                {
                    in_octagon_center.x = octagon_center[0];
                    in_octagon_center.y = octagon_center[1];
                }
                break;
            case 14: // 14_hexagram
                ImGui::SliderFloat("Radius##hexagram", &in_hexagram_radius, 0.0f, 1.0f, "%.3f");
                if (ImGui::SliderFloat2("Center##hexagram", hexagram_center, 0.0f, 1.0f))
                {
                    in_hexagram_center.x = hexagram_center[0];
                    in_hexagram_center.y = hexagram_center[1];
                }
                break;
            case 15: // 15_pentagram
                ImGui::SliderFloat("Radius##pentagram", &in_pentagram_radius, 0.0f, 1.0f, "%.3f");
                if (ImGui::SliderFloat2("Center##pentagram", pentagram_center, 0.0f, 1.0f))
                {
                    in_pentagram_center.x = pentagram_center[0];
                    in_pentagram_center.y = pentagram_center[1];
                }
                break;
            case 16: // 16_ngram
                if (ImGui::SliderFloat2("Center##ngram", ngram_center, 0.0f, 1.0f))
                {
                    in_ngram_center.x = ngram_center[0];
                    in_ngram_center.y = ngram_center[1];
                }
                ImGui::SliderFloat("Radius 1##ngram", &in_ngram_radius, 0.0f, 1.0f, "%.3f");
                ImGui::SliderInt("N Points##ngram", &in_ngram_npoints, 3, 24, "%d");
                ImGui::SliderFloat("Angle##ngram", &in_ngram_angle, 2.0f, in_ngram_npoints, "%.3f");
                break;
            case 17: // 16_pie
                if (ImGui::SliderFloat2("Center##pie", pie_center, 0.0f, 1.0f))
                {
                    in_pie_center.x = pie_center[0];
                    in_pie_center.y = pie_center[1];
                }
                ImGui::SliderFloat("Angle##pie", &in_pie_angle, 0.0f, glm::pi<float>(), "%.3f");
                ImGui::SliderFloat("Radius##pie", &in_pie_radius, 0.0f, 1.0f, "%.3f");
                break;
            case 18: // 18_disk
                if (ImGui::SliderFloat2("Center##disk", disk_center, 0.0f, 1.0f))
                {
                    in_disk_center.x = disk_center[0];
                    in_disk_center.y = disk_center[1];
                }
                ImGui::SliderFloat("Radius##disk", &in_disk_radius, 0.0f, 1.0f, "%.3f");
                ImGui::SliderFloat("Height##disk", &in_disk_height, -in_disk_radius, in_disk_radius, "%.3f");
                break;
            case 19: // 19_arc
                if (ImGui::SliderFloat2("Center##arc", arc_center, 0.0f, 1.0f))
                {
                    in_arc_center.x = arc_center[0];
                    in_arc_center.y = arc_center[1];
                }
                ImGui::SliderFloat("Angle##arc", &in_arc_angle, 0.0f, glm::pi<float>(), "%.3f");
                ImGui::SliderFloat("Radius 1##arc", &in_arc_radius, 0.0f, 1.0f, "%.3f");
                ImGui::SliderFloat("Radius 2##arc", &in_arc_thickness, 0.0f, 1.0f, "%.3f");
                break;
            case 20: // 20_vesica
                if (ImGui::SliderFloat2("Center##vesica", vesica_center, 0.0f, 1.0f))
                {
                    in_vesica_center.x = vesica_center[0];
                    in_vesica_center.y = vesica_center[1];
                }
                ImGui::SliderFloat("Radius##vesica", &in_vesica_radius, 0.0f, 0.5f, "%.3f");
                ImGui::SliderFloat("Distance##vesica", &in_vesica_distance, 0.0f, in_vesica_radius, "%.3f");
                break;
            case 21: // 21_crescent
                if (ImGui::SliderFloat2("Center##crescent", crescent_center, 0.0f, 1.0f))
                {
                    in_crescent_center.x = crescent_center[0];
                    in_crescent_center.y = crescent_center[1];
                }
                ImGui::SliderFloat("Radius 1##crescent", &in_crescent_radius1, 0.0f, 0.5f, "%.3f");
                ImGui::SliderFloat("Radius 2##crescent", &in_crescent_radius2, 0.0f, 0.5f, "%.3f");
                ImGui::SliderFloat("Distance##crescent", &in_crescent_distance, 0.0f, 1.0f, "%.3f");
                break;
            case 22: // 22_egg
                if (ImGui::SliderFloat2("Center##egg", egg_center, 0.0f, 1.0f))
                {
                    in_egg_center.x = egg_center[0];
                    in_egg_center.y = egg_center[1];
                }
                ImGui::SliderFloat("Radius 1##egg", &in_egg_radius1, 0.0f, 0.5f, "%.3f");
                ImGui::SliderFloat("Radius 2##egg", &in_egg_radius2, 0.0f, 0.5f, "%.3f");
                break;
            case 23: // 23_heart
                if (ImGui::SliderFloat2("Center##heart", heart_center, 0.0f, 1.0f))
                {
                    in_heart_center.x = heart_center[0];
                    in_heart_center.y = heart_center[1];
                }
                break;
            case 24: // 24_cross
                if (ImGui::SliderFloat2("Center##cross", cross_center, 0.0f, 1.0f))
                {
                    in_cross_center.x = cross_center[0];
                    in_cross_center.y = cross_center[1];
                }
                if (ImGui::SliderFloat2("Para##cross", cross_para, 0.0f, 1.0f))
                {
                    in_cross_para.x = cross_para[0];
                    in_cross_para.y = cross_para[1];
                }
                ImGui::SliderFloat("Radius##cross", &in_cross_radius, 0.0f, 0.5f, "%.3f");
                break;
            case 25: // 25_ex
                if (ImGui::SliderFloat2("Center##ex", ex_center, 0.0f, 1.0f))
                {
                    in_ex_center.x = ex_center[0];
                    in_ex_center.y = ex_center[1];
                }
                ImGui::SliderFloat("Radius##ex", &in_ex_radius, 0.0f, 0.5f, "%.3f");
                ImGui::SliderFloat("Width##ex", &in_ex_width, 0.0f, 0.5f, "%.3f");
                break;
            case 26: // 26_polygon
                if (ImGui::SliderFloat2("Center##polygon", polygon_center, 0.0f, 1.0f))
                {
                    in_polygon_center.x = polygon_center[0];
                    in_polygon_center.y = polygon_center[1];
                }
                for (size_t iPoint = 0; iPoint < POLYGON_NPOINTS; iPoint++)
                {
                    if (ImGui::SliderFloat2(("Point " + std::to_string(iPoint) + "##polygon").c_str(), polygon_point[iPoint], 0.0f, 1.0f))
                    {
                        in_polygon_point[iPoint].x = polygon_point[iPoint][0];
                        in_polygon_point[iPoint].y = polygon_point[iPoint][1];
                    }
                }
                break;
            case 27: // 27_ellipse
                if (ImGui::SliderFloat2("Center##ellipse", ellipse_center, 0.0f, 1.0f))
                {
                    in_ellipse_center.x = ellipse_center[0];
                    in_ellipse_center.y = ellipse_center[1];
                }
                if (ImGui::SliderFloat2("Axes##ellipse", ellipse_axes, 0.25f - 0.1f, 0.25f + 0.1f))
                {
                    in_ellipse_axes.x = ellipse_axes[0];
                    in_ellipse_axes.y = ellipse_axes[1];
                }
                break;
            case 28: // 28_tunnel
                if (ImGui::SliderFloat2("Center##tunnel", tunnel_center, 0.0f, 1.0f))
                {
                    in_tunnel_center.x = tunnel_center[0];
                    in_tunnel_center.y = tunnel_center[1];
                }
                if (ImGui::SliderFloat2("Dimensions##tunnel", tunnel_dims, 0.0f, 1.0f))
                {
                    in_tunnel_dims.x = tunnel_dims[0];
                    in_tunnel_dims.y = tunnel_dims[1];
                }
                break;
            default:
                break;
            }

            if (ImGui::ColorEdit4("Color 1", (float *)&color1, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_PickerHueBar))
            {
                in_color1.x = color1.x;
                in_color1.y = color1.y;
                in_color1.z = color1.z;
            }

            if (ImGui::ColorEdit4("Color 2", (float *)&color2, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_PickerHueBar))
            {
                in_color2.x = color2.x;
                in_color2.y = color2.y;
                in_color2.z = color2.z;
            }

            {
                viewWindowSize.x = ImGui::GetContentRegionAvail().x;
                viewSize.x = viewWindowSize.x - 14;
                viewSize.y = (int)(viewSize.x * renderTex.aspectRatio);
                viewWindowSize.y = viewSize.y + 14;
                ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6, 6));
                ImGui::BeginChild("canvas_child_window", viewWindowSize, true, ImGuiWindowFlags_None | ImGuiWindowFlags_HorizontalScrollbar);
                if (mouse.Inputs(viewSize))
                {
                    in_mouse.x = mouse.position.x / viewSize.x;
                    in_mouse.y = mouse.position.y / viewSize.y;
                }
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
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    delete p_vertShader;
    delete p_fragShader;
    delete p_shaderProgram;

    ImGui::DestroyContext();

    return 0;
}
// ====================================================================================================================