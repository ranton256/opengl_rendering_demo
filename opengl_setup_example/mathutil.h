//
//  mathutil.h
//
//  Created by Richard Anton on 9/21/21.
//

#ifndef mathutil_hpp
#define mathutil_hpp

#include <cmath>



const double PI = 3.14159265358979323846;
const double TwoPI = 2.0*PI;

// value representing things too small to consider.
const double EPSILON = 0.0000001;

inline double DegreesToRadians(double degrees) {
    return degrees * PI / 180.0;
}

inline bool FCompare( double a, double b )
{
	double d = a-b;
	if( d < 0.0 )
		d = -d;
	return d <= 0.0001; // tolerance.
}

template<typename T> T TAbs(T a)
{
	return a < 0 ? -a : a;
}

template<typename T> T TMin(T a, T b)
{
    return a < b ? a : b;
}

template<typename T> T TMax(T a, T b)
{
    return a > b ? a : b;
}

template<typename T> T TClip(T val, T min, T max)
{
	if (val < min) return min;
	if (val > max) return max;
	return val;
}


template<typename T> T TMin3(T a, T b, T c)
{
	return TMin<T>(a, TMin<T>(b, c));
}

template<typename T> T TMax3(T a, T b, T c)
{
    return TMax<T>(a, TMax<T>(b, c));
}


inline double LinearInterp(double a, double b, double u)
{
    return a + u * (b - a);
}

inline float RandFloat() { return static_cast <float> (rand()) / static_cast <float> (RAND_MAX); }

#endif /* mathutil_hpp */

