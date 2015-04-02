
CXX = clang++ -std=c++11
CXXFLAGS = -I /usr/local/include -I /usr/local/include/GLFW -I.. 

LIBFILES =  ../common/texture.cpp ../common/shader.cpp 
LDFLAGS = $(LIBFILES) -L /usr/local/lib \
          -lpng \
          -lglew -lglfw3 -framework opengl

all:
	$(CXX) $(CXXFLAGS) main.cpp -o texturedTriangle $(LDFLAGS)
