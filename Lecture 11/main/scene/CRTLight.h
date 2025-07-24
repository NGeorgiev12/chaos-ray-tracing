#pragma once
#include "../math/CRTVector.h"

class CRTLight
{
public:

	CRTLight() = default;
	CRTLight(const CRTVector& position, int intensity);
	CRTLight(CRTVector&& position, int intensity);

	const CRTVector& getPosition() const;
	int getIntensity() const;

private:

	CRTVector position;
	int intensity;
};

