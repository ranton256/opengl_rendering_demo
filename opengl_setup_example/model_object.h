//
//  model_object.h
//  opengl_setup_example
//
//  Created by Richard Anton on 12/21/22.
//

#ifndef model_object_hpp
#define model_object_hpp

#include <vector>
#include <stack>

// Define before OpenGL and GLUT includes to avoid deprecation messages
#define GL_SILENCE_DEPRECATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "trianglemesh.h"


struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float specExp;
    
    // TODO: move texture ID here.
    
    Material() {
        ambient  = {0.1, 0.1, 0.1};
        diffuse = {0.1, 0.1, 0.8};
        specular = {1, 1, 1};
        specExp = 10.0;
    }
    
    Material(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float spExp) {
        ambient = amb;
        diffuse = diff;
        specular = spec;
        specExp = spExp;
    }
};

struct FrameState;

class ModelObject {
    
public:
    std::vector<float> vertexData;
    std::vector<float> texCoords;
    std::vector<int> vertexIndexes;
    std::vector<float> normals;
    std::stack<glm::mat4> modelMatrixStack;
    bool isIndexed;
    bool isVisible;
    GLuint vertexBuffer, uvBuffer, indexBuffer, normalBuffer;
    GLuint textureID;
    Material material;
    
    ModelObject() {
        isIndexed = false;
        textureID = -1;
        vertexBuffer = uvBuffer  = indexBuffer = normalBuffer = -1;
        modelMatrixStack.push(glm::identity<glm::mat4>());
    };
    
    ModelObject(const ModelObject& obj) = default;
    ~ModelObject() = default;
    
    void PushModelMatrix(const glm::mat4 mat) { modelMatrixStack.push(mat); }
    
    void PopModelMatrix(void) { modelMatrixStack.pop(); }
    
    const bool Visible(void) const { return isVisible; }
    
    void Hide(void) { isVisible = false; }
    void Show(void) { isVisible = true; }
    
    glm::mat4& ModelMatrix(void) { return modelMatrixStack.top(); }
};


void DrawObject( FrameState* frameState, ModelObject& obj, glm::mat4 viewMat, GLuint mMatUniformLocation, GLuint normMatUniformLocation);

void BindObjectBuffers(ModelObject &obj);

void MakeTriangle(ModelObject& triObj);

void MakeMeshObject(TriangleMesh& mesh, ModelObject& obj);

void GenerateNormals(const std::vector<float>& vertices, std::vector<float>& normals);

#endif /* model_object_hpp */
