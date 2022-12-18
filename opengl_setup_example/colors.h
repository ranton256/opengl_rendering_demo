//
//  colors.h
//  opengl_setup_example
//
//  Created by Richard Anton on 12/18/22.
//

#ifndef colors_h
#define colors_h

#include <iostream>


class RGBColor {
public:
    u_int8_t r,g,b;
        
    RGBColor(void) {r=g=b=0;}
    RGBColor(u_int8_t ir, u_int8_t ig, u_int8_t ib) {r = ir; g = ig; b = ib;}
    RGBColor(const RGBColor& c) {r = c.r; g = c.g; b = c.b;}
    void Set(u_int8_t ir, u_int8_t ig, u_int8_t ib) { r=ir; g=ig; b=ib; }
    
    bool operator == (const RGBColor& color) {
        return r == color.r && g == color.g && b == color.b;
    }
    
    bool operator != (const RGBColor& color) {
        return r != color.r || g != color.g || b != color.b;
    }
    
};

inline std::ostream & operator<<(std::ostream &os, const RGBColor& color)
{
  return os << "(" << (int)color.r << "," << (int)color.g << "," << (int)color.b << ")";
}



inline u_int32_t ColorToRGBA(u_int8_t r, u_int8_t g, u_int8_t b, u_int8_t a)
{
    u_int8_t pp[4];
    pp[0] = r;
    pp[1] = g;
    pp[2] = b;
    pp[3] = a;
    return *(u_int32_t *)pp;
}



#endif /* colors_h */
