#include "smf.h"
#include "dbgutils.h"

#include <iostream>
#include <sstream>

using namespace std;

static const char *kSpace = " \n\t";

static Triangle TriangleFromLineString(const std::string s)
{
    stringstream ss(s);
    
    int v1, v2, v3;
    ss >> v1;
    ss >> v2;
    ss >> v3;

    return Triangle(v1, v2, v3);
}


// Read an SMF file containing triangles.
bool ReadSMF(std::istream &is, std::vector<Vector3>& out_verts, std::vector<Triangle>& out_triangles)
{
    out_verts.clear();
    out_triangles.clear();

    std::string line;

    bool faces = false;
    while(getline(is, line)) {
        // strip leading/trailing whitespace
        auto start = line.find_first_not_of(kSpace);
        if (start != string::npos) {
            line = line.substr(start);
        }
        auto finish = line.find_last_not_of(kSpace);
        if (finish != string::npos) {
            line = line.substr(0, finish + 1);
        } else {
            continue; // all spaces.
        }
        if(line.empty())
            continue;  // skip blank lines

        // TODO: ignore comments after valid lines
        char ch = line[0];
        line = line.substr(1, string::npos);
        if(ch == 'v')
        {
            if(faces) {
                std::cerr << "Error reading SMF: vertex seen while reading faces\n";
                return false;
            }
            // read a vertex
            Vector3 v = Vector3::FromLineString(line);
            out_verts.push_back(v);
        } else if(ch == 'f') {
            // read a face (triangle)
            faces = true;
            Triangle t = TriangleFromLineString(line);
            // Check that indices are valid.
			if (t.vertex[0] < 1 || t.vertex[1] < 1 || t.vertex[2] < 1) {
				std::cerr << "Error reading SMF: vertex index less than 1\n";
				return false;
			}
			const int maxIdx = (int)out_verts.size();
			if (t.vertex[0] > maxIdx || t.vertex[1] > maxIdx || t.vertex[2] > maxIdx) {
				std::cerr << "Error reading SMF: vertex index greater than vertex count\n";
				return false;
			}
			// and convert to zero based.
			t.vertex[0]--;
			t.vertex[1]--;
			t.vertex[2]--;
			out_triangles.push_back(t);

			
        } else if(ch == '#') {
            // skip comment
            continue;
        } else {
            std::cerr << "Error reading SMF: unknown line type\n";
            std::cerr << "ch="<< (int)ch << std::endl;
            std::cerr << "Line: " << ch << line << std::endl;
        }

    }
    return true;
}

// The format is 1 based vertex indices, should convert on reading to zero based.
static const char* kTestSMF = 
"v 0.50000 0.60000 0.700000\n"
"v 0.40000 0.80000 0.900000\n"
" v -0.50000 0.50000 0.500000\n"
" f 1 3 2\n"
" \n"
" # ignore me\n"
"f 1 2 3\n"
;

bool TestSMF()
{
    // Test TriangleFromLineString()
    string tstr("3 4 5");
    Triangle t = TriangleFromLineString(tstr);
    if(t.vertex[0] != 3 || t.vertex[1] != 4 || t.vertex[2] != 5)
        return false;

    std::vector<Vector3> verts;
    std::vector<Triangle> triangles;

    stringstream ss(kTestSMF);
    bool result = ReadSMF(ss, verts, triangles);
    if(!result)
        return false;
    // TODO: test bad inputs.

    const Vector3 kExpectedVerts[] = {
        Vector3(0.5, 0.6, 0.7),
        Vector3(0.4, 0.8, 0.9),
        Vector3(-0.5, 0.5, 0.5)
    };
    const Triangle kExpectedTriangles[] = {
        Triangle(0, 2, 1),
        Triangle(0, 1, 2)
    };
    const int kNExpectedVerts = sizeof(kExpectedVerts) / sizeof(Vector3);
    const int kNExpectedTriangles = sizeof(kExpectedTriangles) / sizeof(Triangle);

    DbgAssert(verts.size() == 3);
    DbgAssert(triangles.size() == 2);

    for(int i=0; i < kNExpectedVerts; i++) {
        cout << "Checking vertex " << i << "\n";
        if(!DbgAssertVectorsAlmostEqual(kExpectedVerts[i], verts[i], 0.0001, 0.00001))
            return false;
    }

    for(int i=0; i < kNExpectedTriangles; i++) {
        cout << "Checking triangle " << i << "\n";
        if(kExpectedTriangles[i] != triangles[i])
            return false;
    }

    if (DbgHasAssertFailed()) {
        return false;
    }

    return true;
}

