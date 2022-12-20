//
//  cube.hpp
//  opengl_setup_example
//
//  Created by Richard Anton on 12/19/22.
//

#ifndef cube_hpp
#define cube_hpp

#include <vector>

void GenerateCube(std::vector<float>& vertices, std::vector<float>& texCoords);
void GenerateNormals(const std::vector<float>& vertices, std::vector<float>& normals);

#endif /* cube_hpp */
