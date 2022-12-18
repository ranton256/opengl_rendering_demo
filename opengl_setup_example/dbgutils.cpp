//
//  dbgutils.cpp
//
//  Created by Richard Anton on 9/21/21.
//

#include "dbgutils.h"
#include "vector3.h"
#include "mathutil.h"

static bool sAssertFailed = false;

void DbgClearAssertFailed() {
    sAssertFailed = false;
}

bool DbgHasAssertFailed() {
    return sAssertFailed;
}

// Failed assertions are handled by this.
void _DbgAssertFailed(const char *filename, int lineno, const char *function, const char *condition)
{
    sAssertFailed = true;
    if( function )
        fprintf(stderr,"Assertion %s failed in %s() at %s:%d\n",
                condition,function,filename,lineno);
    else
        fprintf(stderr,"Assertion %s failed at %s:%d\n",
                condition,filename,lineno);
}

// Check if vectors are near enough to equal both by component and by magnitude of delta.
bool DbgAssertVectorsAlmostEqual(Vector3 expected_pt, Vector3 actual_pt, double maxDeltaMag, double maxCompDiff)
{
    Vector3 delta = expected_pt - actual_pt;
    double deltaMag = delta.Magnitude();
    double compDiff = TMax3<double>(TAbs(delta.x), TAbs(delta.y), TAbs(delta.z));
    
    // float point comparision is not very reliable.
    DbgAssert(deltaMag <= maxDeltaMag);
    DbgAssert(compDiff <= maxCompDiff);
    if(deltaMag > maxDeltaMag || compDiff > maxCompDiff) {
        return false;
    }
    return true;
}


bool DbgAssertAlmostEqual(double x, double y, double maxDiff)
{
    double diff = TAbs(x - y);
    DbgAssert(diff <= maxDiff);
    if(diff > maxDiff)
        return false;
    return true;
}


// colors almost equal
bool DbgAssertColorsAlmostEqual(ColorF expected, ColorF actual, double maxDiff)
{
//	float rd = expected.r - actual.r;
//	float gd = expected.g - actual.g;
//	float bd = expected.b - actual.b;
//
//	double compDiff = TMax3<double>(rd, gd, bd);
    ColorF diff = expected - actual;
    double compDiff = TMax3<double>(diff.r, diff.g, diff.b);
    

	DbgAssert(compDiff <= maxDiff);
	if (compDiff > maxDiff) {
		return false;
	}
	return true;

}
