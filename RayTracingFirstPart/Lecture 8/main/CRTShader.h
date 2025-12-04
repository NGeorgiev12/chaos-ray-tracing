#pragma once
#include "CRTRayTriangle.h"
#include "CRTScene.h"
#include "CRTColor.h"

class CRTShader
{
public:

	static CRTColor shade(const CRTVector& hitPoint,
		const CRTriangle& triangle, const CRTScene& scene);
};

