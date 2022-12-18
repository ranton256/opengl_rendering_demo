//
//  proc_textures.cpp
//  opengl_setup_example
//
//  Created by Richard Anton on 12/18/22.
//

#include "proc_textures.h"
#include <cassert>
#include <iostream>

using namespace std;

RGBImageBuffer* GenerateCheckers(u_int32_t textureSize, u_int32_t checkerSize, RGBColor colorA, RGBColor colorB)
{
    auto buffer = new RGBImageBuffer(textureSize, textureSize);
    
    uint8_t* pixels = buffer->Pixels();
    uint8_t* rowPtr = pixels;
    
    for(u_int32_t y = 0; y < textureSize; y++) {
        auto yChk = y % (checkerSize<<1);
        uint8_t* pp = rowPtr;
        for(u_int32_t x = 0; x < textureSize; x++) {
            auto xChk = x % (checkerSize<<1);
            if(xChk == yChk) {
                // color 1
                *pp++ = colorA.r;
                *pp++ = colorA.g;
                *pp++ = colorA.b;
            } else {
                // color 2;
                *pp++ = colorB.r;
                *pp++ = colorB.g;
                *pp++ = colorB.b;
            }
        }
        rowPtr += buffer->RowBytes();
    }
    return buffer;
}

bool TestGenerateCheckers(void)
{
    RGBColor colorA = {10, 20, 30};
    RGBColor colorB = {5, 10, 15};
    const u_int32_t textureSize = 2;
    const u_int32_t checkerSize = 1;
    
    RGBImageBuffer* buffer = GenerateCheckers(textureSize, checkerSize, colorA,  colorB);
    assert(buffer != nullptr);
    
    if(!buffer)
        return false;
    
    bool good = true;
    assert(buffer->Width() == textureSize);
    assert(buffer->Height() == textureSize);
    assert(buffer->Channels() == 3);
    assert(buffer->RowBytes() > 3 * textureSize);
    
    
    
    const u_int8_t expectedRow1[] = {10, 20, 30, 5, 10, 15};
    const u_int8_t expectedRow2[] = {5, 10, 15, 10, 20, 30};
    
    for(int y = 0; y < textureSize; y++) {
        const bool firstRow = (y  / checkerSize) % 2 == 0;
        
        const auto expected = firstRow ? expectedRow1 : expectedRow2;
        const auto row = buffer->Pixels() + y * buffer->RowBytes();
        
        for(int i=0; i < textureSize * 3; i++) {
            if(expected[i] != row[i]) {
                good = false;
                cerr << "GenerateCheckers row mismatch at y=" << y << " i = " << i << endl;
            }
        }
    }
    
    
    delete buffer;
        
    return good;
}
