#include "CRTShader.h"

CRTColor CRTShader::shade(const CRTIntersectionResult& result, const CRTScene& scene)
{
	const CRTriangle& triangle = result.triangle;
	const CRTVector& hitPoint = result.hitPoint;
	const std::vector<CRTLight>& lightSources = scene.getLights();
	const CRTVector& normalVector = result.hitNormal;
	const CRTVector& albedo = scene.getMaterials()[result.materialIndex].getAlbedo();

	CRTVector finalColorVec;
    
	for (const auto& light : lightSources) {
		
		CRTVector lightContribution;

		CRTVector lightDir = light.getPosition() - hitPoint;
		float sphereRadius = lightDir.getLength();
		lightDir.normalize();

		float cosineLaw = std::max(0.0f, dotProduct(lightDir, normalVector));
		int intensity = light.getIntensity();

		float sphereArea = 4.0f * PI * sphereRadius * sphereRadius;
		CRTVector shadowRayOrigin = hitPoint + normalVector * SHADOW_BIAS;
		CRTRay shadowRay(shadowRayOrigin, lightDir);

		CRTIntersectionResult shadowResult = CRTRayTriangle::intersectsRayTriangle(shadowRay, scene);

		if (shadowResult.hit == true) {
			continue;
		}

		lightContribution = intensity / sphereArea * albedo * cosineLaw;
		finalColorVec += lightContribution;
	}

	return CRTColor(finalColorVec);
}
