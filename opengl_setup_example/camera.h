//
//  camera.hpp
//  oramaxc
//
//  Created by Richard Anton on 4/3/22.
//

#ifndef camera_hpp
#define camera_hpp

#include "vector3.h"

class Camera {
private:
    /*
     * O, eye point
     * Zv, view out direction
     * Vup, view up direction, Vup
     * d, distance to image plane
     * theta, horizontal camera angle
     */
    Vector3 mEyePoint;
    Vector3 mViewDirection;
    Vector3 mViewUp;
    double mImagePlaneDistance;
    double mHorizCameraAngle;
    
    
public:
    Camera() = default;
    
    Camera(const Camera& c);
    
    Camera(const Vector3& eyePoint,
           const Vector3& viewDirection,
           const Vector3& viewUp,
           double imagePlaneDistance,
           double horizCameraAngle
           );

    ~Camera() {}
           
    
    const Vector3& EyePoint() const { return mEyePoint; }
    const Vector3& ViewDirection() const { return mViewDirection; }
    const Vector3& ViewUp() const { return mViewUp; }
    double ImagePlaneDistance() const { return mImagePlaneDistance; }
    double HorizCameraAngle() const { return mHorizCameraAngle; }
    
    void SetEyePoint(const Vector3& eyePoint);
    void SetViewDirection(const Vector3& viewDirection);
    void SetViewUp(const Vector3& viewUp);
    void SetImagePlaneDistance(double mImagePlaneDistance);
    void SetHorizCameraAngle(double mHorizCameraAngle);
    
};

#endif /* camera_hpp */
