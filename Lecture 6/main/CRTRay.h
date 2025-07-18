#pragma once
#include "CRTVector.h"

class CRTRay
{
public:

	CRTRay(const CRTVector& origin, const Grid& grid, const Pixel& currentPixel,
			const CRTMatrix& rotationMatrix);
	
private:

	CRTVector cameraOrigin;
	CRTVector rayDirection;
};

