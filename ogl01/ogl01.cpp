//
// Your First Look at an OpenGL Program
//---------------------------------------------------------------------
//
// clang++ -o ogl01 ogl01.cpp LoadShaders.cpp -I /usr/local/include -lglew -lglfw3 -framework opengl

// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include <iostream>
using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
GLFWwindow* window;

#include "LoadShaders.h"
#define BUFFER_OFFSET(a) (a)

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };
GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
const GLuint NumVertices = 6;
// init
//
void myInit(void)
{
    glGenVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[Triangles]);
    GLfloat vertices[NumVertices][2] = {
        { -0.90, -0.90 }, // Triangle 1
        { 0.85, -0.90 },
        { -0.90, 0.85 },
        { 0.90, -0.85 }, // Triangle 2
        { 0.90, 0.90 },
        { -0.85, 0.90 }
    };
    glGenBuffers(NumBuffers, Buffers);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
                 vertices, GL_STATIC_DRAW);
    ShaderInfo shaders[] = {
        { GL_VERTEX_SHADER, "triangles.vert" },
        { GL_FRAGMENT_SHADER, "triangles.frag" },
        { GL_NONE, NULL }
    };
    GLuint program = LoadShaders(shaders);
    if (program == 0) {
        cerr << "** LoadShaders() returned error" << endl;
    }
    glUseProgram(program);
    glVertexAttribPointer(vPosition, 2, GL_FLOAT,
                          GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vPosition);
}
//---------------------------------------------------------------------
//
// display
//
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VAOs[Triangles]);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
    glFlush();
}


int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
        {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
        }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Tutorial 02 - Red triangle", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    // -------------------------------------------------------------------
    //  now opengl

    // OpenGL version check
    const GLubyte *glVersionString = glGetString (GL_VERSION);
    std::cerr << "OpenGL Version = " << glVersionString << std::endl;
    
    myInit ();
    
    do{
        myDisplay ();
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );
    
    // Cleanup VBO
//    glDeleteBuffers(1, &vertexbuffer);
//    glDeleteVertexArrays(1, &VertexArrayID);
//    glDeleteProgram(programID);
//    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    
    return 0;
}

