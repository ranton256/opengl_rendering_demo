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


RGBImageBuffer* GenerateMarble(u_int32_t textureSize, RGBColor colorA, RGBColor colorB);


RGBImageBuffer* GenerateFractalBrownianMotion(u_int32_t textureSize, RGBColor color,
                                              double gain, double lacun, double freqMin, double min, double max, int octaves, bool abs);

#endif /* proc_textures_hpp */
