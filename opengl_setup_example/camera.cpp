//
//  camera.cpp
//  oramaxc
//
//  Created by Richard Anton on 4/3/22.
//

#include "camera.h"


Camera::Camera(const Vector3& eyePoint,
       const Vector3& viewDirection,
       const Vector3& viewUp,
       double imagePlaneDistance,
       double horizCameraAngle
       )
: mEyePoint(eyePoint), mViewDirection(viewDirection), mViewUp(viewUp),
  mImagePlaneDistance(imagePlaneDistance), mHorizCameraAngle(horizCameraAngle)
{
    
}

Camera::Camera(const Camera& c)
{
    mEyePoint = c.mEyePoint;
    mViewDirection = c.mViewDirection;
    mViewUp = c.mViewUp;
    mImagePlaneDistance = c.mImagePlaneDistance;
    mHorizCameraAngle = c.mHorizCameraAngle;
}


void Camera::SetEyePoint(const Vector3& eyePoint)
{
    mEyePoint = eyePoint;
}

void Camera::SetViewDirection(const Vector3& viewDirection)
{
    mViewDirection = viewDirection;
}

void Camera::SetViewUp(const Vector3& viewUp)
{
    mViewUp = viewUp;
}

void Camera::SetImagePlaneDistance(double imagePlaneDistance)
{
    mImagePlaneDistance = imagePlaneDistance;
}

void Camera::SetHorizCameraAngle(double horizCameraAngle)
{
    mHorizCameraAngle = horizCameraAngle;
}


