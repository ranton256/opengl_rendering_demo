//
//  shaders.cpp
//  opengl_setup_example
//
//  Created by Richard Anton on 12/13/22.
//

#include "shaders.h"

#include <iostream>
#include <vector>


using namespace std;
using namespace gl;

static void logShaderError(gl::GLuint vs) {
    GLint maxLength = 0;
    glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &maxLength);
    
    // The maxLength includes the NULL character
    std::vector<GLchar> errorLog(maxLength);
    glGetShaderInfoLog(vs, maxLength, &maxLength, &errorLog[0]);
    if(!errorLog.empty()) {
        cerr << "\t" << (const GLchar *)errorLog.data() << endl;
    }
}

bool InitShader(GLuint& outProgram, const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource)
{
    GLboolean compile_ok = GL_FALSE, link_ok = GL_FALSE;

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSource, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
    if (!compile_ok) {
        cerr << "Error in vertex shader" << endl;
        
        logShaderError(vs);
        glDeleteShader(vs);
    
        return false;
    }
    
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    
    glShaderSource(fs, 1, &fragmentShaderSource, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
    if (!compile_ok) {
        cerr << "Error in fragment shader" << endl;
        logShaderError(vs);
        
        glDeleteShader(vs);
        glDeleteShader(fs);
    
        return false;
    }


    outProgram = glCreateProgram();
    glAttachShader(outProgram, vs);
    glAttachShader(outProgram, fs);
    glLinkProgram(outProgram);
    glGetProgramiv(outProgram, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        cerr << "Error in glLinkProgram" << endl;
        
        logShaderError(vs);
        
        glDeleteShader(vs);
        glDeleteShader(fs);
    
        return false;
    }
    
    return true;
}

