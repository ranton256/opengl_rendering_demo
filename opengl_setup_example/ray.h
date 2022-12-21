#ifndef __RAY__
#define __RAY__

#include "vector3.h"
class Ray {
public:
	Vector3 o;
	Vector3 d;

	Vector3 PointAt(double t) const {
		return o + d * t;
	}
};

#endif
