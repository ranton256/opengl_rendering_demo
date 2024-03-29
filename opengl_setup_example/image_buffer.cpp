//
//  img_loader.cpp
//  opengl_setup_example
//
//  Created by Richard Anton on 12/18/22.
//

#include "image_buffer.h"
#include "pngreader.h"

#include <cassert>
#include <iostream>


RGBImageBuffer::RGBImageBuffer()
    : mPixels(nullptr), mWidth(0), mHeight(0), mRowBytes(0), mChannels(0)
{
    
}

RGBImageBuffer::RGBImageBuffer(u_int32_t width, u_int32_t height, u_int16_t channels, bool padRows)
    : mPixels(nullptr), mWidth(0), mHeight(0), mRowBytes(0), mChannels(0)
{
    SetSize(width, height, channels, padRows);
}

RGBImageBuffer::RGBImageBuffer(uint8_t* pixels, u_int32_t width, u_int32_t height,int32_t rowBytes, u_int16_t channels)
{
    mHeight = height;
    mWidth = width;
    mChannels = channels;
    mRowBytes = rowBytes;
    
    auto pixBytes = width * channels;
    assert(mRowBytes >= pixBytes);
    
    mPixels = pixels;
    assert(mPixels != nullptr); // this means didn't get expected exception on failure.
}

RGBImageBuffer::~RGBImageBuffer()
{
    FreePixels();
}

void RGBImageBuffer::FreePixels(void)
{
    if(mPixels) {
        delete[] mPixels;
        mPixels = nullptr;
        mHeight = mHeight = mRowBytes = 0;
        mChannels = 0;
    }
}

// set size and reallocate, discards current contents.
void RGBImageBuffer::SetSize(u_int32_t width, u_int32_t height, u_int16_t channels, bool padRows)
{
    
    FreePixels();
    if(width > 0 && height > 0) {
        mHeight = height;
        mWidth = width;
        mChannels = channels;
        
        auto pixBytes = width * channels;
        if(padRows) {
            // round up to 4.
            mRowBytes = ((pixBytes + 3) >> 2) << 2;
        } else {
            mRowBytes = pixBytes;
        }
        assert(mRowBytes >= pixBytes);
        assert(padRows == false || mRowBytes % 4 == 0);
        
        mPixels = new uint8_t[mRowBytes * height];
        assert(mPixels != nullptr); // this means didn't get expected exception on failure.

    }
}

RGBImageBuffer* LoadImageBufferFromPNG(const char* path)
{
    uint32_t width, height;
    uint8_t *pixels = nullptr;
    
    if(!ReadPNGImage(path, &pixels, &width, &height)) {
        std::cerr << "Error loading image from " << path << std::endl;
        return nullptr;
    }
    
    return new RGBImageBuffer(pixels, width, height, width*3, 3);
}
