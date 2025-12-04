#pragma once
#include "CRTCamera.h"
using namespace Helper;

class CRTRay
{
public:

	CRTRay(const CRTCamera& camera, const Grid& grid,
		const Pixel& currentPixel, float angle = 90.f);

	const CRTVector& getCameraOrigin() const;
	const CRTVector& getRayDirection() const;
	
private:

	CRTVector cameraOrigin;
	CRTVector rayDirection;
};

