//
//  main.cpp
//  opengl_setup_example
//
//  Created by Richard Anton on 12/12/22.
//

#include <iostream>

// CODE_SIGN_IDENTITY gets set to '-' this refuses to launch...

// Defined before OpenGL and GLUT includes to avoid deprecation messages
#define GL_SILENCE_DEPRECATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "shaders.h"
#include "proc_textures.h"

const char* kVertexShaderPath = "vertex_shader.glsl";
const char* kFragmentShaderPath = "fragment_shader.glsl";

// TODO: generate this programmitcally
static const GLfloat cube_vertex_data[] = {
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

// TODO: generate this programmatically.
// these values are from http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/#how-to-load-texture-with-glfw

static const GLfloat gTriangleUVBufferData[] = {
    0, 0,
    1, 0,
    0.5, 1
};


static const GLfloat gCubeUVBufferData[] = {
    0.000059f, 1.0f-0.000004f,
    0.000103f, 1.0f-0.336048f,
    0.335973f, 1.0f-0.335903f,
    1.000023f, 1.0f-0.000013f,
    0.667979f, 1.0f-0.335851f,
    0.999958f, 1.0f-0.336064f,
    0.667979f, 1.0f-0.335851f,
    0.336024f, 1.0f-0.671877f,
    0.667969f, 1.0f-0.671889f,
    1.000023f, 1.0f-0.000013f,
    0.668104f, 1.0f-0.000013f,
    0.667979f, 1.0f-0.335851f,
    0.000059f, 1.0f-0.000004f,
    0.335973f, 1.0f-0.335903f,
    0.336098f, 1.0f-0.000071f,
    0.667979f, 1.0f-0.335851f,
    0.335973f, 1.0f-0.335903f,
    0.336024f, 1.0f-0.671877f,
    1.000004f, 1.0f-0.671847f,
    0.999958f, 1.0f-0.336064f,
    0.667979f, 1.0f-0.335851f,
    0.668104f, 1.0f-0.000013f,
    0.335973f, 1.0f-0.335903f,
    0.667979f, 1.0f-0.335851f,
    0.335973f, 1.0f-0.335903f,
    0.668104f, 1.0f-0.000013f,
    0.336098f, 1.0f-0.000071f,
    0.000103f, 1.0f-0.336048f,
    0.000004f, 1.0f-0.671870f,
    0.336024f, 1.0f-0.671877f,
    0.000103f, 1.0f-0.336048f,
    0.336024f, 1.0f-0.671877f,
    0.335973f, 1.0f-0.335903f,
    0.667969f, 1.0f-0.671889f,
    1.000004f, 1.0f-0.671847f,
    0.667979f, 1.0f-0.335851f
};


static bool RunTests(void)
{
    bool good = true;
    
    good = good && TestGenerateCheckers();
    
    return good;
}

static GLuint CreateTextureFromImage(RGBImageBuffer* imageBuffer)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB,
                 imageBuffer->Width(), imageBuffer->Height(),
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE, imageBuffer->Pixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    return textureID;
}

int main(int argc, const char** argv)
{
    if(!RunTests()) {
        std::cerr << "Some tests failed" << std::endl;
        return 1;
    }
    
    const GLint  kWindowWidth = 1024;
    const GLint kWindowHeight = 768;

    std::cout << "GL setup example\n";

    if (!glfwInit())
        return 1;

    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    
    // Required to work on Mac.
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    
    // Disallow resizing.
    glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE);
    
    GLFWwindow *window = glfwCreateWindow(kWindowWidth, kWindowHeight, "OpenGL Setup Example", nullptr, nullptr);
    
    // may differ than expected due to Retina display
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    
    if( window == nullptr ) {
        std::cerr << "Failed creating window with GLFW" << std::endl;
        getchar(); // wait on keypress
        glfwTerminate();
        return 1;
    }
    
    glfwMakeContextCurrent(window);
    
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    
    
    glClearColor((GLfloat)0.5f, (GLfloat)0.5f, (GLfloat)0.5f, (GLfloat)1.0f);

    GLuint program;
    if(!InitShader(program, kVertexShaderPath, kFragmentShaderPath)) {
        std::cerr << "Could not initialize shaders" << std::endl;
        return 1;
    }

    const float fovyInDegrees = 45.0f;
    const float aspectRatio = (float) screenWidth / (float)screenHeight;
    const float znear = 0.001f;
    const float zfar = 100.0f;

    glm::mat4 Projection = glm::perspective(glm::radians(fovyInDegrees), aspectRatio, znear, zfar);
    
    glm::mat4 View = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f), // position
        glm::vec3(0.0f, 0.0f, 0.0f), // target
        glm::vec3(0.0f, 1.0f, 0.0f) //  up vector
        );

    glm::mat4 Model = glm::mat4(1.0f);
    Model = glm::scale(Model, glm::vec3(0.6f, 0.5f, 0.5f));
    
    
    GLint mvpUniformLocation = glGetUniformLocation(program, "mvp_matrix");
    
