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





# ==== main source files ==============================================================================================
MAIN_EXE = 00_simple
MAIN_SOURCES = main.cpp
MAIN_OBJS = $(addprefix ., $(addsuffix .o, $(basename $(notdir $(MAIN_SOURCES)))))
# ==== main source files ==============================================================================================




# ==== gcc flags ======================================================================================================
CXXFLAGS = -Wall -Wformat -O3
CXXFLAGS += $(GLFW_CXX_CXXFLAGS)

LIBS = -lpthread
LIBS += $(GLFW_CXX_LIBS)

CFLAGS = $(CXXFLAGS)
CFLAGS_STD = $(CFLAGS) -std=c17
CXXFLAGS_STD = $(CXXFLAGS) -std=c++17
# ==== gcc flags ======================================================================================================




# ==== build rules ====================================================================================================
._glfw-cxx_%.o:$(GLFW_CXX_DIR)/src/%.cpp
	$(CXX) $(CXXFLAGS_STD) -c -o $@ $<

.%.o:%.cpp
	$(CXX) $(CXXFLAGS_STD) -c -o $@ $<

all: $(MAIN_EXE)
	@echo Build complete

$(MAIN_EXE): $(GLFW_CXX_OBJS) $(MAIN_OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean_main:
	rm -f $(MAIN_EXE) $(MAIN_OBJS)

clean_glfw-cxx:
	rm -f $(MAIN_EXE) $(GLFW_CXX_OBJS)

clean: clean_glfw-cxx clean_main
# ==== build rules ====================================================================================================