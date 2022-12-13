//
//  perspective.hpp
//  opengl_setup_example
//
//  Created by Richard Anton on 12/13/22.
//

#ifndef perspective_hpp
#define perspective_hpp


// Matrix will receive the calculated perspective matrix.
// You would have to upload to your shader
// or use glLoadMatrixf if you aren't using shaders.
void glhPerspectivef2(float *matrix, float fovyInDegrees, float aspectRatio,
                      float znear, float zfar);

void glhFrustumf2(float *matrix, float left, float right, float bottom, float top,
                  float znear, float zfar);


#endif /* perspective_hpp */
