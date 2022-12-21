#include "bbox.h"
#include "dbgutils.h"

BBox::BBox()
{
}

BBox::BBox(const Vector3& min, const Vector3& max)
{
	Set(min, max);
}

BBox::BBox(const BBox& other)
{
	Set(other.mMin, other.mMax);
}


BBox::~BBox()
{
}

BBox BBox::operator = (const BBox& other)
{
	Set(other.mMin, other.mMax);
	return *this;
}

bool BBox::operator == (const BBox& other)
{
	return mMin == other.mMin && mMax == other.mMax;
}

void BBox::Set(const Vector3& min, const Vector3& max)
{
	DbgAssert(min.x <= max.x);
	DbgAssert(min.y <= max.y);
	DbgAssert(min.z <= max.z);
	mMin = min;
	mMax = max;
}


// check for intersection with a ray and axis-aligned bounding box.
bool BBox::Intersect(const Ray& r, double& outTNear, double& outTFar) const
{
	Vector3 d1 = mMin - r.o;
	Vector3 d2 = mMax - r.o;

	double x1t = d1.x / r.d.x;
	double x2t = d2.x / r.d.x;

	double y1t = d1.y / r.d.y;
	double y2t = d2.y / r.d.y;
	
	double z1t = d1.z / r.d.z;
	double z2t = d2.z / r.d.z;

	Vector3 min = { TMin(x1t, x2t), TMin(y1t, y2t), TMin(z1t, z2t) };
	Vector3 max = { TMax(x1t, x2t), TMax(y1t, y2t), TMax(z1t, z2t) };

	outTNear = TMax3(min.x, min.y, min.z);
	outTFar = TMin3(max.x, max.y, max.z);

    // TODO: Double check this logic. It clips some triangles from intersecting that should.
	//  return outTFar > outTNear && outTFar > 0.0;
    return outTFar >= outTNear && outTFar >= 0.0;
}

// merge/union another bounding box so this one includes its volume and that of the other.
void BBox::Union(const BBox& other)
{
	mMin = { TMin(mMin.x, other.mMin.x), TMin(mMin.y, other.mMin.y), TMin(mMin.z, other.mMin.z) };
	mMax = { TMax(mMax.x, other.mMax.x), TMax(mMax.y, other.mMax.y), TMax(mMax.z, other.mMax.z) };
}


// unit tests.
bool BBox::Test(void)
{
	Ray r;
	

	bool result;
	double tNear, tFar;

	{
		BBox xy({ 1, -1, -1 }, { 2, 1, 1 });
		// Hit straight on(perpendicular)
		r.o = { 0,0,0 };
		r.d = { 1,0,0 };
		result = xy.Intersect(r, tNear, tFar);
		DbgAssert(result);
		DbgAssertAlmostEqual(1.0, tNear);
		DbgAssertAlmostEqual(2.0, tFar);

		// Hit at an angle
		r.o = { 0,0,0 };
		r.d = { 2,1,0 };
		r.d.Normalize();
		result = xy.Intersect(r, tNear, tFar);
		DbgAssert(result);
		DbgAssertAlmostEqual(1.0 / r.d.x, tNear);
		DbgAssertAlmostEqual(2.0 / r.d.x, tFar);

		// Miss
		r.o = { 0,0,0 };
		r.d = { 1,1.1,0 };
		result = xy.Intersect(r, tNear, tFar);
		DbgAssert(!result);

		// parallel
		r.o = { 0,0,0 };
		r.d = { 0,1,0 };
		result = xy.Intersect(r, tNear, tFar);
		DbgAssert(!result);
	}
	{
		// Same for yz
		BBox yz({ -1, -1, 1 }, { 1, 1, 2 });

		// Hit straight on(perpendicular)
		r.o = { 0,0,0 };
		r.d = { 0,0,1 };
		result = yz.Intersect(r, tNear, tFar);
		DbgAssert(result);
		DbgAssertAlmostEqual(1.0, tNear);
		DbgAssertAlmostEqual(2.0, tFar);

		// Miss
		r.o = { 0,0,0 };
		r.d = { 0,1.1,1 };
		result = yz.Intersect(r, tNear, tFar);
		DbgAssert(!result);

		// parallel
		r.o = { 0,0,0 };
		r.d = { 0,1,0 };
		result = yz.Intersect(r, tNear, tFar);
		DbgAssert(!result);
	}
	{
		// same for zx
		BBox zx({ -1, 1, -1 }, { 1, 2, 1 });
		r.o = { 0,0,0 };
		r.d = { 0,1,0 };
		result = zx.Intersect(r, tNear, tFar);
		DbgAssert(result);
		DbgAssertAlmostEqual(1.0, tNear);
		DbgAssertAlmostEqual(2.0, tFar);

		// Miss
		r.o = { 0,0,0 };
		r.d = { 1,0,1.1 };
		result = zx.Intersect(r, tNear, tFar);
		DbgAssert(!result);

		// parallel
		r.o = { 0,0,0 };
		r.d = { 0,0,1 };
		result = zx.Intersect(r, tNear, tFar);
		DbgAssert(!result);
	}

	// Test Merge
	{
		BBox first({ -1, -2, -3 }, { 3, 2, 1 });
		BBox second({ -3, -2, -1 }, { 1, 2, 3 });

		auto merged = first;
		merged.Union(second);

		BBox expected({ -3, -2, -3 }, { 3, 2, 3 });
		DbgAssert(expected == merged);

	}

	if (DbgHasAssertFailed()) {
		return false;
	}
	return true;
}
