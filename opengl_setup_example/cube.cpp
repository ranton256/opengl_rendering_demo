//
//  cube.cpp
//  opengl_setup_example
//
//  Created by Richard Anton on 12/19/22.
//

#include "cube.h"


#include "vector3.h"

// TODO: generate this programmitcally
static const float kCubeVertexData[] = {
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

static const float kCubeUVData[] = {
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



void GenerateCube(std::vector<float>& vertices,  std::vector<float>& texCoords)
{
    constexpr int vertexDataCount = sizeof(kCubeVertexData) / sizeof(kCubeVertexData[0]);
    
    vertices.clear();
    vertices.assign(kCubeVertexData, kCubeVertexData + vertexDataCount);
    
    constexpr int uvDataCount = sizeof(kCubeUVData) / sizeof(kCubeUVData[0]);
    texCoords.clear();
    texCoords.assign(kCubeUVData, kCubeUVData + uvDataCount);
}

// NOTE: this is duplicated from Orama triangles.cpp
static Vector3 TriangleNormal(Vector3 v0, Vector3 v1, Vector3 v2)
{
    Vector3 d1 = v1 - v0;
    Vector3 d2 = v2 - v0;
    Vector3 normal = CrossProduct(d1, d2);
    normal.Normalize();
    return normal;
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
