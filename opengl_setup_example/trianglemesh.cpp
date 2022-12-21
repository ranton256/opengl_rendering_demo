#include "trianglemesh.h"

#include "dbgutils.h"
#include "smf.h"
#include "mathutil.h"
#include "transform.h"

#include <fstream>
#include <sstream>

using namespace std;

TriangleMesh::TriangleMesh()
{
}

TriangleMesh::TriangleMesh(const TriangleMesh& tm)
{
	mVertices = tm.mVertices;
	mTriangles = tm.mTriangles;
}

TriangleMesh::~TriangleMesh()
{
}

bool TriangleMesh::LoadFromSMF(const char* path)
{
	ifstream file;

	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		file.open(path, std::ifstream::in);

	}
	catch (exception& oe) {
		cerr << "Error opening file " << path << ": " << oe.what() << endl;
		return false;
	}

	bool result = false;
	file.exceptions(std::ifstream::badbit);
	try
	{
		result = LoadFromSMF(file);
		file.close();
	}
	catch (...)
	{
		cerr << "Error while reading file: " << path << endl;
		try
		{
			if (file.is_open())
				file.close();
		}
		catch (...) {}

		return false;
	}
	return result;
}

bool TriangleMesh::LoadFromSMF(std::istream &is)
{
	bool result = ReadSMF(is, mVertices, mTriangles);
	
	return result;
}

void TriangleMesh::TransformPoints(const Matrix& tm)
{
	for (size_t i = 0; i < mVertices.size(); i++) {
		Vector3 ov(mVertices[i]);
		Matrix vMat;
		ov.ToMatrix(vMat);
		
		Matrix tVecMat;
		Matrix::Multiply(tVecMat, tm, vMat);

		mVertices[i] = Vector3(tVecMat);
	}
}

void TriangleMesh::CalcNormals(bool flat)
{
  	mVertexNormals.clear();
	if (!flat) {
 		for (size_t i = 0; i < mVertices.size(); i++) {
			mVertexNormals.push_back({ 0,0,0 });
		}
	}

	mTriangleNormals.clear();

	for (auto tri : mTriangles) {
		auto v0 = tri.vertex[0], v1 = tri.vertex[1], v2 = tri.vertex[2];

		Vector3 triNormal = TriangleNormal(mVertices[v0], mVertices[v1], mVertices[v2]);
        DbgAssert(!triNormal.IsNan());
        
		mTriangleNormals.push_back(triNormal);
		if (!flat) {
			mVertexNormals[v0] += triNormal;
			mVertexNormals[v1] += triNormal;
			mVertexNormals[v2] += triNormal;
  		}  
	}

	if (!flat) {
		for (size_t i = 0; i < mVertexNormals.size(); i++) {
            DbgAssert(!mVertexNormals[i].IsNan());
            
			mVertexNormals[i].Normalize();
            DbgAssert(!mVertexNormals[i].IsNan());
		}
	}
}


// return object type name.
const char* TriangleMesh::ObjectTypeName() const
{
	return "TriangleMesh";
}

bool TriangleMesh::IntersectTriangle(const Ray& r, double& outT, double& outBeta, double& outGamma, const Vector3& v1, const Vector3& v2, const Vector3& v3)
{
	//         | a b c |
	// mat A = | d e f |
	//         | g h i |

	double a = v1.x - v2.x;
	double b = v1.x - v3.x;
	double c = r.d.x;

	double d = v1.y - v2.y;
	double e = v1.y - v3.y;
	double f = r.d.y;

	double g = v1.z - v2.z;
	double h = v1.z - v3.z;
	double i = r.d.z;

	double detA = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);

	double invDetA = 1.0 / detA;

	// Cramer's rule
	Vector3 rh = v1 - r.o;
	// substitute v1 - r.o for first column to get beta
	// | rh.x b c |  replace a
	// | rh.y e f |  replace d
	// | rh.z h i |  replace g
	double det1 = rh.x * (e * i - f * h) - b * (rh.y * i - f * rh.z) + c * (rh.y * h - e * rh.z);
	double beta = det1 * invDetA;
	if (beta < 0.0)
		return false; // plane intersection not in triangle.

	// substitute v1 - r.o for second column to get gamma
	// | a rh.x c |  replace b
	// | d rh.y f |  replace e
	// | g rh.z i |  replace h
	double det2 = a * (rh.y * i - f * rh.z) - rh.x * (d * i - f * g) + c * (d * rh.z - rh.y * g);
	double gamma = det2 * invDetA;
	if (gamma < 0.0)
		return false; // plane intersection not in triangle.
	if (beta + gamma > 1.0) {
		// not in triangle.
		return false;
	}

	// substitute v1 - r.o for third column to get t
	// | a b rh.x |  replace c
	// | d e rh.y |  replace f
	// | g h rh.z |  replace i
	double det3 = a * (e * rh.z - rh.y * h) - b * (d * rh.z - rh.y  * g) + rh.x * (d * h - e * g);
	double t = det3 * invDetA;
	if (t < EPSILON) {
		return false;
	}
	outT = t;
	outBeta = beta;
	outGamma = gamma;
	return true; 
}

