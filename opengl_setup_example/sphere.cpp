//
//  sphere.cpp
//  opengl_setup_example
//
//  Created by Richard Anton on 12/18/22.
//

#include "sphere.h"

#include <vector>
#include "mathutil.h"
#include "vector3.h"

static void PushVector3(std::vector<float> &floatVec, const Vector3& v) {
    floatVec.push_back(v.x);
    floatVec.push_back(v.y);
    floatVec.push_back(v.z);
}

void GenerateSphere(float radius, int longCount, int latCount,
                    std::vector<float>& vertices, std::vector<float>& normals, std::vector<float>& texCoords,
                    std::vector<int>& indices
                    )
{
    const float oneOverRadius = 1.0f / radius;

    float longInc = 2 * PI / longCount;
    float latInc = PI / latCount;
    
    for(int i = 0; i <= latCount; ++i)
    {
        float latAngle = PI / 2 - i * latInc;
        float radCosLat = radius * cosf(latAngle);
        float z = radius * sinf(latAngle);

        for(int j = 0; j <= longCount; ++j)
        {
            float longAngle = j * longInc;
            
            Vector3 pos = {radCosLat * cosf(longAngle), radCosLat * sinf(longAngle), z};
            
            PushVector3(vertices, pos);
            PushVector3(normals, pos * oneOverRadius);
            
            texCoords.push_back((float)j / longCount);
            texCoords.push_back((float)i / latCount);
        }
    }
    
    const int colCount = longCount + 1;
    for(int i = 0; i < latCount; ++i)
    {
        int idx1 = i * colCount;

        for(int j = 0; j < longCount; j++)
        {
            if(i > 0)
            {
                indices.push_back(idx1);
                indices.push_back(idx1 + colCount);
                indices.push_back(idx1 + 1);
            }

            if(i < latCount - 1)
            {
                indices.push_back(idx1 + 1);
                indices.push_back(idx1 + colCount);
                indices.push_back(idx1 + colCount + 1);
            }
            
            ++idx1;
        }
    }
}
