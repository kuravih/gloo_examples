glfw3_dep = dependency('glfw3')

conf = configure_file(
    input: 'path.h.in',
    output: 'path.h',
    configuration: {
        'SRC_ROOT': meson.project_source_root()
    }
)

lib_path = '../lib'

stb_inc = include_directories(lib_path)

glm_inc = include_directories(lib_path)

glad_path = lib_path + '/glad'
glad_inc = include_directories(glad_path + '/include')
glad_lib = static_library(
    'glad',
    sources : glad_path + '/src/glad.c',
    include_directories : glad_inc,
    c_args : [
        '-DIMGUI_IMPL_OPENGL_LOADER_GLAD',
        '-Wno-pedantic'
    ]
)

glfw_cxx_path = lib_path + '/glfw-cxx'
glfw_cxx_inc = include_directories(glfw_cxx_path + '/include')
glfw_cxx_sources = files(
    glfw_cxx_path + '/src/Context.cpp',
    glfw_cxx_path + '/src/Error.cpp',
    glfw_cxx_path + '/src/EventCallbacks.cpp',
    glfw_cxx_path + '/src/GammaRamp.cpp',
    glfw_cxx_path + '/src/Init.cpp',
    glfw_cxx_path + '/src/Monitor.cpp',
    glfw_cxx_path + '/src/Time.cpp',
    glfw_cxx_path + '/src/Version.cpp',
    glfw_cxx_path + '/src/VideoMode.cpp',
    glfw_cxx_path + '/src/WindowCallbacks.cpp',
    glfw_cxx_path + '/src/Window.cpp'
)
glfw_cxx_lib = static_library(
    'glfw-cxx',
    sources : glfw_cxx_sources,
    include_directories : glfw_cxx_inc
)

gloo_path = lib_path + '/gloo'
gloo_inc = include_directories(gloo_path + '/include')
gloo_sources = files(
    gloo_path + '/src/addon.cpp',
    gloo_path + '/src/Camera.cpp',
    gloo_path + '/src/ElementBufferObject.cpp',
    gloo_path + '/src/Framebuffer.cpp',
    gloo_path + '/src/Mesh.cpp',
    gloo_path + '/src/Program.cpp',
    gloo_path + '/src/Renderbuffer.cpp',
    gloo_path + '/src/Shader.cpp',
    gloo_path + '/src/Texture.cpp',
    gloo_path + '/src/VertexArrayObject.cpp',
    gloo_path + '/src/VertexBufferObject.cpp'
)
gloo_lib = static_library(
    'gloo',
    sources : gloo_sources,
    include_directories : [
        gloo_inc,
        glad_inc,
        glm_inc
    ],
    cpp_args : [
        '-DGLM_ENABLE_EXPERIMENTAL'
    ]
)

imgui_path = lib_path + '/imgui'
imgui_inc = include_directories(imgui_path)
imgui_sources = files(
    imgui_path + '/imgui.cpp',
    imgui_path + '/imgui_draw.cpp',
    imgui_path + '/imgui_widgets.cpp',
    imgui_path + '/imgui_tables.cpp',
    imgui_path + '/backend/imgui_impl_glfw.cpp',
    imgui_path + '/backend/imgui_impl_opengl3.cpp',
)
imgui_lib = static_library(
    'imgui',
    sources : imgui_sources,
    include_directories : [
        imgui_inc,
        glfw_cxx_inc,
        glad_inc
    ],
    cpp_args : [
        '-DIMGUI_IMPL_OPENGL_LOADER_GLAD'
    ]
)

executable(
    '12_multiple_shaders_imgui',
    ['main.cpp', 'ImguiWindow.cpp', conf],
    include_directories : [
        glfw_cxx_inc,
        stb_inc,
        imgui_inc,
        glad_inc,
        gloo_inc
    ],
    dependencies : glfw3_dep,
    link_whole : [
        glfw_cxx_lib,
        imgui_lib
    ],
    link_with: [
        glad_lib,
        gloo_lib
    ],
    install : false
)