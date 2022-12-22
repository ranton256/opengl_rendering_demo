//
//  frame_state.h
//  opengl_setup_example
//
//  Created by Richard Anton on 12/22/22.
//

#ifndef frame_state_h
#define frame_state_h

// Define before OpenGL and GLUT includes to avoid deprecation messages
#define GL_SILENCE_DEPRECATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

struct FrameState {
    GLFWwindow *window;
    
    GLint aPositionLocation;
    GLint mUniformLocation;
    GLint vUniformLocation;
    GLint normMatUniformLocation;
    GLint projUniformLocation;
    GLint lightPositionID;
    
    GLint materialAmbientID;
    GLint materialDiffuseID;
    GLint materialSpecularID;
    GLint materialSpecExpID;

    GLint globalAmbientID;
    GLint lightDiffuseID;
    GLint lightSpecularID;
    
    GLuint VertexArrayID;
   
    glm::vec3 lightPosition;
    
    glm::vec3 globalAmbient;
    glm::vec3 lightDiffuse;
    glm::vec3 lightSpecular;
    
    glm::vec3 eyePosition;
    glm::mat4 viewMatrix;
    glm::mat4 projMatrix;
};



#endif /* frame_state_h */
