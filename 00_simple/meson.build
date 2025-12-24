glfw3_dep = dependency('glfw3')

lib_path = '../lib'
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

executable(
    '00_simple',
    'main.cpp',
    include_directories : glfw_cxx_inc,
    dependencies : glfw3_dep,
    link_whole : glfw_cxx_lib,
    install : false
)