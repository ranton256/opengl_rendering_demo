//
//  noise.cpp
//  oramaxc
//
//  Created by Richard Anton on 6/8/22.
//

#include <cstdlib>
#include <ctime>
#include <cstdio>

#include "dbgutils.h"
#include "mathutil.h"

using namespace std;

#include "noise.h"

static bool sNoiseInitialized = false;
static int sPerm[256];


inline double ImpSplineInterp(double u)
{
    // 6t^5 - 15t^4 + 10t^3 from Perlin 2002
    double u2 = u*u;
    double u3 = u2*u;
    double u4 = u2*u2;
    double u5 = u2*u3;
    return 6 * u5 - 15 * u4 + 10 * u3;
}

inline int Perm(int x)
{
    return sPerm[ x & 0xff ];
}

static const Vector3 sGradValues[] = {
    { 1, 1, 0},
    {-1, 1, 0},
    { 1,-1, 0},
    {-1,-1, 0},
    
    { 1, 0, 1},
    {-1, 0, 1},
    { 1, 0, -1},
    {-1, 0, -1},
    
    { 0, 1, 1},
    { 0,-1, 1},
    { 0, 1, -1},
    { 0,-1, -1},
    
    { 1, 1, 0},
    { 0,-1, 1},
    { -1, 1, 0},
    { 0,-1, -1}
};

inline double Gradient(int hashVal, Vector3 frac)
{
    int lo4 = hashVal & 0xF;
    
    return DotProduct(sGradValues[lo4], frac);
}

void InitNoise()
{
    srand ( 11587  ); // larger prime numbers.
    
    for(int i=0; i < 256; i++) {
        sPerm[i] = i;
    }
    
    // Randomly shuffled array, aka permutation
    for(int i=0; i < 256; i++) {
        int selected = rand() % (256 - i);
        int temp = sPerm[i];
        sPerm[i] = sPerm[selected];
        sPerm[selected] = temp;
    }
    sNoiseInitialized = true;
}

// This is Perlin improved noise funcion.
double ImpPerlinNoise(Vector3 p)
{
    if(!sNoiseInitialized) {
        InitNoise();
    }
    
    // whole and fractional parts of p.
    Vector3 pf = Vector3(floor(p.x), floor(p.y), floor(p.z));
    
    int  xInt, yInt, zInt;
    xInt = TAbs((int)pf.x) & 0xff;
    yInt = TAbs((int)pf.y) & 0xff;
    zInt = TAbs((int)pf.z) & 0xff;

    Vector3 frac = p - pf;
    
    Vector3 si = {
        ImpSplineInterp(frac.x),
        ImpSplineInterp(frac.y),
        ImpSplineInterp(frac.z)
    };
    
    // Compute hash values for all the corners of the lattice cube.
    
    int h1[2], h2[4], h3[8];
    h1[0] = Perm(xInt)   + yInt;
    h1[1] = Perm(xInt+1) + yInt;
    
    h2[0] = Perm(h1[0]    ) + zInt;
    h2[1] = Perm(h1[0] + 1) + zInt;
    
    h2[2] = Perm(h1[1]    ) + zInt;
    h2[3] = Perm(h1[1] + 1) + zInt;
    
    Vector3 offset[8];
    
    for(int i=0; i<8; i++) {
        int h2Idx = i % 4;
        int inc = i / 4;
        h3[i] = Perm(h2[h2Idx + inc]);
        offset[i].x = (i&1) == 0 ? 0 : -1;
        offset[i].y = ((i>>1)&1) == 0 ? 0 : -1;
        offset[i].z = ((i>>2)&1) == 0 ? 0 : -1;
    }
    
    double temp1 = LinearInterp(Gradient(h3[0], frac + offset[0]), Gradient(h3[1], frac + offset[1]), si.x);
    double temp2 = LinearInterp(Gradient(h3[2], frac + offset[2]), Gradient(h3[3], frac + offset[3]), si.x);
    double temp3 = LinearInterp(Gradient(h3[4], frac + offset[4]), Gradient(h3[5], frac + offset[5]), si.x);
    double temp4 = LinearInterp(Gradient(h3[6], frac + offset[6]), Gradient(h3[7], frac + offset[7]), si.x);
    
    double tempA = LinearInterp(temp1, temp2, si.y);
    double tempB = LinearInterp(temp3, temp4, si.y);
    
    double result = LinearInterp(tempA, tempB, si.z);
    
    return result;
}


