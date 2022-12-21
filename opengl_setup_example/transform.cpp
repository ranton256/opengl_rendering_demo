//
//  transform.cpp
//  geomodel
//
//  Created by Richard Anton on 11/27/21.
//

#include "transform.h"
#include "dbgutils.h"
#include "mathutil.h"


void TranslationMatrix(Matrix& outM, Vector3 trans)
{
    outM.Identity(4);
    outM.Set(0, 3, trans.x);
    outM.Set(1, 3, trans.y);
    outM.Set(2, 3, trans.z);
}

// create a rotation matrix around the z axis given theta in radians.
void RotationAroundZMatrix(Matrix& outM, double theta)
{
    outM.Identity(4);
    double cosTheta = cos(theta);
    double sinTheta = sin(theta);
    outM[0][0] = cosTheta;
    outM[0][1] = -sinTheta;
    outM[1][0] = sinTheta;
    outM[1][1] = cosTheta;
}

// create a rotation matrix around the Y axis given theta in radians.
void RotationAroundYMatrix(Matrix& outM, double theta)
{
    outM.Identity(4);
    double cosTheta = cos(theta);
    double sinTheta = sin(theta);
    outM[0][0] = cosTheta;
    outM[0][2] = sinTheta;
    outM[2][0] = -sinTheta;
    outM[2][2] = cosTheta;
}

// create a rotation matrix around the X axis given theta in radians.
void RotationAroundXMatrix(Matrix& outM, double theta)
{
    outM.Identity(4);
    double cosTheta = cos(theta);
    double sinTheta = sin(theta);
    outM[1][1] = cosTheta;
    outM[1][2] = -sinTheta;
    outM[2][1] = sinTheta;
    outM[2][2] = cosTheta;
}

void ScalingMatrix(Matrix& outM, double scale)
{
    ScalingMatrix(outM, scale, scale, scale);
}

// create a scaling matrix.
void ScalingMatrix(Matrix& outM, double scaleX, double scaleY, double scaleZ)
{
    outM.Identity(4);
    outM[0][0] = scaleX;
    outM[1][1] = scaleY;
    outM[2][2] = scaleZ;
}

void ScalingMatrix(Matrix& outM, Vector3 scale)
{
    ScalingMatrix(outM, scale.x, scale.y, scale.z);
}



void CompositeTransform(Matrix& outMat, Vector3 trans, Vector3 scale, Vector3 rotate)
{
    Matrix temp, moveMat, scaleMat;
    
    TranslationMatrix(moveMat, trans);
    ScalingMatrix(temp, scale);
    
    // Handle all 3 axes of rotations.
    if(TAbs(rotate.x) > EPSILON){
        Matrix curr(temp), rotMat;
        RotationAroundXMatrix(rotMat, rotate.x);
        Matrix::Multiply(temp, rotMat, curr);
    }
    
    if(TAbs(rotate.y) > EPSILON){
        Matrix curr(temp), rotMat;
        RotationAroundYMatrix(rotMat, rotate.y);
        Matrix::Multiply(temp, rotMat, curr);
        
    }
    if(TAbs(rotate.z) > EPSILON){
        Matrix curr(temp), rotMat;
        RotationAroundZMatrix(rotMat, rotate.z);
        Matrix::Multiply(temp, rotMat, curr);
    }
    
    Matrix::Multiply(outMat, moveMat, temp);
}



