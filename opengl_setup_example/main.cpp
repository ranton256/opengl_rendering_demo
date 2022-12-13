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

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "perspective.h"

#include "shaders.h"

using namespace gl;
using namespace glbinding;


const GLchar* vertexShaderSource = "#version 330 core\n"
"layout ( location = 0 ) in vec4 a_position;\n"
"void main( )\n"
"{\n"
"gl_Position = a_position;\n"
"}\n"
;

/* TODO: example for projection.
attribute vec4      a_position;
attribute vec4      a_color;

varying vec4        v_color;

uniform mat4 u_proj_matrix;
uniform mat4 u_model_matrix;

void main() {
  mat4 mvp_matrix = u_proj_matrix * u_model_matrix;
  v_color = a_color;
  gl_Position = mvp_matrix * a_position;
}
*/


const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec3 color;\n"
"void main(){\n"
"  color = vec3(1,0,0);\n"
"}\n"
;

//
//const char *fragmentShaderSource =
//    //"#version 100\n"  // OpenGL ES 2.0
//    "#version 120\n"  // OpenGL 2.1
//    "void main(void) {        "
//    "  gl_FragColor[0] = 0.0; "
//    "  gl_FragColor[1] = 0.0; "
//    "  gl_FragColor[2] = 1.0; "
//    "}";
//

/* vertex shader for perspctive matrix.
attribute vec4      a_position;
attribute vec4      a_color;

varying vec4        v_color;

uniform mat4 u_proj_matrix;
uniform mat4 u_model_matrix;

void main() {
  mat4 mvp_matrix = u_proj_matrix * u_model_matrix;
  v_color = a_color;
  gl_Position = mvp_matrix * a_position;
}
 */


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
    glbinding::Binding::initialize();
    
    glViewport((GLint)0, (GLint)0, (GLsizei)screenWidth, (GLsizei)screenHeight);
    
    // glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
    glClearColor((GLfloat)0.5f, (GLfloat)0.05f, (GLfloat)0.5f, (GLfloat)1.0f);
    
    // TODO: not sure we want these.
//    glMatrixMode( GL_PROJECTION );
//    glLoadIdentity( );

    const float fovyInDegrees = 60.0f;
    const float aspectRatio = 16.0f / 10.0f; // 4.0/3.0;
    const float znear = 0.001f;
    const float zfar = 1000.0;

    glm::mat4 projection = glm::perspective(
      // FOV & aspect
      fovyInDegrees, aspectRatio, znear, zfar);

    
    
//    // If you're using the now deprecated matrix stacks
//    glMatrixMode(GL_PROJECTION);
//    glLoadMatrixf(glm::value_ptr(projection));

    // TODO:
//    // if you're using the new shader based pipelines
//    GLint projectionUniformLocation = ...;
//    glUniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE,
//      glm::value_ptr(projection));
    
    GLuint program;
    if(!InitShader(program, vertexShaderSource, fragmentShaderSource)) {
        std::cerr << "Could not initialize shaders" << std::endl;
    }
    
    while( !glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    std::cout << "GL setup exiting cleanly\n";
    return 0;
}