double FBM(Vector3 p, double gain, double lacun,
           double freqMin, double min, double max,
           int octaves, bool abs)
{
    double amp, freq, accum;
    amp = 1;
    freq = freqMin;
    accum = 0;
    for(int i=0; i < octaves; i++) {
        double n = ImpPerlinNoise(p * freq);
        if(abs && n < 0.0)
            n = -n;
        accum += amp * n;
        freq *= lacun;
        amp *= gain;
    }
    
    // double fbm = (TClip(accum,mMin,mMax) - mMin) / (mMax - mMin);
    double clipped = TClip(accum, min, max);
    double fbm = (clipped - min) / (max - min);
 
    return fbm;
}

double Turbulence(Vector3 p, double freqMin, int octaves)
{
    return FBM(p, 0.5, 2.0, 1, 0, 1, 4, true);
}


bool TestNoise()
{
    InitNoise();
    
    double li = LinearInterp(2, 3, 0.3);
    DbgAssertAlmostEqual(li, 2.3);
    
    double t = 0.4;
    double siExpected = 0.31744;
    
    double siActual = ImpSplineInterp(t);
    DbgAssertAlmostEqual(siExpected, siActual);
    
    bool seen[256];
    for(int i=0; i<256; i++) seen[i] = false;
    
    for(int i=0; i<256; i++) {
        int p = Perm(i);
        DbgAssert( !seen[p]);
        seen[p] = true;
    }
    
    const int kExpectedGradients[] = {
        2, 0, 0, -2,
        2, 0,0, -2,
        2, 0, 0, -2,
        2, 0, 0, -2
    };
    // smoke test
    for(int i=0; i < 16; i++) {
        int hval = i | (rand()&0xf << 4);
        Vector3 frac = {1,1,1};
        double g = Gradient(hval, frac);
        if(!DbgAssertAlmostEqual(g, kExpectedGradients[i])) {
            cout << "incorrect gradient i=" << i << " g=" << g << endl;
        }
    }
    

    Vector3 p;
    double n;
    constexpr double kNoiseMin = -1.0, kNoiseMax = 1.0;
    
    struct {
        Vector3 p;
        double expected;
        bool final;
    } kNoiseTestCases[] =
    {
        { {1, 2, 3}, 0.0, false },
        { {0.1, 0.2, 0.3}, -0.0634159, false },
        { {100.1, 50.3, 30.6}, 0.252869, false },
        { {2, -1, -1}, 0.0, false },
        { {2.22876, -1.60205, -1.42402}, 0.259875, false },
        { {-7.5, 3.2, 4.3}, 0.142334, false },
        { {0,0,0}, 0, true },
    };
    
    for(int i=0; kNoiseTestCases[i].final == false; i++) {
        n = ImpPerlinNoise(kNoiseTestCases[i].p);
        DbgAssert(n > kNoiseMin && n < kNoiseMax);
        if(!DbgAssertAlmostEqual(n, kNoiseTestCases[i].expected)) {
            cout << "p=" << kNoiseTestCases[i].p << " n=" << n << " expected=" << kNoiseTestCases[i].expected << endl;
        }
    }
    
    
    // print histogram of noise results for inspection.
    
    constexpr int kHistSize = 100;
    int hist[kHistSize];
    for(int x=0; x < kHistSize; x++) hist[x] = 0;
    bool first = true;
    double min = 0, max = 0;
    for(int i = 0; i < 10000; i++) {
        p = {RandFloat(), RandFloat(), RandFloat()};
        n = ImpPerlinNoise(p);
        int histIdx = (int)(n * kHistSize);
        // cout << "p=" << p << " n=" << n << " histIdx=" << histIdx << endl;
        
        if(first) {
            first = false;
            min = n;
            max = n;
        } else {
            if(n < min) min = n;
            if(n > max) max = n;
        }
        hist[ TClip(histIdx, 0, kHistSize-1) ] ++;
    }
    
    int histMax = *std::max_element(hist, hist+kHistSize);
    int histMin = *std::min_element(hist, hist+kHistSize);

    DbgAssert(min > kNoiseMin && max < kNoiseMax);
    
    if(/* DISABLES CODE */ (0)) { //  debug printing
        for(int x=0; x < kHistSize; x++) {
            cout << "hist[" << x << "]=" << hist[x] << endl;
        }
        
        cout << "histMin=" << histMin << endl;
        cout << "histMax=" << histMax << endl;
        cout << "max = " << max << " min = " << min << endl;
        
    }

    
    if (DbgHasAssertFailed()) {
        return false;
    }

    return true;
}