bool TriangleMesh::Hit(const Ray& r, HitInfo& outHit) const
{
	double minT = 0, minTBeta = 0, minTGamma = 0;
	
	int bestIndex = -1;
	for (int index = 0; index < (int)mTriangles.size(); index++) {
		const Triangle& tri = mTriangles[index];
		double t, beta, gamma;
		if (IntersectTriangle(r, t, beta, gamma, mVertices[tri.vertex[0]], mVertices[tri.vertex[1]], mVertices[tri.vertex[2]])) {
			if (bestIndex == -1 || t < minT) {
				minT = t;
				minTBeta = beta;
				minTGamma = gamma;
				bestIndex = index;
			}
		}
	}
	if (bestIndex != -1) {
		outHit.t = minT;
		outHit.hitPoint = r.PointAt(minT);
        outHit.objectIdx = bestIndex;
		if (IsFlat()) {
			DbgAssert(mTriangleNormals.size() > (size_t)bestIndex);
			outHit.normal = mTriangleNormals[bestIndex];
		} else {
			outHit.normal = CalcSmoothNormal(bestIndex, minTBeta, minTGamma);
		}
		outHit.objectPtr = this;

		return true;
	}
	
	return false;
}


Vector3 TriangleMesh::CalcSmoothNormal(size_t index, double beta, double gamma) const
{
	const Triangle& tri = mTriangles[index];
	double alpha = 1.0 - beta - gamma;

	DbgAssert(mVertexNormals.size() == mVertices.size());

	Vector3 normal = mVertexNormals[tri.vertex[0]] * alpha
		+ mVertexNormals[tri.vertex[1]] * beta
		+ mVertexNormals[tri.vertex[2]] * gamma
		;

    
	normal.Normalize();
    
    DbgAssert(!normal.IsNan());
    // This is for debugging any bad mesh calculations.
    if(normal.IsNan()) {
        std::cout << "index=" << index << ", beta=" << beta << ", gamma=" << gamma
            << ", tri=" << tri.vertex[0] << ", " << tri.vertex[1] << ", " << tri.vertex[2]
            << ", trivals=" << mVertexNormals[tri.vertex[0]] << ", " << mVertexNormals[tri.vertex[1]] << ", " << mVertexNormals[tri.vertex[2]]
            << std::endl;
    }
    
	return normal;
}

BBox TriangleMesh::CalcBBox(Vector3 v1, Vector3 v2, Vector3 v3)
{
    double xMin = TMin3(v1.x, v2.x, v3.x);
    double yMin = TMin3(v1.y, v2.y, v3.y);
    double zMin = TMin3(v1.z, v2.z, v3.z);
    
    double xMax = TMax3(v1.x, v2.x, v3.x);
    double yMax = TMax3(v1.y, v2.y, v3.y);
    double zMax = TMax3(v1.z, v2.z, v3.z);
    
    return BBox({xMin, yMin, zMin}, {xMax, yMax, zMax});
}


BBox TriangleMesh::GetBBox(void) const
{
    BBox meshBBox;
    
    for (int index = 0; index < (int)mTriangles.size(); index++) {
        const Triangle& tri = mTriangles[index];
        BBox triBBox = CalcBBox(mVertices[tri.vertex[0]], mVertices[tri.vertex[1]], mVertices[tri.vertex[2]]);
        // cout << "Tri " << index << " bbox min = " << triBBox.Min() << " max = " << triBBox.Max() << endl;
        
        meshBBox.Union(triBBox);
    }
    return meshBBox;
}

Vector3 TriangleMesh::GetCentroid(void) const
{
    // What would this even mean for a mesh?!
    // Should never get called since TriangleMesh::Divisible() returns true.
    DbgAssert(false);
    return Vector3();
}

BBox TriangleMesh::GetPartBBox(int partIdx) const
{
    DbgAssert(partIdx < (int)mTriangles.size());
    
    const Triangle& tri = mTriangles[partIdx];
    return CalcBBox(mVertices[tri.vertex[0]], mVertices[tri.vertex[1]], mVertices[tri.vertex[2]]);
}

Vector3 TriangleMesh::GetPartCentroid(int partIdx) const
{
    const Triangle& tri = mTriangles[partIdx];
    Vector3 c = mVertices[tri.vertex[0]] + mVertices[tri.vertex[1]] + mVertices[tri.vertex[2]];
    constexpr double oneThird = 1.0 / 3.0;
    c *= oneThird;
    return c;
}