bool TestTransform()
{
    // Test translation
    {
        Vector3 v {3,2,1};
        Matrix vMat;
        v.ToMatrix(vMat);
        Matrix trans;
        TranslationMatrix(trans, {0.1, 0.2, 0.3});
        
        Matrix tVecMat;
        Matrix::Multiply(tVecMat, trans, vMat);
        
        DbgAssert(tVecMat.Rows() == 4);
        DbgAssert(tVecMat.Cols() == 1);
        
        Vector3 tVec(tVecMat);
        DbgAssert(tVec == Vector3(3.1, 2.2, 1.3));
        
        DbgAssert(tVecMat[0][3] == 1.0);
    }
    // Test rotation around z.
    {
        Matrix zRotMat, zRotStartMat, zRotated;
        RotationAroundZMatrix(zRotMat, PI / 2.0); // 90 degrees, CCW
        
        Vector3 zRotStart {1.0, 0.0, 5.0};
        zRotStart.ToMatrix(zRotStartMat);
        
        Matrix::Multiply(zRotated, zRotMat, zRotStartMat);
        
        DbgAssert(zRotated.Rows() == 4);
        DbgAssert(zRotated.Cols() == 1);
        Vector3 zRotatedVec(zRotated);
        
        DbgAssert(DbgAssertVectorsAlmostEqual(Vector3(0.0, 1.0, 5.0), zRotatedVec));
        DbgAssert(zRotated[0][3] == 1.0);
    }
    // Test rotation around Y.
    {
        Matrix yRotMat, yRotStartMat, yRotated;
        RotationAroundYMatrix(yRotMat, PI / 2.0); // 90 degrees, CCW
        
        Vector3 yRotStart {1.0, 3.0, 0.0};
        yRotStart.ToMatrix(yRotStartMat);
        
        Matrix::Multiply(yRotated, yRotMat, yRotStartMat);
        
        DbgAssert(yRotated.Rows() == 4);
        DbgAssert(yRotated.Cols() == 1);
        Vector3 yRotatedVec(yRotated);
        
        DbgAssert(DbgAssertVectorsAlmostEqual(Vector3(0.0, 3.0, -1.0), yRotatedVec));
        DbgAssert(yRotated[0][3] == 1.0);
    }
    // Test rotation around X.
    {
        Matrix xRotMat, xRotStartMat, xRotated;
        RotationAroundXMatrix(xRotMat, PI / 2.0); // 90 degrees, CCW
        
        Vector3 xRotStart {4.0, 1.0, 0.0};
        xRotStart.ToMatrix(xRotStartMat);
        
        Matrix::Multiply(xRotated, xRotMat, xRotStartMat);
        
        DbgAssert(xRotated.Rows() == 4);
        DbgAssert(xRotated.Cols() == 1);
        Vector3 xRotatedVec(xRotated);
        
        DbgAssert(DbgAssertVectorsAlmostEqual(Vector3(4.0, 0.0, 1.0), xRotatedVec));
        DbgAssert(xRotated[0][3] == 1.0);
    }
    // Test Scaling
    {
        Vector3 v {3,2,1};
        Matrix vMat;
        v.ToMatrix(vMat);
        Matrix scaling;
        ScalingMatrix(scaling, 0.5, 0.2, 2.0);
        
        Matrix tVecMat;
        Matrix::Multiply(tVecMat, scaling, vMat);
        
        DbgAssert(tVecMat.Rows() == 4);
        DbgAssert(tVecMat.Cols() == 1);
        
        Vector3 tVec(tVecMat);
        DbgAssert(tVec == Vector3(1.5, 0.4, 2.0));
        
        DbgAssert(tVecMat[0][3] == 1.0);
    }
    // Test  Uniform Scaling
    {
        Vector3 v {3,2,1};
        Matrix vMat;
        v.ToMatrix(vMat);
        Matrix scaling;
        ScalingMatrix(scaling, 2.0);
        
        Matrix tVecMat;
        Matrix::Multiply(tVecMat, scaling, vMat);
        
        DbgAssert(tVecMat.Rows() == 4);
        DbgAssert(tVecMat.Cols() == 1);
        
        Vector3 tVec(tVecMat);
        DbgAssert(tVec == Vector3(6,4,2));
        
        DbgAssert(tVecMat[0][3] == 1.0);
    }
    
    
    if (DbgHasAssertFailed()) {
        return false;
    }
    return true;
}

