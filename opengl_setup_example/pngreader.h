//
//  pngreader.h
//  oramaxc
//
//  Created by Richard Anton on 6/6/22.
//

#ifndef pngreader_hpp
#define pngreader_hpp

#include <string>

bool ReadPNGImage(const char* path, uint8_t** outPixels, uint32_t* outWidth, uint32_t* outHeight);

#endif /* pngreader_hpp */
