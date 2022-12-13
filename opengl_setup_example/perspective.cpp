//
//  perspective.cpp
//  opengl_setup_example
//
//  Created by Richard Anton on 12/13/22.
//


#include "perspective.h"
#include <cmath>

// TODO: Also look at https://www.khronos.org/opengl/wiki/GluLookAt_code
//
//gluLookAt(camera.eye.x,camera.eye.y,camera.eye.z,
//        camera.lookAt.x,camera.lookAt.y,camera.lookAt.z,
//        camera.up.x,camera.up.y,camera.up.z);


// Based on code from https://www.khronos.org/opengl/wiki/GluPerspective_code
// which is in term from glh library (OpenGL Helper Library), LGPL license https://sourceforge.net/projects/glhlib


// Matrix will receive the calculated perspective matrix.
// You would have to upload to your shader
// or use glLoadMatrixf if you aren't using shaders.
void glhPerspectivef2(float *matrix, float fovyInDegrees, float aspectRatio,
                      float znear, float zfar)
{
    float ymax, xmax;
    // float temp, temp2, temp3, temp4;
    ymax = znear * tanf(fovyInDegrees * M_PI / 360.0);
    // ymin = -ymax;
    // xmin = -ymax * aspectRatio;
    xmax = ymax * aspectRatio;
    glhFrustumf2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

void glhFrustumf2(float *matrix, float left, float right, float bottom, float top,
                  float znear, float zfar)
{
    float temp, temp2, temp3, temp4;
    temp = 2.0 * znear;
    temp2 = right - left;
    temp3 = top - bottom;
    temp4 = zfar - znear;
    matrix[0] = temp / temp2;
    matrix[1] = 0.0;
    matrix[2] = 0.0;
    matrix[3] = 0.0;
    matrix[4] = 0.0;
    matrix[5] = temp / temp3;
    matrix[6] = 0.0;
    matrix[7] = 0.0;
    matrix[8] = (right + left) / temp2;
    matrix[9] = (top + bottom) / temp3;
    matrix[10] = (-zfar - znear) / temp4;
    matrix[11] = -1.0;
    matrix[12] = 0.0;
    matrix[13] = 0.0;
    matrix[14] = (-temp * zfar) / temp4;
    matrix[15] = 0.0;
}

//  TODO: decide which we are using.
void SimpleMakePerspectiveMat(float *m, float fov, float aspect,
float znear, float zfar)
{
    float f = 1/tan(fov * (M_PI / 360.0));

    m[0]  = f/aspect;
    m[1]  = 0;
    m[2]  = 0;
    m[3]  = 0;

    m[4]  = 0;
    m[5]  = f;
    m[6]  = 0;
    m[7]  = 0;

    m[8]  = 0;
    m[9]  = 0;
    m[10] = (zfar + znear) / (znear - zfar);
    m[11] = -1;

    m[12] = 0;
    m[13] = 0;
    m[14] = 2*zfar*znear / (znear - zfar);
    m[15] = 0;
}