bool TriangleMesh::PartHit(const Ray& r, HitInfo& outHit, int partIdx) const
{
    const Triangle& tri = mTriangles[partIdx];
    double t, beta, gamma;
    if (IntersectTriangle(r, t, beta, gamma, mVertices[tri.vertex[0]], mVertices[tri.vertex[1]], mVertices[tri.vertex[2]])) {
        outHit.t = t;
        outHit.hitPoint = r.PointAt(t);
        
        if (IsFlat()) {
            DbgAssert(mTriangleNormals.size() > (size_t)partIdx);
            outHit.normal = mTriangleNormals[partIdx];
        } else {
            outHit.normal = CalcSmoothNormal(partIdx, beta, gamma);
        }
        outHit.objectPtr = this;

        return true;
    }
    return false;
}



static const char* kTestSMF =
"v 0.00000 0.00000 0.000000\n"
"v 0.00000 1.00000 0.000000\n"
"v 1.00000 1.00000 0.000000\n"
"v 1.00000 0.00000 0.000000\n"
"f 1 3 2\n" // do we want CCW?
"f 1 4 3\n"
;

bool LoadTestMesh(TriangleMesh& mesh)
{
    stringstream ss(kTestSMF);
    
    if (!mesh.LoadFromSMF(ss)) {
        cerr << "Error loading test mesh\n";
        return false;
    }

    mesh.CalcNormals();
    return true;
}

bool TriangleMesh::Test()
{
	TriangleMesh mesh;

	if (!LoadTestMesh(mesh)) {
		cerr << "Error loading test mesh\n";
		return false;
	}
	
	// Test Hit()
	Ray r;
	r.o = Vector3(0.6, 0.5, 1);
	r.d = Vector3(0, 0, -1);

	HitInfo hit;
	bool result = mesh.Hit(r, hit);

	DbgAssert(result);
	DbgAssertAlmostEqual(hit.t, 1.0);

    bool part1 = mesh.PartHit(r, hit, 0);
    bool part2 = mesh.PartHit(r, hit, 1);
    DbgAssert(!part1);
    DbgAssert(part2);
    
	r.o = Vector3(-0.1, 0, 0);

	result = mesh.Hit(r, hit);
	DbgAssert(!result);

    
    // Test PartHit misses
    DbgAssert(!mesh.PartHit(r, hit, 0));
    DbgAssert(!mesh.PartHit(r, hit, 1));
    
    
	Matrix tMat;
	TranslationMatrix(tMat, { 0, -1, 0 });
	auto originals = mesh.mVertices;
	mesh.TransformPoints(tMat);

	for (size_t i = 0; i < originals.size(); i++) {
		DbgAssertVectorsAlmostEqual(originals[i] + Vector3{0, -1, 0}, mesh.mVertices[i]);
	}

    BBox bbox = CalcBBox({-1,0,3}, {1, -2, 0}, {0, 2, -3});
    DbgAssertVectorsAlmostEqual({-1,-2,-3}, bbox.Min());
    DbgAssertVectorsAlmostEqual({1,2,3}, bbox.Max());
    
    bbox = CalcBBox({1, -2, 0}, {0, 2, -3}, {-1,0,3});
    DbgAssertVectorsAlmostEqual({-1,-2,-3}, bbox.Min());
    DbgAssertVectorsAlmostEqual({1,2,3}, bbox.Max());
    
    bbox = CalcBBox({0, 2, -3}, {-1,0,3}, {1, -2, 0});
    DbgAssertVectorsAlmostEqual({-1,-2,-3}, bbox.Min());
    DbgAssertVectorsAlmostEqual({1,2,3}, bbox.Max());
    
    // Test mesh bounding box.
    if (!LoadTestMesh(mesh)) {
        return false;
    }
    
    bbox = mesh.GetBBox();
    // cout << "Mesh bbox min = " << bbox.Min() << " max = " << bbox.Max() << endl;
    DbgAssertVectorsAlmostEqual({0,0,0}, bbox.Min());
    DbgAssertVectorsAlmostEqual({1,1,0}, bbox.Max());
    
    DbgAssert(mesh.Divisible());
    
    // cout << "box 1\n";
    bbox = mesh.GetPartBBox(0);
    DbgAssertVectorsAlmostEqual({0,0,0}, bbox.Min());
    DbgAssertVectorsAlmostEqual({1,1,0}, bbox.Max());
    
    // cout << "box 2\n";
    bbox = mesh.GetPartBBox(1);
    DbgAssertVectorsAlmostEqual({0,0,0}, bbox.Min());
    DbgAssertVectorsAlmostEqual({1,1,0}, bbox.Max());

    // cout << "done box\n";
    
    if(!DbgAssertVectorsAlmostEqual({1.0/3.0,2.0/3.0,0.0}, mesh.GetPartCentroid(0))) {
        cout << "mesh.GetPartCentroid(0)=" << mesh.GetPartCentroid(0) << endl;
    }
    
    
    
	if (DbgHasAssertFailed()) {
		return false;
	}

	return true;
}
