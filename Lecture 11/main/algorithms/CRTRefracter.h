#pragma once
#include "../math/CRTVector.h"
#include "../math/CRTRay.h"
#include "../scene/CRTScene.h"
#include "CRTRayTriangle.h"
#include "CRTShader.h"
#include "CRTReflector.h"

class CRTRefracter
{
public:

	static CRTVector handleRefraction(const CRTRay& ray, const CRTIntersectionResult& result,
		const CRTScene& scene, int depth = 0);

private:
	static CRTVector computeRefractedVector(const CRTVector& incident, const CRTVector& normal,
		 float cosAlpha, float sinBetha);
};

