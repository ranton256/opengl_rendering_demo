#ifndef __SURFACE__
#define __SURFACE__

#pragma once

#include <memory>
#include "color.h"

constexpr double AIR_INDEX_REFRACTION = 1.00029;

class Material;

class Surface {
	// Currently we just have simple (non-textured) surfaces.
private:
    // TODO: we we want to add a separate reflective color?
	ColorF mDiffColor, mSpecColor, mAmbiColor;
    double mDiffCoeff, mSpecCoeff, mAmbiCoeff, mSpecFactor, mReflectionCoeff;
    double mTransCoeff, mIndexOfRefraction;
    ColorF mTransColor;
    
    std::shared_ptr<Material> mMaterial;
    
    
public:
	Surface();
	
	Surface(ColorF diffColor, ColorF specColor, ColorF ambiColor, double diffCoeff, double specCoeff, double ambiCoeff, double specFactor, double reflectionCoeff);

	~Surface() = default;

    void SetMaterial(std::shared_ptr<Material> material) {
        mMaterial = material;
    }
    
    const Material* GetMaterialPtr(void) const { return mMaterial.get(); }
    
    
    // TODO: change to use material when set for color instead based on u,v
	const ColorF GetDiffColor(void) const { return mDiffColor; }
	const ColorF GetSpecColor(void) const { return mSpecColor; }
	const ColorF GetAmbiColor(void) const { return mAmbiColor; }

    
	double GetDiffCoeff(void) const { return mDiffCoeff; }
	double GetSpecCoeff(void) const { return mSpecCoeff; }
	double GetAmbiCoeff(void) const { return mAmbiCoeff; }

	double GetSpecFactor(void) const { return mSpecFactor; }

    double GetReflectionCoeff(void) const { return mReflectionCoeff; }
    
    double GetTransCoeff(void) const { return mTransCoeff; }
    double GetIndexOfRefraction(void) const { return mIndexOfRefraction; }
    const ColorF GetTransColor(void) const { return mTransColor; }
    
	// set all 3 (diffuse, spec, ambient) colors.
	void SetColor(ColorF clr);

	void SetDiffColor(ColorF clr) { mDiffColor = clr; }
	void SetSpecColor(ColorF clr) { mSpecColor = clr; }
	void SetAmbiColor(ColorF clr) { mAmbiColor = clr; }

	void SetDiffCoeff(double cf) { mDiffCoeff = cf; }
	void SetSpecCoeff(double cf) { mSpecCoeff = cf; }
	void SetAmbiCoeff(double cf) { mAmbiCoeff = cf; }

	void SetSpecFactor(double fact) { mSpecFactor = fact; }

    void SetReflectionCoeff(double cf) { mReflectionCoeff = cf; }

    void SetTransCoeff(double cf) { mTransCoeff = cf; }
    void SetIndexOfRefraction(double r) { mIndexOfRefraction = r; }
    void SetTransColor(ColorF clr) { mTransColor = clr; }
    
	static bool Test();
    
    friend std::ostream& operator<< (std::ostream& os, const Surface& s);
};

inline std::ostream& operator<< (std::ostream& os, const Surface& s)
{
    os << "\tsurface[\n";
    os << "\t\tcolors diffuse=" << s.mDiffColor << ", ";
    os << " ambient=" << s.mAmbiColor << ", ";
    os << " specular=" << s.mSpecColor << "\n";
    
    os << "\t\tcoeff. diffuse=" << s.mDiffCoeff << ", ";
    os << " ambient=" << s.mAmbiCoeff << ", ";
    os << " specular=" << s.mSpecCoeff << "\n";
    
    os << "\t\tspecular factor=" << s.mSpecFactor << "\n";
    
    os << "\t\treflection=" << s.mReflectionCoeff << "\n";
    os << "\t]\n";

    return os;
}


#endif // __SURFACE__
