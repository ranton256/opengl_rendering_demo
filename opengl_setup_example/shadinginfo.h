#ifndef __SHADING_INFO__
#define __SHADING_INFO__

#include "vector3.h"
#include "dbgutils.h"

class SceneObject;

class HitInfo {
public:
	double t;
	Vector3 hitPoint;
	Vector3 normal;
	const SceneObject* objectPtr;
    int objectIdx; //  index into object for mesh or -1
    
    double u, v;
	// TODO: add local hit point if/when we add transforms.

	HitInfo() = default;
	~HitInfo() = default;

};

class ShadingInfo {
private:
	bool mHasHit;
	HitInfo mClosestHit, mLatestHit;
	
public:
	ShadingInfo();
	~ShadingInfo() = default;

	// updates hit info with latest hit.
	void AddHit(const HitInfo& hp);

	bool HasHit(void) const { return mHasHit;  }

	const HitInfo& ClosestHit() const {
		DbgAssert(mHasHit);
		return mClosestHit;
	}
};

#endif // __SHADING_INFO__
