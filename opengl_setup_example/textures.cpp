//
//  textures.cpp
//  opengl_setup_example
//
//  Created by Richard Anton on 12/21/22.
//

#include "textures.h"


GLuint CreateTextureFromImage(RGBImageBuffer* imageBuffer)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB,
                 imageBuffer->Width(), imageBuffer->Height(),
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE, imageBuffer->Pixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    return textureID;
}

void AutoMapUV(TriangleMesh &mesh, std::vector<float>& texCoords)
{
    double minX = mesh.GetVertices().front().x;
    double minY = mesh.GetVertices().front().y;
    double minZ = mesh.GetVertices().front().z;
    double maxX = minX;
    double maxY = minY;
    double maxZ = minZ;
    
    // find min/max of model in each direction.
    for(auto vtx : mesh.GetVertices()) {
        if(vtx.x < minX)
            minX = vtx.x;
        if(vtx.y < minY)
            minY = vtx.y;
        if(vtx.z < minZ)
            minZ = vtx.z;
        
        if(vtx.x > maxX)
            maxX = vtx.x;
        if(vtx.y > maxY)
            maxY = vtx.y;
        if(vtx.z > maxZ)
            maxZ = vtx.z;
    }
    
    constexpr double kMinRange = 0.00001;
    double rangeX = TMax(maxX - minX, kMinRange);
    double rangeY = TMax(maxY - minY, kMinRange);
    double rangeZ = TMax(maxZ - minZ, kMinRange);
    
    int uIdx, vIdx;
    // ignore the one with the smallest range.
    if(rangeX <= rangeY && rangeX <= rangeZ) {
        // discard X
        uIdx = 1;
        vIdx = 2;
    } else if(rangeY <= rangeX && rangeY <= rangeZ) {
        // discard Y
        uIdx = 0;
        vIdx = 2;
    } else {
        // discard Z
        uIdx = 0;
        vIdx = 1;
    }
    
    for(auto vtx : mesh.GetVertices()) {
        float uvw[3];
        uvw[0] = (vtx.x - minX) / rangeX;
        uvw[1] = (vtx.y - minY) / rangeY;
        uvw[2] = (vtx.z - minZ) / rangeZ;
        
        float u = uvw[uIdx];
        float v = uvw[vIdx];
        
        texCoords.push_back(u);
        texCoords.push_back(v);
    }
}
