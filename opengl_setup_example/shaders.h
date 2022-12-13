//
//  shaders.hpp
//  opengl_setup_example
//
//  Created by Richard Anton on 12/13/22.
//

#ifndef shaders_hpp
#define shaders_hpp

#include <glbinding/gl/gl.h>

bool InitShader(gl::GLuint& outProgram, const gl::GLchar* vertexShaderSource, const gl::GLchar* fragmentShaderSource);

#endif /* shaders_hpp */