//    GLint triColorLocation = glGetUniformLocation(program, "tri_color");
//    if (triColorLocation == -1) {
//        std::cerr << "Could not bind attribute frag_color" << std::endl;
//    }

    GLint aPositionLocation = glGetAttribLocation(program, "a_position");
    if (aPositionLocation == -1) {
        std::cerr << "Could not bind attribute a_position" << std::endl;
    }
    
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    static const GLfloat g_vertex_buffer_data[] = {
       -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       0.0f,  1.0f, 0.0f,
    };
    
    GLuint vao, vertexbuffer, cubeVertexBuffer;
    glGenVertexArrays(1, &vao);
    
    glGenBuffers(1, &vertexbuffer);
    glGenBuffers(1, &cubeVertexBuffer);
    
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertex_data), cube_vertex_data, GL_STATIC_DRAW);
    
    // Create texture
    const RGBColor red = {255, 0, 0};
    const RGBColor blue = {0, 0, 255};
    const RGBColor green = {0, 255, 0};
    const RGBColor white = {255, 255, 255};
    
    RGBImageBuffer* cubeTextureImage = GenerateCheckers(1024, 32, blue, green);
    assert(cubeTextureImage);
    
    GLuint cubeTextureID = CreateTextureFromImage(cubeTextureImage);
    

    // TODO: mip mapping?
    
    
    GLuint cubeUVBuffer;
    glGenBuffers(1, &cubeUVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, cubeUVBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gCubeUVBufferData), gCubeUVBufferData, GL_STATIC_DRAW);
    
    RGBImageBuffer* triTextureImage = GenerateCheckers(1024, 32, white, red);
    assert(triTextureImage);
    
    GLuint triTextureID = CreateTextureFromImage(triTextureImage);

    GLuint triangleUVBuffer;
    glGenBuffers(1, &triangleUVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, triangleUVBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gTriangleUVBufferData), gTriangleUVBufferData, GL_STATIC_DRAW);
    
    
    float triAngle = 0.0f;
    float cubeAngle = 0.0f;
    const float kTriRotSpeed = 0.02;
    const float kCubeRotSpeed = 0.03;
    
    while( !glfwWindowShouldClose(window)) {
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(program);
        
        // glUniform4f(triColorLocation, (GLfloat)0.3f, (GLfloat)0.1f, (GLfloat)0.1f, (GLfloat)1.0f);

        
       
        
        
        glEnableVertexAttribArray(aPositionLocation);
        
        if (1) { // Triangle
            
            glm::mat4 Model = glm::mat4(1.0f);
            Model = glm::scale(Model, glm::vec3(0.6f, 0.5f, 0.5f));
            Model = glm::rotate(Model, triAngle, glm::vec3(0, 0.2, 1));
            
            glm::mat4 mvp = Projection * View * Model;
            
            glUniformMatrix4fv(mvpUniformLocation, 1, GL_FALSE, glm::value_ptr(mvp));
            
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glVertexAttribPointer(
               0, // idx
               3, // size
               GL_FLOAT, // type
               GL_FALSE, // normalized
               0, // stride
               (void*)0 // pointer
            );
            
//
            glBindBuffer(GL_ARRAY_BUFFER, triangleUVBuffer);
            glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, NULL );
            glEnableVertexAttribArray( 1 );
           
            
//            glEnableVertexAttribArray(1);
//            glBindBuffer(GL_ARRAY_BUFFER, triColorBuffer);
//            glVertexAttribPointer(
//                1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
//                3,                                // size
//                GL_FLOAT,                         // type
//                GL_FALSE,                         // normalized?
//                0,                                // stride
//                (void*)0                          // array buffer offset
//            );
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, triTextureID);
            
            
            
            glDrawArrays(GL_TRIANGLES, 0, 3);
            
            triAngle += kTriRotSpeed;
        }
        if(1){ // Cube
            
            glm::mat4 Model = glm::mat4(1.0f);
            Model = glm::translate(Model, glm::vec3(-2.5, 0, -4));
            Model = glm::scale(Model, glm::vec3(0.6f, 0.5f, 0.5f));
            Model = glm::rotate(Model, cubeAngle, glm::vec3(0.5, 0.0, 0.5));
            
            glm::mat4 mvp = Projection * View * Model;
            
            glUniformMatrix4fv(mvpUniformLocation, 1, GL_FALSE, glm::value_ptr(mvp));
            
            
            glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
            glVertexAttribPointer(
               0,
               3,
               GL_FLOAT,
               GL_FALSE,
               0,
               (void*)0
            );
 
            glBindBuffer(GL_ARRAY_BUFFER, cubeUVBuffer);
            glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, NULL );
            glEnableVertexAttribArray( 1 );
           
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, cubeTextureID);
            
            
            // cube is 12 triangles, 2 each for 6 sides
            glDrawArrays(GL_TRIANGLES, 0, 12*3);
            
            cubeAngle += kCubeRotSpeed;
        }
        
        
        glDisableVertexAttribArray(0);
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        
    }
    
    if(cubeTextureImage) {
        delete cubeTextureImage;
    }
    if(triTextureImage) {
        delete triTextureImage;
    }
    
    glfwTerminate();
    std::cout << "GL setup exiting cleanly\n";
    return 0;
}

