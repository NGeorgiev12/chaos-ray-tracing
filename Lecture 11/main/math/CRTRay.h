#pragma once  
#include "../scene/CRTCamera.h"  
using namespace Helper;  

enum class RayType  
{  
	CAMERA,
	SHADOW,
	REFLECTION,  
	REFRACTION,  
	INVALID
};

class CRTRay  
{  
public:  

	static CRTRay generateCameraRay(const CRTCamera& camera, const Grid& grid,  
		const Pixel& currentPixel, float angle = 90.f);  

	CRTRay(const CRTVector& cameraOrigin, const CRTVector& rayDirection, RayType rayType, int depth);  
	CRTRay(CRTVector&& cameraOrigin, CRTVector&& rayDirection, RayType rayType, int depth);  

	const CRTVector& getCameraOrigin() const;  
	const CRTVector& getRayDirection() const;  
	RayType getRayType() const;
	int getPathDepth() const;

private:  

	CRTVector cameraOrigin;  
	CRTVector rayDirection;
	RayType rayType = RayType::INVALID;
	int pathDepth = -1;
};
