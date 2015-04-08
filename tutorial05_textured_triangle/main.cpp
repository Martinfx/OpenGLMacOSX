// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//using namespace glm;

#include "common/shader.hpp"
#include "common/texture.hpp"


// This one file (png_texture.cpp) is free and unencumbered software
// released into the public domain.

#include <png.h>

GLuint png_texture_load(const char * file_name, int * width, int * height)
{
    // This function was originally written by David Grayson for
    // https://github.com/DavidEGrayson/ahrs-visualizer
    
    png_byte header[8];
    
    FILE *fp = fopen(file_name, "rb");
    if (fp == 0)
        {
        perror(file_name);
        return 0;
        }
    
    // read the header
    fread(header, 1, 8, fp);
    
    if (png_sig_cmp(header, 0, 8))
        {
        fprintf(stderr, "error: %s is not a PNG.\n", file_name);
        fclose(fp);
        return 0;
        }
    
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
        {
        fprintf(stderr, "error: png_create_read_struct returned 0.\n");
        fclose(fp);
        return 0;
        }
    
    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
        {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(fp);
        return 0;
        }
    
    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info)
        {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        return 0;
        }
    
    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "error from libpng\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }
    
    // init png reading
    png_init_io(png_ptr, fp);
    
    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);
    
    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);
    
    // variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 temp_width, temp_height;
    
    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
                 NULL, NULL, NULL);
    
    if (width){ *width = temp_width; }
    if (height){ *height = temp_height; }
    
    //printf("%s: %lux%lu %d\n", file_name, temp_width, temp_height, color_type);
    
    if (bit_depth != 8)
        {
        fprintf(stderr, "%s: Unsupported bit depth %d.  Must be 8.\n", file_name, bit_depth);
        return 0;
        }
    
    GLint format;
    switch(color_type)
    {
        case PNG_COLOR_TYPE_RGB:
        format = GL_RGB;
	fprintf(stderr, "%s: image type = GL_RGB\n", file_name);
        break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
        format = GL_RGBA;
	fprintf(stderr, "%s: image type = GL_RGBA\n", file_name);
        break;
        default:
        fprintf(stderr, "%s: Unknown libpng color type %d.\n", file_name, color_type);
        return 0;
    }
    
    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);
    
    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    
    // glTexImage2d requires rows to be 4-byte aligned
    rowbytes += 3 - ((rowbytes-1) % 4);
    
    // Allocate the image_data as a big block, to be given to opengl
    png_byte * image_data = (png_byte *)malloc(rowbytes * temp_height * sizeof(png_byte)+15);
    if (image_data == NULL)
        {
        fprintf(stderr, "error: could not allocate memory for PNG image data\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
        }
    
    // row_pointers is for pointing to image_data for reading the png with libpng
    png_byte ** row_pointers = (png_byte **)malloc(temp_height * sizeof(png_byte *));
    if (row_pointers == NULL)
        {
        fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(fp);
        return 0;
        }
    
    // set the individual row_pointers to point at the correct offsets of image_data
    for (unsigned int i = 0; i < temp_height; i++)
        {
        row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
        }
    
    // read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);

    // clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    free(row_pointers);
    fclose(fp);

    // Generate the OpenGL texture object
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, temp_width, temp_height, 0, format, GL_UNSIGNED_BYTE, image_data);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    return texture; // return the texture name allocated.
} // png_texture_load() with libpng

int main( void )
{
    std::cerr << "Learing various combination of Projection, Viewing, Modeling" << std::endl;
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
    window = glfwCreateWindow( 1024, 768, "Tutorial 03 - Matrices", NULL, NULL);
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
    
    // print out the OpenGL Version Information
    const GLubyte *version_string = glGetString (GL_VERSION);
    std::cout << "OpenGL: " << version_string << std::endl;
    
    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glClearColor(0.0f, 0.0f, 0.f, 0.0f);
    
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader" );
    
    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    
    glm::mat4 Projection;
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : -0.1 unit <-> -100 units
    Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    // Or, for an ortho camera :
    //Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,-1.0f,100.0f); // In world coordinates
    
    // Camera matrix
    glm::mat4 View = glm::lookAt(
                                 glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
                                 glm::vec3(0,0,0), // and looks at the origin
                                 glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                                 );
    View = glm::mat4(1.0); // identity
    
    
    
    
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f, // left bottom
        1.0f, -1.0f, 0.0f,  // right bottom
        1.0f,  1.0f, 0.0f,  // right top
        -1.0f, 1.0f, 0.0f   // left top
    };
    static const GLushort g_element_buffer_data[] = { 0, 1, 2 };
    
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    
    std::string textureFileName = "road512.png"; //"sunflower-transp.png"; //"grass512.png";
    glActiveTexture (GL_TEXTURE0);
    GLuint texture;
    
