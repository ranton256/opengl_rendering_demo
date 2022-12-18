//
//  img_loader.cpp
//  opengl_setup_example
//
//  Created by Richard Anton on 12/18/22.
//

#include "image_buffer.h"

#include <cassert>


RGBImageBuffer::RGBImageBuffer()
    : mPixels(nullptr), mWidth(0), mHeight(0), mRowBytes(0), mChannels(0)
{
    
}

RGBImageBuffer::RGBImageBuffer(u_int32_t width, u_int32_t height, u_int16_t channels)
    : mPixels(nullptr), mWidth(0), mHeight(0), mRowBytes(0), mChannels(0)
{
    SetSize(width, height);
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
void RGBImageBuffer::SetSize(u_int32_t width, u_int32_t height, u_int16_t channels)
{
    
    FreePixels();
    if(width > 0 && height > 0) {
        mHeight = height;
        mWidth = width;
        mChannels = channels;
        
        auto pixBytes = width * channels;
        // round up to 4.
        mRowBytes = ((pixBytes + 3) >> 2) << 2;
        assert(mRowBytes >= pixBytes);
        assert(mRowBytes % 4 == 0);
        
        mPixels = new uint8_t[mRowBytes * height];
        assert(mPixels != nullptr); // this means didn't get expected exception on failure.

    }
}

