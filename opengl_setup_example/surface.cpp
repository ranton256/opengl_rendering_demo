#include "surface.h"


Surface::Surface()
	: mDiffColor(1.0, 1.0, 1.0),
	mSpecColor(1.0, 1.0, 1.0),
	mAmbiColor(1.0, 1.0, 1.0),
	mDiffCoeff(0.8),
	mSpecCoeff(0.1),
	mAmbiCoeff(0.1),
	mSpecFactor(2.0),
    mReflectionCoeff(0.1),
    mTransCoeff(0.0),
    mIndexOfRefraction(1.0),
    mTransColor(1.0, 1.0, 1.0)
{
}

Surface::Surface(ColorF diffColor, ColorF specColor, ColorF ambiColor, double diffCoeff, double specCoeff, double ambiCoeff, double specFactor, double reflectionCoeff)
	: mDiffColor(diffColor), mSpecColor(specColor), mAmbiColor(ambiColor), mDiffCoeff(diffCoeff), mSpecCoeff(specCoeff), mAmbiCoeff(ambiCoeff), mSpecFactor(specFactor), mReflectionCoeff(reflectionCoeff)
{

}

void Surface::SetColor(ColorF clr)
{
	SetDiffColor(clr);
	SetSpecColor(clr);
	SetAmbiColor(clr);
}



bool Surface::Test() {

	// TODO: test something at least constructor and getters


	return true; // TODO: surface test.
}
