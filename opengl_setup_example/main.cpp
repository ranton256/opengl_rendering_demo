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

// This has to come first before glfw3.h it seems.
#include <glbinding/gl/gl.h>

// define this to prevent the non glbinding gl.h from being included
// without this get an ambiguious glViewport() error.
#define __gl_h_

#include <GLFW/glfw3.h>

#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>
#include <glbinding/gl/functions.h>


using namespace gl;
using namespace glbinding;


const GLchar* vertexShaderSource = "#version 330 core\n"
"layout ( location = 0 ) in vec3 position;\n"
"void main( )\n"
"{\n"
"gl_Position = vec4(position.x position.y, position.z, 1.0 );"
"}\n"
;

int main(int argc, const char * argv[])
{
    const GLint  kWindowWidth = 1024;
    const GLint kWindowHeight = 768;

    std::cout << "GL setup example\n";

    if (!glfwInit())
        return 1;

    // Ask for version 3.3
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
        glfwTerminate();
        return 1;
    }
    
    glfwMakeContextCurrent(window);
    
    // Assume context creation using GLFW
    
    // glbinding::Binding::initialize(glfwGetProcAddress);
    glbinding::Binding::initialize();
    
    glViewport((GLint)0, (GLint)0, (GLsizei)screenWidth, (GLsizei)screenHeight);
    
    
    // glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
    glClearColor((GLfloat)0.5f, (GLfloat)0.05f, (GLfloat)0.5f, (GLfloat)1.0f);
    
    while( !glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    std::cout << "GL setup exiting cleanly\n";
    return 0;
}

