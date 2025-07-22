#pragma once
#include "CRTVector.h"

class CRTLight
{
public:

	CRTLight() = default;
	CRTLight(const CRTVector& position, int intensity);
	CRTLight(CRTVector&& position, int intensity);

	const CRTVector& getPosition() const;
	int getIntensity() const;

	void shade();

private:

	CRTVector position;
	int intensity;
};