//    texture = loadBMP_custom("uvtemplate.bmp");
    int textureImageHeight, textureImageWidth;
    texture = png_texture_load (textureFileName.c_str(), &textureImageWidth, &textureImageHeight);

    if (texture==0) {
        std::cerr << "Texture loading error: " << textureFileName << std::endl;
    }
    std::cerr << "Texture file loaded: " << textureFileName
              << " " << textureImageWidth << "x" << textureImageHeight << std::endl;
    
    GLuint textureSampler= glGetUniformLocation(programID, "myTextureSampler");

    
    static const GLfloat g_uv_buffer_data[] = {
        0,0,   // left bottom of the image
        1,0,   // right bottom
        1.,1, // top-middle
        0, 1
    };
    
    GLuint uvBuffer;
    glGenBuffers (1, &uvBuffer);
    glBindBuffer (GL_ARRAY_BUFFER, uvBuffer);
    glBufferData (GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

    bool doAlphaBlending = false;

    printf("now run ...\n");

// alpha-blending and depth testing cannot be done simply. You need some sort of buffer manipulation.
// Here we just show you that alpha-blending is possible, without applying depth test

    if (doAlphaBlending) {
    // alpha channel blending
    glEnable (GL_BLEND);
    glEnable (GL_TEXTURE_2D);
    glEnable (GL_ALPHA_TEST);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else {
    glEnable (GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    }



    do{
        
        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Use our shader
        glUseProgram(programID);
        
        // 2. texture image
        glActiveTexture(GL_TEXTURE0);
        glBindTexture (GL_TEXTURE_2D, texture);
        glUniform1i(textureSampler, 0);
        
        // 3. texture coordinates
        glEnableVertexAttribArray(1);
        glBindBuffer (GL_ARRAY_BUFFER, uvBuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        

        // Model matrix : an identity matrix (model will be at the origin)
        float tz = -5 + cos(glfwGetTime()*2*M_PI);
        glm::mat4 Tz = glm::translate(glm::vec3(.0f, 0.0f , tz));
        float tx = 2*cos(glfwGetTime()*2*M_PI/4);
        glm::mat4 Tx = glm::translate(glm::vec3(tx, 0.0f , 0));
        
        float ty = 2*cos(glfwGetTime()*2*M_PI/7);
        glm::mat4 Ty = glm::translate(glm::vec3(0.f, ty , 0));
        
        glm::mat4 T = Tx*Tz;
        
        float angle = glfwGetTime() * 2 * M_PI/67.;
        glm::mat4 R = glm::rotate (angle,glm::vec3(0.f,1.f,0.f));
        
        glm::mat4 Model = T * R;

        // draw the planar object
        {
        // Our ModelViewProjection : multiplication of our 3 matrices
        glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
                                                   // Send our transformation to the currently bound shader,
                                                   // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(MVP));
        
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                              0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                              3,                  // size
                              GL_FLOAT,           // type
                              GL_FALSE,           // normalized?
                              0,                  // stride
                              (void*)0            // array buffer offset
                              );
        
        // Draw the triangle !
        //glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // 3 indices starting at 0 -> 1 triangle
        }
        
        // draw the planar object, ANOTHER
        if (10) {
        // Our ModelViewProjection : multiplication of our 3 matrices
        glm::mat4 MVP = Projection * View * Ty*Tz*glm::translate(glm::vec3(.0f, 0.0f , -2.));
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(MVP));
        
        // Draw the triangle !
        //glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // 3 indices starting at 0 -> 1 triangle
        }
        
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );
    
    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    
    return 0;
}

