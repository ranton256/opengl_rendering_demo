//
//  proc_textures.cpp
//  opengl_setup_example
//
//  Created by Richard Anton on 12/18/22.
//

#include "proc_textures.h"
#include <cassert>
#include <iostream>

#include "color.h"
#include "vector3.h"
#include "noise.h"

using namespace std;

RGBImageBuffer* GenerateCheckers(u_int32_t textureSize, u_int32_t checkerSize, RGBColor colorA, RGBColor colorB)
{
    auto buffer = new RGBImageBuffer(textureSize, textureSize);
    
    uint8_t* pixels = buffer->Pixels();
    uint8_t* rowPtr = pixels;
    
    for(u_int32_t y = 0; y < textureSize; y++) {
        auto yChk = (y / checkerSize) % 2;
        uint8_t* pp = rowPtr;
        for(u_int32_t x = 0; x < textureSize; x++) {
            auto xChk = (x / checkerSize) % 2;
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
    assert(buffer->RowBytes() >= 3 * textureSize);
    
    
    
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


class Marble {
private:
    ColorF mColorA, mColorB;
    double mFreq, mAmp;
    float mPertub;
    
    
public:
    Marble(ColorF colorA, ColorF colorB)
    : mColorA(colorA), mColorB(colorB), mFreq(4), mAmp(1.0), mPertub(0.05) {
        
    }
    
    Marble(ColorF colorA, ColorF colorB, double freq, double amp, float perturb)
    : mColorA(colorA), mColorB(colorB), mFreq(freq), mAmp(amp), mPertub(perturb) {
       
    }

    ColorF  MarbleColorAtPoint(Vector3 hp)
    {
        // Perturbing here breaks up the artifacts along lattice cube boundaries.
        Vector3 p = hp + Vector3(RandFloat() * mPertub, RandFloat() * mPertub, RandFloat() * mPertub);

        double t = Turbulence(p, 1.5, 5);
        double mval = sin(mFreq * (p.x + mAmp*t));

        double cval = TClip(mval, 0.0, 1.0);
        ColorF clr;
        clr.r = LinearInterp(mColorA.r, mColorB.r, cval);
        clr.g = LinearInterp(mColorA.g, mColorB.g, cval);
        clr.b = LinearInterp(mColorA.b, mColorB.b, cval);

        return clr;
    }
};

    
RGBImageBuffer* GenerateMarble(u_int32_t textureSize, RGBColor colorA, RGBColor colorB)
{
    ColorF fcA = From24Color(colorA.r, colorA.g, colorA.b);
    ColorF fcB = From24Color(colorB.r, colorB.g, colorB.b);
    Marble marble(fcA, fcB, 0.3, 0.25, 15);
    Marble marble2(fcA, fcB, 0.11, 0.15, 10);
    
    auto buffer = new RGBImageBuffer(textureSize, textureSize);
    
    uint8_t* pixels = buffer->Pixels();
    uint8_t* rowPtr = pixels;
    
    for(u_int32_t y = 0; y < textureSize; y++) {
        uint8_t* pp = rowPtr;
        for(u_int32_t x = 0; x < textureSize; x++) {
            float z = 0;
            ColorF fmc = marble.MarbleColorAtPoint(Vector3(x, y, z));
            ColorF fmc2 = marble2.MarbleColorAtPoint(Vector3(x, y, z));
            ColorF ftot = fmc + fmc2;
        
            RGBColor mc;
            To24Color(ftot, mc.r, mc.g, mc.b);
            *pp++ = mc.r;
            *pp++ = mc.g;
            *pp++ = mc.b;
        
        }
        rowPtr += buffer->RowBytes();
    }
    return buffer;
}




// This is a 3d procedural texture based on fractional Brownian motion.
class FractBrownianMotion {
private:
    ColorF mColor;
    double mGain;
    double mLacunarity;
    double mFreqMin;
    double mMin, mMax;
    int mOctaves;
    bool mAbs; // use absolute value in numerator (like turbulence).
    
public:
    
    FractBrownianMotion(ColorF color);
    FractBrownianMotion(ColorF color, double gain, double lacun, double freqMin, double min, double max, int octaves, bool abs);
        
    ColorF ColorAtPoint(const Vector3& hp) const;
};




FractBrownianMotion::FractBrownianMotion(ColorF color)
: mColor(color), mGain(0.5), mLacunarity(2.0), mFreqMin(2), mMin(0.2), mMax(0.9), mOctaves(5), mAbs(true)
{
}

FractBrownianMotion::FractBrownianMotion(ColorF color, double gain, double lacun,
                                         double freqMin, double min, double max,
                                         int octaves, bool abs)
: mColor(color), mGain(gain), mLacunarity(lacun), mFreqMin(freqMin), mMin(min), mMax(max), mOctaves(octaves), mAbs(abs)
{
}


ColorF FractBrownianMotion::ColorAtPoint(const Vector3& hp) const
{
    double fbm = FBM(hp, mGain, mLacunarity, mFreqMin, mMin, mMax, mOctaves, mAbs);
    ColorF clr = mColor * fbm;
    return clr;
}


RGBImageBuffer* GenerateFractalBrownianMotion(u_int32_t textureSize, RGBColor color,
                                              double gain, double lacun, double freqMin, double min, double max, int octaves, bool abs)
{
    ColorF fc = From24Color(color.r, color.g, color.b);
    
    FractBrownianMotion fbm(fc, gain, lacun, freqMin, min, max, octaves, abs);
    
    auto buffer = new RGBImageBuffer(textureSize, textureSize);

    uint8_t* pixels = buffer->Pixels();
    uint8_t* rowPtr = pixels;

    for(u_int32_t y = 0; y < textureSize; y++) {
        uint8_t* pp = rowPtr;
        for(u_int32_t x = 0; x < textureSize; x++) {
            ColorF fmc = fbm.ColorAtPoint(Vector3(x, y, 0.0));
            
            RGBColor mc;
            To24Color(fmc, mc.r, mc.g, mc.b);
            *pp++ = mc.r;
            *pp++ = mc.g;
            *pp++ = mc.b;
        
        }
        rowPtr += buffer->RowBytes();
    }
    return buffer;
}


