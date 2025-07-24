#pragma once  
#include "../scene/CRTCamera.h"  
using namespace Helper;  

class CRTRay  
{  
public:  

	static CRTRay generateRay(const CRTCamera& camera, const Grid& grid,  
		const Pixel& currentPixel, float angle = 90.f);  

	CRTRay(const CRTVector& cameraOrigin, const CRTVector& rayDirection);  
	CRTRay(CRTVector&& cameraOrigin, CRTVector&& rayDirection);  

	const CRTVector& getCameraOrigin() const;  
	const CRTVector& getRayDirection() const;  

private:  

	CRTVector cameraOrigin;  
	CRTVector rayDirection;  
};
