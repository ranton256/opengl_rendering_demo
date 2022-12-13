//
//  main.cpp
//  opengl_setup_example
//
//  Created by Richard Anton on 12/12/22.
//

#include <iostream>

//#define GLEW_STATIC
//#include <GL/glew.h>


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



int main(int argc, const char * argv[])
{
    const GLint  kWindowWidth = 1024;
    const GLint kWindowHeight = 768;

    std::cout << "GL setup example\n";

    
//    for (const Version & v : aux::Meta::versions())
//      std::cout << v << std::endl;
//

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
    
//
//    // for newer features.
//    glewExperimental = GL_TRUE;
//
//    if( glewInit() == GLEW_OK ) {
//        std::cerr << "Failed initializing GLEW" << std::endl;
//        return 1;
//    }
    
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



// TODO: move to readme

//- install Homebrew
//- brew install glfw3
//- brew instll glew
// Create an xcode cli project, pick C++.
// build setting: search for paths, header, click +, /usr/local/include
// (or wherever they are, might be different on M1
// for Apple Silicon (M1,M2) mac native homebrew, headers  are in /opt/homebrew/include instead.
// add the libraries from /usr/local or wherever for glfw3 and glew
// Intel homebrew: /usr/local/Cellar, Apple Silicon Homebrew: /opt/homebrew/Cellar


// to do this, go to project settings, select your target, then
// click "Build  Phases" tab, and expand "Link Binary with Libraries"
// Click +

// add openGl framework (built in)

// then click + again
// Select "Add Other..." then select "Add files..." from the drop down.
// hit Command + shift + G
// type the path i.e. /usr/local/lib
// go into the glew and glfw dirctories

// add the libraries from the glfw and glew directories.
// the file will be named something like /opt/homebrew/Cellar/glew/<version>/something.dylib
// Pick the ones that do not show as an alias.


// Link the glew library statically (.a) file.
// sign the .dylib file with your developer identity.

// check how it is signed or not with this:
// codesign -d -v /opt/homebrew/opt/glfw/lib/libglfw.3.dylib

// find with this:
// security find-identity -v -p codesigning

// sign it like this:
// codesign -s "Apple Development: Your Name (blablah)" <lib_path>

// If you get an "already signed" error then add "-f" flag in front of the library path and re-run.

// Had to switch to glbinding because glew was a pain.
// now have to do signing of glbinding library...

// And then... it had "Zero Metal services found" just like the glew error.
// so glew was not the problem.
// and it's just a warning, it actually worked....

// okay, all worky worky.

