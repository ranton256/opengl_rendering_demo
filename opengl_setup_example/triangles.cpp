//
//  triangles.cpp
//  geomodel
//
//  Created by Richard Anton on 10/31/21.
//

#include "triangles.h"
#include "dbgutils.h"
#include "mathutil.h"

using namespace std;


Vector3 TriangleNormal(Vector3 v0, Vector3 v1, Vector3 v2)
{
	Vector3 d1 = v1 - v0;
	Vector3 d2 = v2 - v0;
	Vector3 normal = CrossProduct(d1, d2);
	normal.Normalize();
	return normal;
}

// Compute centroid for triangle.
Vector3 TriangleCentroid(Vector3 v0, Vector3 v1, Vector3 v2)
{
    Vector3 c = v0 + v1 + v2;
    c *= 1.0/3.0;
    return c;
}


inline int VertexIndexFromIJ(int i, int j, int uNumPoints, int vNumPoints)
{
    int idx = j * uNumPoints + i;
    return idx;
}


// Generate triangle vertex index set from control points.
void GenerateTriangleIndexes(int uNumPoints, int vNumPoints,
                             std::vector<Triangle>& outTriangleList,
                             bool wrapEndsOfRow)
{
    outTriangleList.clear();
   
    const int kRowMax = wrapEndsOfRow ? uNumPoints : uNumPoints-1;
    
    for (int j = 0; j < vNumPoints-1; j++) { // z axis
        for (int i = 0; i < kRowMax; i++) {  // each row/revolution
            // Counter clockwise.
            Triangle t1, t2;
            int iNext = i + 1;
            if(iNext >= uNumPoints) {
                iNext = 0;
            }
            t1.vertex[0] = VertexIndexFromIJ(i, j, uNumPoints, vNumPoints);
            t1.vertex[1] = VertexIndexFromIJ(i, j+1, uNumPoints, vNumPoints);
            t1.vertex[2] = VertexIndexFromIJ(iNext, j, uNumPoints, vNumPoints);
            
            t2.vertex[0] = VertexIndexFromIJ(i, j+1, uNumPoints, vNumPoints);
            t2.vertex[1] = VertexIndexFromIJ(iNext, j+1, uNumPoints, vNumPoints);
            t2.vertex[2] = VertexIndexFromIJ(iNext, j, uNumPoints, vNumPoints);
            
            outTriangleList.push_back(t1);
            outTriangleList.push_back(t2);
        }
       
            
    }
}

static void GenerateTriangleFan(std::vector<Triangle>& outTriangleList, int n_row_points, int centerVertexIndex, int rowIndexStart)
{
    for(int i = 0; i < n_row_points; i++) {
        int firstOuter = i;
        int secondOuter = i + 1;
        if (secondOuter >= n_row_points) {
            secondOuter = 0; // wrap around.
        }
        firstOuter += rowIndexStart;
        secondOuter += rowIndexStart;
        outTriangleList.push_back({centerVertexIndex, firstOuter, secondOuter});
    }
}

