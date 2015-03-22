//
//  main.cpp
//  sb6apps
//
//  Created by Yongduek Seo on 2015. 3. 22..
//  Copyright (c) 2015년 Yongduek Seo. All rights reserved.
//

#include <iostream>
#include "myApp.h"
#include "shader.hpp"

class myTriangle : public my::application {

    GLuint programID;
    GLuint vertexbuffer;
    GLuint VertexArrayID;
    
    void initGL() {
        // Dark blue background
        glClearColor(0.0f, 0.0f, 0.7f, 0.0f);
        
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);
        
        // Create and compile our GLSL program from the shaders
        programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );
        
        
        static const GLfloat g_vertex_buffer_data[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.0f,  1.0f, 0.0f,
        };
        
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        printf ("sizeof(g_vertex_buffer_data)= %lu\n", sizeof(g_vertex_buffer_data));
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(g_vertex_buffer_data), // Specifies the size in bytes of the buffer object's new data store.
                     g_vertex_buffer_data,
                     GL_STATIC_DRAW);
        
        printf("- initGL() finished.\n");
    }

    void closeGL() {
        // Cleanup VBO
        glDeleteBuffers(1, &vertexbuffer);
        glDeleteVertexArrays(1, &VertexArrayID);
        glDeleteProgram(programID);
        printf("- closeGL() finished.\n");
    }
    
    void render(double currentTime) {
        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT );
        
        // Use our shader
        glUseProgram(programID);
        
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0); // Specifies the index of the generic vertex attribute to be enabled or disabled.
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                              0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                              3,                  // size
                              GL_FLOAT,           // type
                              GL_FALSE,           // normalized?
                              0,                  // stride
                              (void*)0            // array buffer offset
                              );
        
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
        
        glDisableVertexAttribArray(0);
    }
    
    
};

//DECLARE_MAIN(myTriangle);

int main(int argc, const char ** argv)
{
    myTriangle *app = new myTriangle;
    app->run();
    delete app;
    return 0;
}
