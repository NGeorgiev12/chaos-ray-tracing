#pragma once
#include "CRTRay.h"
#include "CRTScene.h"
#include "CRTRayTriangle.h"
#include "CRTShader.h"

class CRTReflector
{
public:
	static CRTRay reflectRay(const CRTRay& inputRay, const CRTVector& hitPoint, const CRTVector& normalVector);
	static CRTVector handleReflection(const CRTRay& ray, const CRTIntersectionResult& result,
		const CRTScene& scene, int depth = 0);
};

