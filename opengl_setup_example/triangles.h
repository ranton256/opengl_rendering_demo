//
//  triangles.h
//  geomodel
//
//  Created by Richard Anton on 10/31/21.
//

#ifndef triangles_hpp
#define triangles_hpp

#include <vector>
#include "vector3.h"

struct Triangle {
    // array of 3 vertex indices.
    int vertex[3];
    
    Triangle() {}

    Triangle(const Triangle& t) {
        vertex[0] = t.vertex[0];
        vertex[1] = t.vertex[1];
        vertex[2] = t.vertex[2];
    }

    Triangle(int v1, int v2, int v3) {
        vertex[0] = v1;
        vertex[1] = v2;
        vertex[2] = v3;
    }
    
    bool operator == (const Triangle& t) const {
        return (
                vertex[0] == t.vertex[0] &&
                vertex[1] == t.vertex[1] &&
                vertex[2] == t.vertex[2]);
    }

    bool operator != (const Triangle& t) const {
        return (
                vertex[0] != t.vertex[0] ||
                vertex[1] != t.vertex[1] ||
                vertex[2] != t.vertex[2]);
    }


};


// Compute normal for this triangle, assumes right-handed system.
Vector3 TriangleNormal(Vector3 v0, Vector3 v1, Vector3 v2);

// Compute centroid for triangle.
Vector3 TriangleCentroid(Vector3 v0, Vector3 v1, Vector3 v2);

// Generate triangle vertex index set from control points.
void GenerateTriangleIndexes(int uNumPoints, int vNumPoints,
                             std::vector<Triangle>& outTriangleList,
                             bool wrapEndsOfRow = false);

// Add beginning and ending triangle fans to a triangle mesh given the center point of each and existing triangle list.
// This updates the contents of ioTriangleList to include the triangle fans.
// Add beginning and ending triangle fans to a triangle mesh given the center point of each and existing triangle list.
// This updates the contents of ioSurfacePoints, ioNormals, and ioTriangleList to include the triangle fans.
void AddTriangleFanEndCaps(
                           std::vector<Vector3>& ioSurfacePoints,
                           bool wantNormals,
                           std::vector<Vector3>& ioNormals,
                           std::vector<Triangle>& ioTriangleList, int n_row_points, const Vector3& beginCenterPoint, const Vector3& endCenterPoint,
                           const Vector3& beginNormal, const Vector3& endNormal);
bool TestTriangles();

#endif /* triangles_hpp */
