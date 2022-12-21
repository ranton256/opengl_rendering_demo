#ifndef __BBOX__
#define __BBOX__

#include "ray.h"

#pragma once
class BBox
{
private:
	Vector3 mMin, mMax;

public:
	// double xMin, xMax, yMin, yMax, zMin, zMax;
	
	BBox();
	BBox(const Vector3& min, const Vector3& max);
	BBox(const BBox& other);
	~BBox();

	const Vector3& Min() const { return mMin; }
	const Vector3& Max() const { return mMax; }

	// require set both together so can check max > min
	void Set(const Vector3& min, const Vector3& max);

	// assignment operator
	BBox operator = (const BBox& other);

	// equals/compare operator
	bool operator == (const BBox& other);


	// check for intersection with a ray
	bool Intersect(const Ray& r, double& outTNear, double& outTFar) const;
	
	// merge/union another bounding box so this one includes its volume and that of the other.
	void Union(const BBox& other);

	// unit tests.
	static bool Test(void);
};

#endif
