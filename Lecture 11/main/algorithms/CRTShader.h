#pragma once
#include "CRTRayTriangle.h"
#include "../scene/CRTScene.h"
#include "../math/CRTColor.h"

class CRTShader
{
public:

	static CRTVector shade(const CRTIntersectionResult& result, const CRTScene& scene);
};

