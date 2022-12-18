//
//  img_loader.hpp
//  opengl_setup_example
//
//  Created by Richard Anton on 12/18/22.
//

#ifndef img_loader_hpp
#define img_loader_hpp

#include <cstdlib>

class RGBImageBuffer {
private:
    u_int8_t* mPixels;
    u_int32_t mWidth, mHeight;
    u_int32_t mRowBytes;
    u_int16_t mChannels;
    
protected:
    void FreePixels(void);
    
public:
    RGBImageBuffer();
    RGBImageBuffer(u_int32_t width, u_int32_t height, u_int16_t channels=3, bool padRows=false);
    
    virtual ~RGBImageBuffer();
    
    // set size and reallocate, discards current contents.
    void SetSize(u_int32_t width, u_int32_t height, u_int16_t channels=3, bool padRows=false);
    
    u_int32_t Width(void) const { return mWidth; }
    u_int32_t Height(void) const { return mHeight; }
    u_int16_t Channels(void) const { return mChannels; }
    
    u_int32_t RowBytes(void) const { return mRowBytes; }
    
    const u_int8_t* Pixels(void) const { return mPixels; }
    u_int8_t* Pixels(void) { return mPixels; }
};




#endif /* img_loader_hpp */
