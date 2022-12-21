//
//  transform.h
//  geomodel
//
//  Created by Richard Anton on 11/27/21.
//

#ifndef __TRANSFORM__
#define __TRANSFORM__

#include "matrix.h"
#include "vector3.h"

// create a translation matrix.
void TranslationMatrix(Matrix& outM, Vector3 trans);

// create a rotation matrix around the Z axis given theta in radians.
void RotationAroundZMatrix(Matrix& outM, double theta);

// create a rotation matrix around the Y axis given theta in radians.
void RotationAroundYMatrix(Matrix& outM, double theta);

// create a rotation matrix around the X axis given theta in radians.
void RotationAroundXMatrix(Matrix& outM, double theta);

// create a uniform translation matrix.
void ScalingMatrix(Matrix& outM, double scale);

// create a scaling matrix.
void ScalingMatrix(Matrix& outM, double scaleX, double scaleY, double scaleZ);
void ScalingMatrix(Matrix& outM, Vector3 scale);

// Build a composite transform to scale, rotate, and translate.
void CompositeTransform(Matrix& outMat, Vector3 trans, Vector3 scale, Vector3 rotate);

bool TestTransform();

#endif

