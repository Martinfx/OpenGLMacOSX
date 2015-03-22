SRCS = main.cpp ../sb6apps/myApp.cpp ../sb6apps/shader.cpp ../sb6apps/sb6ktx.cpp

all: $(SRCS)
	clang++ -o xogl $(SRCS) -I../sb6apps/ -I /usr/local/include -L/usr/local/lib -lglew -lglfw3 -framework opengl

