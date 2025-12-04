#include "CRTShader.h"

// hardcoded albedo and shadow bias
const CRTVector ALBEDO(0.0f, 1.0f, 0.0f);
const float SHADOW_BIAS = 1e-4f;

CRTColor CRTShader::shade(const CRTVector& hitPoint,
	const CRTriangle& triangle, const CRTScene& scene)
{
	const std::vector<CRTLight>& lightSources = scene.getLights();
	const CRTVector& normalVector = triangle.normalVector();
	CRTVector finalColor;
    
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

		//return CRTColor(0, 0, 255);
		lightContribution = intensity / sphereArea * ALBEDO * cosineLaw;
		//std::cout << "Light contribution: " << lightContribution << std::endl;
		finalColor += lightContribution;
	}

	//std::cout << "Final color: " << finalColor << std::endl;
	return CRTColor(finalColor);
}
