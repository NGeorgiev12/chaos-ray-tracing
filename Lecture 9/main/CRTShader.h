#pragma once
#include "CRTRayTriangle.h"
#include "CRTScene.h"
#include "CRTColor.h"

class CRTShader
{
public:

	static CRTColor shade(const CRTIntersectionResult& result, const CRTScene& scene);
};

