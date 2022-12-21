#ifndef __SCENE__
#define __SCENE__

#include "camera.h"
#include "ray.h"
#include "shadinginfo.h"
#include "light.h"
#include "surface.h"
#include "bbox.h"

#include <list>
#include <memory>
#include <map>
#include <string>
#include <functional>

class SceneObject {
private:
    int32_t mID;
    static int32_t mNextID;
	std::shared_ptr<Surface> mSurface;

public:
    // base constructor assigns a unique ID
    SceneObject();
    virtual ~SceneObject();
    
    int32_t GetID() const { return mID; }
    
	void SetSurfacePtr(std::shared_ptr<Surface> surface);

	std::shared_ptr<Surface> GetSurfacePtr(void) { return mSurface;  }

	const Surface* GetSurface(void) const {
		return mSurface.get();
	}

	Surface* GetSurface(void) {
		return mSurface.get();
	}

    // return object type name.
    virtual const char* ObjectTypeName(void) const = 0;
    
    virtual bool Hit(const Ray& r, HitInfo& outHit) const = 0;
    
    // must override to return true if object is divisible into parts.
    virtual bool Divisible() const { return false; }
    
    virtual BBox GetBBox(void) const = 0;
    virtual Vector3 GetCentroid(void) const = 0;
    
    // Must implement these if divisible to parts(i.e. triangle in mesh).
    virtual int NumParts(void) const { return 0; }
    virtual BBox GetPartBBox(int partIdx) const { return GetBBox(); }
    virtual Vector3 GetPartCentroid(int partIdx) const { return GetCentroid(); }
    virtual bool PartHit(const Ray& r, HitInfo& outHit, int partIdx) const { return false; }
};


class Scene {
private:
    std::unique_ptr<Camera> mCameraPtr;
    std::map<int32_t, std::unique_ptr<SceneObject>> mObjects;
	// currently only expect to access all lights in order, so just using list.
	std::list<std::unique_ptr<Light>> mLights;

public:
    Scene();
    ~Scene();
    
    // set camera object and take ownership if its memory
    void SetCamera(std::unique_ptr<Camera> cameraPtr);
    
    Camera* GetCamera() { return mCameraPtr.get(); }
  
    // add an object to scene and take ownership of object memory.
    void AddObject(std::unique_ptr<SceneObject> objPtr);
    
    // delete an object and remove it from scene
    void DeleteObject(SceneObject* objPtr);
    void DeleteObject(int32_t withId);
    
    // find an object by id; returns nullptr if not found.
    SceneObject* FindObject(int32_t withId);
    
    // run a void return function on all objects.
    void RunOnObjects(std::function<void(SceneObject*)> func);

	// add a light to scene and take ownership of its memory
	void AddLight(std::unique_ptr<Light> lightPtr);
	
	// remove a light and delete the object.
	void DeleteLight(Light* lightPtr);

	// access (const) list of lights
	const std::list<std::unique_ptr<Light>>& Lights(void) const {
		return mLights;
	}

	static bool Test();
};

#endif

