#pragma once
#include "CRTVector.h"
using namespace Helper;

class CRTRay
{
public:

	CRTRay(const CRTVector& origin, const Grid& grid, const Pixel& currentPixel,
			const CRTMatrix& rotationMatrix = CRTMatrix::identity(), float angle = 90.f);

	const CRTVector& getCameraOrigin() const;
	const CRTVector& getRayDirection() const;
	
private:

	CRTVector cameraOrigin;
	CRTVector rayDirection;
};

