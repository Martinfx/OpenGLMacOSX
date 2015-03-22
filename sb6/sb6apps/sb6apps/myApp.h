//
//  myApp.h
//  sb6apps
//
//  Created by Yongduek Seo on 2015. 3. 22..
//  Copyright (c) 2015년 Yongduek Seo. All rights reserved.
//

#ifndef __sb6apps__myApp__
#define __sb6apps__myApp__

#include <string>
// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
using namespace glm;





namespace my {


struct AppInfo {
    union {
        int width;
        int windowWidth;
    };
    union {
        int height;
        int windowHeight;
    };
    std::string windowTitle;
    
    AppInfo() {
        width = 1024;
        height = 768;
        windowTitle = "OpenGL Window";
    }
};

class application {
    
protected:
//    static      my::application * app;
    my::AppInfo info;
    
private:
    GLFWwindow* window;
    
public:

    application() {}
    
    virtual void init() {}
    virtual void initGL () {}
    virtual void closeGL () {}
    
    virtual void render(double currentTime) {}
    
    virtual void run() {
//        virtual void run(my::application* the_app) {
//        app = the_app;
    
        init();
        
        // Initialise GLFW
        if( !glfwInit() ) {
            fprintf( stderr, "Failed to initialize GLFW\n" );
            exit (-1);
        }
        
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        
        // Open a window and create its OpenGL context
        window = glfwCreateWindow( info.width, info.height,
                                  info.windowTitle.c_str(), NULL, NULL);
        if( window == NULL ){
            fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
            glfwTerminate();
            exit (-1);
        }
        glfwMakeContextCurrent(window);
        
        // Initialize GLEW
        glewExperimental = true; // Needed for core profile
        if (glewInit() != GLEW_OK) {
            fprintf(stderr, "Failed to initialize GLEW\n");
            exit (-1);
        }
        
        // Ensure we can capture the escape key being pressed below
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
        
        this->initGL ();
        
        do {
            render(glfwGetTime());
            
            glfwSwapBuffers(window);
            glfwPollEvents();
        } while(
                (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS)
                && (glfwWindowShouldClose(window)==0)
                );
        
        this->closeGL();
       
        glfwTerminate();
    } // virtual void run()
};


}

#define DECLARE_MAIN(a)                             \
int main(int argc, const char ** argv)              \
{                                                   \
   a *app = new a;                                 \
   app->run();                                  \
   delete app;                                     \
   return 0;                                       \
}
#endif /* defined(__sb6apps__myApp__) */
