//
//  pyramid.cpp
//  opengl_setup_example
//
//  Created by Richard Anton on 12/19/22.
//

#include "pyramid.h"


// TODO: this should have normals.
void GeneratePyramid(std::vector<float>& vertices, std::vector<float>& texCoords)
{
    static const float pyramidVertices[] = {
        -1, -1,  1,  1, -1,  1,  0,  1,  0,
         1, -1,  1,  1, -1, -1,  0,  1,  0,
         1, -1, -1, -1, -1, -1,  0,  1,  0,
        -1, -1, -1, -1, -1,  1,  0,  1,  0,
        -1, -1, -1,  1, -1,  1, -1, -1,  1,
         1, -1,  1, -1, -1, -1,  1, -1, -1
    };
    
    constexpr int count = sizeof(pyramidVertices) / sizeof(pyramidVertices[0]);
    
    vertices.clear();
    vertices.assign(pyramidVertices, pyramidVertices + count);
    
    texCoords.clear();
    assert(count/3 == 18);
    static const float pyramidFaceU[] = {
        0, 1, 0.5
    };
    static const float pyramidFaceV[] = {
        0, 0, 1
    };
    // same pair 18 times
    for(int i = 0; i < count / 3; i++) {
        const int faceIdx = i % 3;
        texCoords.push_back(pyramidFaceU[faceIdx]);
        texCoords.push_back(pyramidFaceV[faceIdx]);
    }
}
