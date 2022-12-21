#include "scene.h"
#include "dbgutils.h"

int32_t SceneObject::mNextID = 0;

// base constructor assigns a unique ID
SceneObject::SceneObject()
{
    // This is not thread safe.
    // We are assuming scenes are built in single thread.
    mID = ++ mNextID;
}

SceneObject::~SceneObject()
{
    
}

void SceneObject::SetSurfacePtr(std::shared_ptr<Surface> surface)
{
	mSurface = surface;
}

Scene::Scene()
{
    
}

Scene::~Scene()
{
    
}

void Scene::SetCamera(std::unique_ptr<Camera> cameraPtr)
{
    mCameraPtr = std::move(cameraPtr);
}

// add an object to scene and take ownership of object memory.
void Scene::AddObject(std::unique_ptr<SceneObject> objPtr)
{
    DbgAssert(objPtr != nullptr);
    if(objPtr)
        mObjects.insert(make_pair(objPtr->GetID(), std::move(objPtr)));
}

// delete an object and remove it from scene
void Scene::DeleteObject(SceneObject* objPtr)
{
    DbgAssert(objPtr != nullptr);
    if(objPtr)
        mObjects.erase(objPtr->GetID());
}

void Scene::DeleteObject(int32_t withId)
{
    auto obj = FindObject(withId);
    if(obj)
        DeleteObject(obj);
}

// find an object by id; returns nullptr if not found.
SceneObject* Scene::FindObject(int32_t withId)
{
    auto it = mObjects.find(withId);
    if(it == mObjects.end())
        return nullptr;
    return it->second.get();
}

// run a void return function on all objects.
void Scene::RunOnObjects(std::function<void(SceneObject*)> func)
{
    for(auto it = mObjects.begin(); it != mObjects.end(); it++) {
        func(it->second.get());
    }
}

// add a light to scene and take ownership of its memory
void Scene::AddLight(std::unique_ptr<Light> lightPtr)
{
	mLights.push_back(std::move(lightPtr));
}


void Scene::DeleteLight(Light* lightPtr)
{
	DbgAssert(lightPtr != nullptr);
	if (lightPtr) {
		for (auto it = mLights.begin(); it != mLights.end(); it++) {
			if (it->get() == lightPtr) {
				mLights.erase(it);
				break;
			}
		}
	}
}


class TestObject : public SceneObject {
public:
	int mValue;
	TestObject(int value) : mValue(value) {}
	~TestObject() {}

	const char *ObjectTypeName(void) const override { return "TestObject"; } 
	bool Hit(const Ray& r, HitInfo& outHit) const override { return false; }
    
    BBox GetBBox(void) const override { return {{0,0,0},{1,1,1}}; }
    Vector3 GetCentroid(void) const override { return {0,0,0}; }
};

bool Scene::Test()
{
	
	Scene scene;
	std::unique_ptr<SceneObject> p1(new TestObject(1));
	auto id1 = p1->GetID();
	auto realPtr1 = p1.get();
	scene.AddObject(std::move(p1));

	std::unique_ptr<SceneObject> p2(new TestObject(2));
	auto id2 = p2->GetID();
	auto realPtr2 = p2.get();
	scene.AddObject(std::move(p2));

	DbgAssert(id1 != id2);
	DbgAssert(id1 > 0);
	DbgAssert(id2 > 0);

	DbgAssert(scene.FindObject(id1) == realPtr1);
	DbgAssert(scene.FindObject(id2) == realPtr2);
	DbgAssert(scene.FindObject(99) == nullptr);

	bool found1 = false, found2 = false;

	auto onEach = [&](SceneObject* o) {
		// std::cout << "onEach o:" << o << " id:" << o->GetID() << std::endl;
		if (o->GetID() == id1) {
			found1 = true;
		} else if (o->GetID() == id2) {
			found2 = true;
		} else {
			DbgAssert(false);
		}
	};

	scene.RunOnObjects(onEach);
	DbgAssert(found1);
	DbgAssert(found2);


	scene.DeleteObject(id1);
	scene.DeleteObject(realPtr2);
	DbgAssert(scene.FindObject(id1) == nullptr);
	DbgAssert(scene.FindObject(id2) == nullptr);


	std::unique_ptr<Light> l1(new Light({ 0,0,1 }, { 1.0f, 0.2f, 0.2f }));
	auto lightPtr1 = l1.get();
	scene.AddLight(std::move(l1));
	
	
	std::unique_ptr<Light> l2(new Light({ 1,0,0 }, { 0.5f, 0.5f, 0.5f }));
	auto lightPtr2 = l2.get();
	scene.AddLight(std::move(l2));

	DbgAssert(scene.Lights().size() == 2);
	DbgAssert(scene.Lights().front().get() == lightPtr1);
	DbgAssert(scene.Lights().back().get() == lightPtr2);

	for (auto it = scene.Lights().begin(); it != scene.Lights().end(); it++) {
		Light* light = it->get();
		std::cout << "Light at " << light->GetLocation() << " of color " << light->GetColor() << std::endl;
	}

	scene.DeleteLight(lightPtr1);
	scene.DeleteLight(lightPtr2);
	DbgAssert(scene.Lights().size() == 0);

	if (DbgHasAssertFailed()) {
		return false;
	}

	return true;
}
