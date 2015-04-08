all:
	@echo "Please READ Tutorial 1 and don't build the tutorials yourself ! Use CMake instead. If you have a problem, read the FAQ"

CXX = clang++ -std=c++11
CXXFLAGS = -I /usr/local/include -I /usr/local/include/GLFW -I ..

LIBFILES =  ../common/shader.cpp ../common/texture.cpp ../common/controls.cpp
LDFLAGS = $(LIBFILES) -L /usr/local/lib -lglew -lglfw3 -framework opengl
