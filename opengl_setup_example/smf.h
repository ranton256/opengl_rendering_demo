#ifndef __SMF__
#define __SMF__

#include "vector3.h"
#include "triangles.h"

// Code for reading simple model format files.


bool ReadSMF(std::istream &is, std::vector<Vector3>& out_verts, std::vector<Triangle>& out_triangles);

bool TestSMF();

#endif

