//
//  noise.h
//  oramaxc
//
//  Created by Richard Anton on 6/8/22.
//

#ifndef noise_hpp
#define noise_hpp

#include "vector3.h"

// This is Perlin improved noise funcion.
double ImpPerlinNoise(Vector3 p);


double FBM(Vector3 p, double gain, double lacun,
           double freqMin, double min, double max,
           int octaves, bool abs);

double Turbulence(Vector3 p, double freqMin, int octaves);

bool TestNoise();

#endif /* noise_hpp */
