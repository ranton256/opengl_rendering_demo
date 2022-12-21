//
//  model_object.h
//  opengl_setup_example
//
//  Created by Richard Anton on 12/21/22.
//

#ifndef model_object_hpp
#define model_object_hpp

#include <vector>


// Define before OpenGL and GLUT includes to avoid deprecation messages
#define GL_SILENCE_DEPRECATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "trianglemesh.h"

class ModelObject {
    
public:
    std::vector<float> vertexData;
    std::vector<float> texCoords;
    std::vector<int> vertexIndexes;
    std::vector<float> normals;
    glm::mat4 mMat;
    bool isIndexed;
    GLuint vertexBuffer, uvBuffer, indexBuffer, normalBuffer;
    GLuint textureID;
    
    ModelObject() = default;
    ModelObject(const ModelObject& obj) = default;
    ~ModelObject() = default;
};


void DrawObject( ModelObject& obj, glm::mat4 viewMat, GLuint mMatUniformLocation, GLuint normMatUniformLocation);

void BindObjectBuffers(ModelObject &obj);

void MakeTriangle(ModelObject& triObj);

void MakeMeshObject(TriangleMesh& mesh, ModelObject& obj);

void GenerateNormals(const std::vector<float>& vertices, std::vector<float>& normals);

#endif /* model_object_hpp */
