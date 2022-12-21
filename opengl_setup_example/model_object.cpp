//
//  model_object.cpp
//  opengl_setup_example
//
//  Created by Richard Anton on 12/21/22.
//

#include "model_object.h"
#include "textures.h"

void DrawObject( ModelObject& obj, glm::mat4 viewMat, GLuint mMatUniformLocation, GLuint normMatUniformLocation)
{
    glUniformMatrix4fv(mMatUniformLocation, 1, GL_FALSE, glm::value_ptr(obj.mMat));
    
    auto mv = viewMat * obj.mMat;
    glm::mat4 normMat = glm::transpose(glm::inverse(mv));
    
    glUniformMatrix4fv(normMatUniformLocation, 1, GL_FALSE, glm::value_ptr(normMat));
    
    glBindBuffer(GL_ARRAY_BUFFER, obj.vertexBuffer);
    glVertexAttribPointer(
       0,
       3,
       GL_FLOAT,
       GL_FALSE,
       0,
       (void*)0
    );

    
    if(obj.texCoords.empty()) {
        std::cerr << "Missing object texture coordinates" << std::endl;
    } else {
        glBindBuffer(GL_ARRAY_BUFFER, obj.uvBuffer);
        glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray( 1 );
    }
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, obj.textureID);
   
    if(obj.normals.size() > 0 ) {
        glEnableVertexAttribArray( 2 );
        glBindBuffer(GL_ARRAY_BUFFER, obj.normalBuffer);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    }
    
    if(obj.isIndexed) {
        if(obj.vertexIndexes.empty()) {
            std::cerr << "Missing object vertex indexes" << std::endl;
            return;
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.indexBuffer);
        
        glDrawElements(GL_TRIANGLES,                    // primitive type
                       (GLsizei)obj.vertexIndexes.size(),   // # of indices
                       GL_UNSIGNED_INT,                 // data type
                       (void*)0);                       // offset to indices

    } else {
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)obj.vertexData.size());
    }
    
}

void BindObjectBuffers(ModelObject &obj)
{
    glGenBuffers(1, &obj.vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, obj.vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, obj.vertexData.size() * sizeof(float), obj.vertexData.data(), GL_STATIC_DRAW);
    
    glGenBuffers(1, &obj.uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, obj.uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, obj.texCoords.size() * sizeof(float), obj.texCoords.data(), GL_STATIC_DRAW);
    
    glGenBuffers(1, &obj.normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, obj.normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, obj.normals.size() * sizeof(float), obj.normals.data(), GL_STATIC_DRAW);
    
    if(obj.vertexIndexes.size() > 0) {
        glGenBuffers(1, &obj.indexBuffer);
        assert(sizeof(GLuint) == sizeof(int));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.vertexIndexes.size() * sizeof(int), obj.vertexIndexes.data(), GL_STATIC_DRAW);
    }
}

void MakeTriangle(ModelObject& triObj)
{
    static const float triVertexBufferData[] = {
       -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       0.0f,  1.0f, 0.0f,
    };
    static const GLfloat triTexCoords[] = {
        0, 0,
        1, 0,
        0.5, 1
    };

    triObj.vertexData.assign(triVertexBufferData, triVertexBufferData + sizeof(triVertexBufferData)/sizeof(float));
    
    GenerateNormals(triObj.vertexData, triObj.normals);
    
    triObj.texCoords.assign(triTexCoords, triTexCoords + sizeof(triTexCoords)/sizeof(float));
}


void MakeMeshObject(TriangleMesh& mesh, ModelObject& obj)
{
    obj.vertexData.clear();
    obj.normals.clear();
    obj.vertexIndexes.clear();
    
    for(auto v : mesh.GetVertices()) {
        obj.vertexData.push_back(v.x);
        obj.vertexData.push_back(v.y);
        obj.vertexData.push_back(v.z);
    }
    
    for(auto n : mesh.GetVertexNormals()) {
        obj.normals.push_back(n.x);
        obj.normals.push_back(n.y);
        obj.normals.push_back(n.z);
    }
    
    for(auto t : mesh.GetTriangles()) {
        obj.vertexIndexes.push_back(t.vertex[0]);
        obj.vertexIndexes.push_back(t.vertex[1]);
        obj.vertexIndexes.push_back(t.vertex[2]);
    }
    obj.isIndexed = true;
    
    AutoMapUV(mesh, obj.texCoords);
}


void GenerateNormals(const std::vector<float>& vertices,
                     std::vector<float>& normals)
{
    auto vertexCount = (int)vertices.size() / 3;
    
    std::vector<Vector3> vvec;
    
    // This copy is not necessary, but makes the code simpler.
    for(auto i = 0; i < vertexCount; i++) {
        auto base = i * 3;
        vvec.push_back( { vertices[base], vertices[base+1], vertices[base+2] } );
    }
    
    auto faceCount = vertexCount / 3;
    for(auto f = 0; f < faceCount; f++) {
        auto vi = f * 3;
        Vector3 v1 = vvec[vi];
        Vector3 v2 = vvec[vi+1];
        Vector3 v3 = vvec[vi+2];
        
        Vector3 n = TriangleNormal(v1, v2, v3);
        // push back same one for all three vertices.
        for(int j=0; j < 3; j++) {
            normals.push_back(n.x);
            normals.push_back(n.y);
            normals.push_back(n.z);
        }
    }
}

