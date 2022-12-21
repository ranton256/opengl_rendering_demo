#include "shadinginfo.h"

ShadingInfo::ShadingInfo() 
: mHasHit(false) 
{

}
// updates hit info with latest hit.
void ShadingInfo::AddHit(const HitInfo& hp)
{
	mLatestHit = hp;
	if (!mHasHit || mClosestHit.t > hp.t) {
		mClosestHit = hp;
	}
	mHasHit = true;
}