void AddTriangleFanEndCaps(
                           std::vector<Vector3>& ioSurfacePoints,
                           bool wantNormals,
                           std::vector<Vector3>& ioNormals,
                           std::vector<Triangle>& ioTriangleList, int n_row_points, const Vector3& beginCenterPoint, const Vector3& endCenterPoint,
                           const Vector3& beginNormal, const Vector3& endNormal)
{
    int originalVertexCount = (int) ioSurfacePoints.size();
    
    // must have at least one row to start with.
    DbgAssert(originalVertexCount >= n_row_points);
    
    // add the new surface points.
    ioSurfacePoints.insert(ioSurfacePoints.begin(), beginCenterPoint);
    ioSurfacePoints.push_back(endCenterPoint);
    
    // add normals for the new points.
    if(wantNormals) {
        ioNormals.insert(ioNormals.begin(), beginNormal);
        ioNormals.push_back(endNormal);
    }
    
    // generate both fans.
    std::vector<Triangle> beginFan, endFan;
    
    GenerateTriangleFan(beginFan, n_row_points, 0, 1);
    GenerateTriangleFan(endFan, n_row_points, originalVertexCount + 1, originalVertexCount - n_row_points + 1);
    
    // Shift all the triangle indices up by one to account for new point at beginning.
    for(int i=0; i < (int)ioTriangleList.size(); i++) {
        auto t = ioTriangleList[i];
        t.vertex[0]++;
        t.vertex[1]++;
        t.vertex[2]++;
        ioTriangleList[i] = t;
    }
    
    // insert beginning fan
    ioTriangleList.insert(ioTriangleList.begin(), beginFan.begin(), beginFan.end());
    
    // append end fan, but flip order first so the vertices are wound correctly for the normal direction.
    for(auto& t : endFan) {
        // flip order of vertices...
        auto temp = t.vertex[0];
        t.vertex[0] = t.vertex[2];
        t.vertex[2]  = temp;
    }
    ioTriangleList.insert(ioTriangleList.end(), endFan.begin(), endFan.end());
}



