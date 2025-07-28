#pragma once
#include "../scene/CRTScene.h"
#include "../math/CRTRay.h"
#include "../math/CRTVector.h"
#include "CRTRayTriangle.h"
#include "CRTAlbedo.h"

class CRTShader
{
public:

	static CRTVector shade(const CRTRay& ray, const CRTIntersectionResult& result,
		const CRTScene& scene, TraceType traceType);

private:

	static CRTVector shadeDiffuse(const CRTRay& ray, const CRTIntersectionResult& result,
		const CRTScene& scene, TraceType traceType);
	static CRTVector shadeReflection(const CRTRay& ray, const CRTIntersectionResult& data,
		const CRTScene& scene, TraceType traceType);
	static CRTVector shadeRefraction(const CRTRay& ray, const CRTIntersectionResult& data,
		const CRTScene& scene, TraceType traceType);
	static CRTVector shadeConstant(const CRTIntersectionResult& data, const CRTScene& scene);

	static CRTRay constuctRefractionRay(const CRTRay& inputRay, const CRTVector& normal,
		const CRTVector& hitPoint, float cosAlpha, float sinBetha);
	static CRTRay constructReflectionRay(const CRTRay& inputRay, const CRTVector& hitPoint,
		const CRTVector& normalVector);


};

