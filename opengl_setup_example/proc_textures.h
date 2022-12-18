//
//  proc_textures.hpp
//  opengl_setup_example
//
//  Created by Richard Anton on 12/18/22.
//

#ifndef proc_textures_hpp
#define proc_textures_hpp

#include "image_buffer.h"
#include "colors.h"


RGBImageBuffer* GenerateCheckers(u_int32_t textureSize, u_int32_t checkerSize, RGBColor colorA, RGBColor colorB);

bool TestGenerateCheckers(void);

#endif /* proc_textures_hpp */
