#ifndef __COLOR__
#define __COLOR__

#include "mathutil.h"

#include <iostream>

// represents floating point color
class ColorF {
public:
	float r, g, b;

	ColorF() {
		r = g = b = 0.0;
	}

	ColorF(float ir, float ig, float ib) {
		r = ir;
		g = ig;
		b = ib;
	}

	// equals operator
	ColorF operator = (ColorF clr) {
		r = clr.r;
		g = clr.g;
		b = clr.b;
		return *this;
	}

	// scalar multiply
	ColorF operator * (float n) const {
		return ColorF{ r*n, g*n, b*n };
	}
	
	// scalar multiply (double) to quiet compiler warnings.
	ColorF operator * (double n) const {
		const float nf = float(n);
		return ColorF{ r*nf, g*nf, b*nf };
	}

	// color * color multiply
	ColorF operator * (ColorF clr) const {
		return ColorF{ r*clr.r, g*clr.g, b*clr.b };
	}

	// plus
	ColorF operator + (ColorF clr) const {
		return ColorF{ r + clr.r, g + clr.g, b + clr.b };
	}
    
    // minus
    ColorF operator - (ColorF clr) const {
        return ColorF{ r - clr.r, g - clr.g, b - clr.b };
    }
    
	// scalar multiply
	void operator *= (float n) {
		r *= n;
		g *= n;
		b *= n;
	}

	// plus equals
	void operator += (ColorF clr)
	{
		r += clr.r;
		g += clr.g;
		b += clr.b;
	}

	void TrimToRange() {
		r = TClip(r, 0.0f, 1.0f);
		g = TClip(g, 0.0f, 1.0f);
		b = TClip(b, 0.0f, 1.0f);
	}

};

inline std::ostream& operator<< (std::ostream& os, const ColorF& c) {
	os << "(";
	os << c.r << ", ";
	os << c.g << ", ";
	os << c.b;
	os << ")";

	return os;
}

inline float MaxAbsColorCompDelta(ColorF a, ColorF b)
{
    ColorF d = a - b;
    float compDiff = TMax3<float>(TAbs(d.r), TAbs(d.g), TAbs(d.b));
    return compDiff;
}


inline ColorF From24Color(uint8_t r, uint8_t g, uint8_t b)
{
    return ColorF((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);
}


inline void To24Color(ColorF clr, uint8_t& r, uint8_t& g, uint8_t& b)
{
    // return ColorF((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);
    // TODO: clamp
    r = clr.r * 255.0;
    g = clr.g * 255.0;
    b = clr.b * 255.0;
}


#endif // __COLOR__
