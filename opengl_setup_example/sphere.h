//
//  sphere.hpp
//  opengl_setup_example
//
//  Created by Richard Anton on 12/18/22.
//

#ifndef sphere_hpp
#define sphere_hpp

#include <vector>

void GenerateSphere(float radius, int sectorCount, int stackCount,
                    std::vector<float>& vertices, std::vector<float>& normals, std::vector<float>& texCoords,
                    std::vector<int>& indices);

#endif /* sphere_hpp */
