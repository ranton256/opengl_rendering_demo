//
//  dbgutils.h
//
//  Created by Richard Anton on 9/21/21.
//

#ifndef dbgutils_hpp
#define dbgutils_hpp

#include "color.h"

#include <cstdio>

// define in project/Makefile to enable debugging assertions.

#if DEBUG == 1 || defined(_DEBUG)
#define DBG_ENABLED
#endif



// enable this for printing extra debug information.
//#define VERBOSE_DEBUG 1

#ifdef DBG_ENABLED
    // gnu c supports Pretty function which is very helpful, but nothing else
    // supports an equivalent that I know of.
    #ifdef __GNUC__
        #define DbgAssert(condition) if(!(condition)) \
        _DbgAssertFailed( __FILE__, __LINE__, __PRETTY_FUNCTION__, #condition )
    #else
        #define DbgAssert(condition) if(!(condition)) \
        _DbgAssertFailed( __FILE__, __LINE__, 0, #condition )
    #endif
#else
    #define DbgAssert(condition)  // this turns off assertions on non-debug builds
#endif


// -- Failed assertions are handled by this.
void _DbgAssertFailed(const char *filename, int lineno, const char *function, const char *condition);

void DbgClearAssertFailed();

bool DbgHasAssertFailed();


constexpr auto kMaxDeltaMag = 1e-6;
constexpr auto kMaxCompDiff = 1e-6;

// Check if vectors are near enough to equal both by component and by magnitude of delta.
class Vector3;
bool DbgAssertVectorsAlmostEqual(Vector3 expected_pt, Vector3 actual_pt, double maxDeltaMag = kMaxDeltaMag, double maxCompDiff = kMaxCompDiff);

constexpr auto kDefaultMaxDoubleDiff = 0.000001;

bool DbgAssertAlmostEqual(double x, double y, double maxDiff = kDefaultMaxDoubleDiff);

// colors almost equal
bool DbgAssertColorsAlmostEqual(ColorF expected, ColorF actual, double maxDiff = kDefaultMaxDoubleDiff);

#endif /* dbgutils_h */
