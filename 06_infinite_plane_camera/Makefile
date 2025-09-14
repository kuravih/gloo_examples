CXX = g++
CC = gcc

LIB_DIR = ../lib
# =====================================================================================================================




# ==== glfw-cxx source file ===========================================================================================
GLFW_CXX_DIR = $(LIB_DIR)/glfw-cxx

GLFW_CXX_SOURCES = $(GLFW_CXX_DIR)/src/Context.cpp $(GLFW_CXX_DIR)/src/Error.cpp $(GLFW_CXX_DIR)/src/EventCallbacks.cpp $(GLFW_CXX_DIR)/src/GammaRamp.cpp $(GLFW_CXX_DIR)/src/Init.cpp $(GLFW_CXX_DIR)/src/Monitor.cpp $(GLFW_CXX_DIR)/src/Time.cpp $(GLFW_CXX_DIR)/src/Version.cpp $(GLFW_CXX_DIR)/src/VideoMode.cpp $(GLFW_CXX_DIR)/src/WindowCallbacks.cpp $(GLFW_CXX_DIR)/src/Window.cpp
GLFW_CXX_OBJS = $(addprefix ._glfw-cxx_, $(addsuffix .o, $(basename $(notdir $(GLFW_CXX_SOURCES)))))

GLFW_CXX_CXXFLAGS = -I$(GLFW_CXX_DIR)/include `pkg-config --cflags glfw3`
GLFW_CXX_LIBS = `pkg-config --static --libs glfw3`
# ==== glfw-cxx source file ===========================================================================================




# ==== glad source file ===============================================================================================
GLAD_DIR = $(LIB_DIR)/glad

GLAD_SOURCES = $(GLAD_DIR)/src/glad.c # GLAD OPENGL LOADER
GLAD_OBJS = $(addprefix ._glad_, $(addsuffix .o, $(basename $(notdir $(GLAD_SOURCES)))))

GLAD_CXXFLAGS = -I$(GLAD_DIR)/include -DIMGUI_IMPL_OPENGL_LOADER_GLAD # GLAD OPENGL LOADER
# ==== glad source file ===============================================================================================




# ==== gloo source file ===============================================================================================
GLOO_DIR = $(LIB_DIR)/gloo

GLOO_SOURCES = $(GLOO_DIR)/src/Texture.cpp $(GLOO_DIR)/src/Shader.cpp $(GLOO_DIR)/src/Program.cpp $(GLOO_DIR)/src/Mesh.cpp $(GLOO_DIR)/src/VertexArrayObject.cpp $(GLOO_DIR)/src/VertexBufferObject.cpp $(GLOO_DIR)/src/ElementBufferObject.cpp $(GLOO_DIR)/src/Framebuffer.cpp $(GLOO_DIR)/src/Renderbuffer.cpp $(GLOO_DIR)/src/Camera.cpp
GLOO_OBJS = $(addprefix ._gloo_, $(addsuffix .o, $(basename $(notdir $(GLOO_SOURCES)))))

GLOO_CXXFLAGS = -I$(GLOO_DIR)/include
# ==== gloo source file ===============================================================================================




# ==== glm source file ================================================================================================
GLM_DIR = $(LIB_DIR)
GLM_CXXFLAGS = -I$(GLM_DIR)
# ==== glm source file ================================================================================================




# ==== stb source file ================================================================================================
STB_DIR = $(LIB_DIR)/stb
STB_CXXFLAGS = -I$(STB_DIR)
# ==== stb source file ================================================================================================




# ==== imgui source file ==============================================================================================
IMGUI_DIR = $(LIB_DIR)/imgui

IMGUI_SOURCES = $(IMGUI_DIR)/backend/imgui_impl_glfw.cpp $(IMGUI_DIR)/backend/imgui_impl_opengl3.cpp
IMGUI_SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_widgets.cpp $(IMGUI_DIR)/imgui_tables.cpp
IMGUI_OBJS = $(addprefix ._imgui_, $(addsuffix .o, $(basename $(notdir $(IMGUI_SOURCES)))))

IMGUI_CXXFLAGS = -I$(IMGUI_DIR)
# ==== imgui source file ==============================================================================================




# ==== main source files ==============================================================================================
MAIN_EXE = 06_infinite_plane_camera
MAIN_SOURCES = main.cpp
MAIN_OBJS = $(addprefix ., $(addsuffix .o, $(basename $(notdir $(MAIN_SOURCES)))))
# ==== main source files ==============================================================================================




# ==== gcc flags ======================================================================================================
CXXFLAGS = -Wall -Wformat -O3
CXXFLAGS += $(GLAD_CXXFLAGS)
CXXFLAGS += $(GLFW_CXX_CXXFLAGS)
CXXFLAGS += $(GLOO_CXXFLAGS)
CXXFLAGS += $(GLM_CXXFLAGS)
CXXFLAGS += $(STB_CXXFLAGS)
CXXFLAGS += $(IMGUI_CXXFLAGS)

LIBS = -lpthread
LIBS += $(GLFW_CXX_LIBS)

CFLAGS = $(CXXFLAGS)
CFLAGS_STD = $(CFLAGS) -std=c17
CXXFLAGS_STD = $(CXXFLAGS) -std=c++17
# ==== gcc flags ======================================================================================================




# ==== build rules ====================================================================================================
._glfw-cxx_%.o:$(GLFW_CXX_DIR)/src/%.cpp
	$(CXX) $(CXXFLAGS_STD) -c -o $@ $<

._glad_%.o:$(GLAD_DIR)/src/%.c
	$(CC) $(CFLAGS_STD) -c -o $@ $<

._gloo_%.o:$(GLOO_DIR)/src/%.cpp
	$(CXX) $(CXXFLAGS_STD) -c -o $@ $<

._stb_%.o:$(STB_DIR)/%.cpp
	$(CXX) $(CXXFLAGS_STD) -c -o $@ $<

._imgui_%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS_STD) -c -o $@ $<

._imgui_%.o:$(IMGUI_DIR)/backend/%.cpp
	$(CXX) $(CXXFLAGS_STD) -c -o $@ $<

.%.o:%.cpp $(MAIN_HEADERS)
	$(CXX) $(CXXFLAGS_STD) -c -o $@ $<

all: $(MAIN_EXE)
	@echo Build complete

$(MAIN_EXE): $(GLAD_OBJS) $(GLFW_CXX_OBJS) $(GLOO_OBJS) $(STB_OBJS) $(IMGUI_OBJS) $(MAIN_OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean_glad:
	rm -f $(MAIN_EXE) $(GLAD_OBJS)

clean_glfw-cxx:
	rm -f $(MAIN_EXE) $(GLFW_CXX_OBJS)

clean_gloo:
	rm -f $(MAIN_EXE) $(GLOO_OBJS)

clean_stb:
	rm -f $(MAIN_EXE) $(STB_OBJS)

clean_imgui:
	rm -f $(MAIN_EXE) $(IMGUI_OBJS)

clean_main:
	rm -f $(MAIN_EXE) $(MAIN_OBJS)

clean: clean_glad clean_glfw-cxx clean_gloo clean_stb clean_imgui clean_main
# ==== build rules ====================================================================================================