#ifndef __TRIANGLE_MESH__
#define __TRIANGLE_MESH__

#include "scene.h"
#include "triangles.h"
#include "matrix.h"

#include <vector>

class TriangleMesh : public SceneObject {
private:
	std::vector<Vector3> mVertices;
	std::vector<Triangle> mTriangles;
	// normals are stored per vertex(even for flat shading).
	std::vector<Vector3> mVertexNormals, mTriangleNormals;

public:
	TriangleMesh();
	TriangleMesh(const TriangleMesh& tm);

	virtual ~TriangleMesh();

	bool LoadFromSMF(const char* path);
	bool LoadFromSMF(std::istream &is);

	void TransformPoints(const Matrix& tm);

	void CalcNormals(bool flat = false);

	// return object type name.
	const char* ObjectTypeName() const override;

	bool Hit(const Ray& r, HitInfo& outHit) const override;

    BBox GetBBox(void) const override;
    
    Vector3 GetCentroid(void) const override;
    
    
    bool Divisible() const override { return true; }
    int NumParts(void) const override { return (int)mTriangles.size(); }
    
    BBox GetPartBBox(int partIdx) const override;
    Vector3 GetPartCentroid(int partIdx) const override;
    
    bool PartHit(const Ray& r, HitInfo& outHit, int partIdx) const override;

    
	bool IsFlat(void) const { return mVertexNormals.size() == 0; }


    
    const std::vector<Vector3>& GetVertices(void) const { return mVertices; }
    const std::vector<Triangle>& GetTriangles(void) const { return mTriangles; }
    const std::vector<Vector3>& GetVertexNormals(void) const { return mVertexNormals; }
    
    static bool Test();
    
private:
	Vector3 CalcSmoothNormal(size_t index, double beta, double gamma) const;

    static BBox CalcBBox(Vector3 v1, Vector3 v2, Vector3 v3);
    
	static bool IntersectTriangle(const Ray& r, double& outT, double& outBeta, double& outGamma, const Vector3& v1, const Vector3& v2, const Vector3& v3);
};

bool LoadTestMesh(TriangleMesh& mesh);

#endif // __TRIANGLE_MESH__
