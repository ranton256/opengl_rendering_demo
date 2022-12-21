#ifndef __LIGHT__
#define __LIGHT__

#pragma once

#include "vector3.h"
#include "color.h"

class Light
{
	// currently we only support point lights.
private:
	Vector3 mLocation;
	ColorF mColor;

public:
	Light() = default;
	Light(const Vector3& inLoc, const ColorF& color);

	~Light() = default;

	const Vector3& GetLocation(void) const { return mLocation; }
	void SetLocation(const Vector3& inLoc) { mLocation = inLoc; }

	const ColorF& GetColor(void) const { return mColor; }
	void SetColor(const ColorF& inColor) { mColor = inColor; }

};

#endif // __LIGHT__
