//
//  textures.h
//  opengl_setup_example
//
//  Created by Richard Anton on 12/21/22.
//

#ifndef textures_hpp
#define textures_hpp

#include <GL/glew.h>

#include "image_buffer.h"
#include "trianglemesh.h"

GLuint CreateTextureFromImage(RGBImageBuffer* imageBuffer);

void AutoMapUV(TriangleMesh &mesh, std::vector<float>& texCoords);

#endif /* textures_hpp */
