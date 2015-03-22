cmake_minimum_required (VERSION 2.8)
project (tutorial04)

find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)

add_executable(tutorial04.exe
	tutorial04.cpp
	../common/shader.cpp
	../common/shader.hpp
)
	