bool TestTriangles()
{
    const int kNumUPoints = 4;
    const int kNumVPoints = 3;
 
	// Test calculating normal.
	Vector3 n1 = TriangleNormal({ 0, 0, 0 }, { 1, 0, 0 }, { 1, 1, 0 });
	DbgAssertVectorsAlmostEqual({ 0, 0, 1 }, n1);
	
    // Test calculating centroid
    Vector3 centroid = TriangleCentroid({ 0, 0, 0 }, { 1, 0, 0 }, { 1, 1, 0 });
    DbgAssertVectorsAlmostEqual({ 2.0/3.0, 1.0/3.0, 0.0 }, centroid);
    
    // Test generating triangles.
    std::vector<Triangle> triangles;
    GenerateTriangleIndexes(kNumUPoints, kNumVPoints, triangles, false);
    const int kExpectedNumTriangles = (kNumUPoints-1)*(kNumVPoints-1)*2;
    DbgAssert(triangles.size() == kExpectedNumTriangles);
    // row 1
    DbgAssert(triangles[0] == Triangle(0,4,1));
    DbgAssert(triangles[1] == Triangle(4,5,1));
    DbgAssert(triangles[2] == Triangle(1,5,2));
    DbgAssert(triangles[3] == Triangle(5,6,2));
    DbgAssert(triangles[4] == Triangle(2,6,3));
    DbgAssert(triangles[5] == Triangle(6,7,3));
    // row 2
    DbgAssert(triangles[6] == Triangle(4,8,5));
    DbgAssert(triangles[7] == Triangle(8,9,5));
    DbgAssert(triangles[8] == Triangle(5,9,6));
    DbgAssert(triangles[9] == Triangle(9,10,6));
    DbgAssert(triangles[10] == Triangle(6,10,7));
    DbgAssert(triangles[11] == Triangle(10,11,7));
    
    
    // Test adding triangle fans.
    std::vector<Vector3> surfacePoints;
    std::vector<Vector3> normals;
    const int kTotalPoints = kNumUPoints * kNumVPoints;
    
    for(int i=0; i < kTotalPoints; i++) {
        double z = (double)(i / kNumUPoints) * 5.0;
        int j = i % 6;
        double x = cos( (double)j * TwoPI / (double)kNumUPoints );
        double y = sin( (double)j * TwoPI / (double)kNumUPoints );
        Vector3 pt = {x,y,z};
        Vector3 normal = pt; // same direction as point.
        normal.Normalize();
        surfacePoints.push_back(pt * 2.0);
        normals.push_back(normal);
    }
    
    
    DbgAssert(surfacePoints.size() == kTotalPoints);
    DbgAssert(normals.size() == kTotalPoints);
    
    // test GenerateTriangleFan(std::vector<Triangle>& outTriangleList, int n_row_points, int centerVertexIndex, int rowIndexStart)
    std::vector<Triangle> fan;
    GenerateTriangleFan(fan, 4, 0, 1);
    DbgAssert(fan.size() == 4);
    DbgAssert(fan[0] == Triangle(0,1,2));
    DbgAssert(fan[1] == Triangle(0,2,3));
    DbgAssert(fan[2] == Triangle(0,3,4));
    DbgAssert(fan[3] == Triangle(0,4,1));
    
    // and add triangle end caps and check
    const Vector3 begCenterPoint = {0,0,0};
    const Vector3 endCenterPoint = {0,0,5.0};
    
    // make a copy of surfacePoints and normals for comparison.
    std::vector<Vector3> expectedNormals = normals;
    expectedNormals.insert(expectedNormals.begin(), {0,0,1});
    expectedNormals.insert(expectedNormals.end(), {0,0,-1});
    
    std::vector<Vector3> expectedSurfacePoints = surfacePoints;
    expectedSurfacePoints.insert(expectedSurfacePoints.begin(), begCenterPoint);
    expectedSurfacePoints.insert(expectedSurfacePoints.end(), endCenterPoint);
    
    std::vector<Triangle> originalTriangles = triangles;
    
    Vector3 beginNormal = endCenterPoint - begCenterPoint;
    beginNormal.Normalize();
    Vector3 endNormal = begCenterPoint - endCenterPoint;
    endNormal.Normalize();
    
    AddTriangleFanEndCaps(surfacePoints, true, normals, triangles, kNumUPoints, begCenterPoint, endCenterPoint, beginNormal, endNormal);
    
    // should be a new point and normal added at each end of list.
    DbgAssert(surfacePoints.size() == kTotalPoints + 2);
    DbgAssert(normals.size() == kTotalPoints + 2);
    
    // Check values of points and normals, new and old.
    for(size_t i=0; i < surfacePoints.size(); i++) {
        if (!DbgAssertVectorsAlmostEqual( expectedSurfacePoints[i], surfacePoints[i], kMaxDeltaMag, kMaxCompDiff)) {
            cerr << "Point at index " << i << " expected " << expectedSurfacePoints[i] << " actual " << surfacePoints[i] << endl;
        }
        
        if (!DbgAssertVectorsAlmostEqual( expectedNormals[i], normals[i], kMaxDeltaMag, kMaxCompDiff)) {
            cerr << "Normal at index " << i << " expected " << expectedNormals[i] << " actual " << normals[i] << endl;
        }
    }
    
    // Should be fan with kNumUPoints triangles added at start.
    // Should be fan with kNumUPoints triangles added at end.
    const int kFanSize = kNumUPoints;
    DbgAssert(triangles.size() == kExpectedNumTriangles + 2 * kFanSize);
    DbgAssert(triangles[0] == Triangle(0,1,2));
    DbgAssert(triangles[1] == Triangle(0,2,3));
    DbgAssert(triangles[2] == Triangle(0,3,4));
    DbgAssert(triangles[3] == Triangle(0,4,1));
    
    // All prior triangles should be offset by 1, and shift in place by beginning fan size.
    for(int idx=0; idx < (int)originalTriangles.size();  idx++) {
        int newIdx = idx + kFanSize;
        Triangle expected = originalTriangles[idx];
        Triangle actual = triangles[newIdx];
        for(int v = 0; v < 3; v++) {
            DbgAssert(actual.vertex[v] == expected.vertex[v] + 1);
        }
    }
    
    DbgAssert(triangles[16] == Triangle(10,9, 13));
    DbgAssert(triangles[17] == Triangle(11,10,13));
    DbgAssert(triangles[18] == Triangle(12,11,13));
    DbgAssert(triangles[19] == Triangle(9,12, 13));
        
    if (DbgHasAssertFailed()) {
        return false;
    }
    return true;

}

