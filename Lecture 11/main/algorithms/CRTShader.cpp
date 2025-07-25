#include "CRTShader.h"

CRTVector CRTShader::shade(const CRTRay& ray, const CRTIntersectionResult& result, const CRTScene& scene)
{

	if (ray.getPathDepth() >= MAX_RAY_DEPTH || !result.hit)
	{
		return scene.getSettings().getBackgroundColor();
	}

	const CRTMaterial& material = scene.getMaterials()[result.materialIndex];

	switch (material.getType())
	{
		case MaterialType::DIFFUSE:
		{
			return shadeDiffuse(ray, result, scene);
		}
		case MaterialType::REFLECTIVE:
		{
			return shadeReflection(ray, result, scene);
		}
		case MaterialType::REFRACTIVE:
		{
			return shadeRefraction(ray, result, scene);
		}
		case MaterialType::CONSTANT:
		{
			return shadeConstant(ray, result, scene);
		}
		default:
		{
			throw std::runtime_error("Unsupported material type encountered during ray tracing.");
		}
	}

}

CRTVector CRTShader::shadeDiffuse(const CRTRay & ray, const CRTIntersectionResult & result, const CRTScene & scene)
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

		CRTRay shadowRay(shadowRayOrigin, lightDir, RayType::SHADOW, ray.getPathDepth() + 1);

		float maxT = lightDir.getLength() - SHADOW_BIAS;

		CRTIntersectionResult shadowResult = CRTRayTriangle::traceRay(shadowRay, scene, maxT);

		if (shadowResult.hit == true) {
			continue;
		}

		lightContribution = intensity / sphereArea * albedo * cosineLaw;
		finalColorVec += lightContribution;
	}

	return finalColorVec;
}

CRTVector CRTShader::shadeReflection(const CRTRay& ray, const CRTIntersectionResult& data, const CRTScene& scene)
{
	const CRTVector& normal = data.hitNormal;

	CRTVector rayDir = ray.getRayDirection();
	rayDir.normalize();

	CRTRay reflectionRay = constructReflectionRay(ray, data.hitPoint, normal);

	CRTIntersectionResult reflectionResult = CRTRayTriangle::traceRay(reflectionRay, scene);

	CRTVector finalColor = shade(reflectionRay, reflectionResult, scene);

	const CRTVector& albedo = scene.getMaterials()[data.materialIndex].getAlbedo();
	finalColor = CRTVector::multiplyColors(finalColor, albedo);

	return finalColor;
}

CRTVector CRTShader::shadeRefraction(const CRTRay& ray, const CRTIntersectionResult& data, const CRTScene& scene)
{
	float ethaIncident = IOR_AIR;
	float ethaRefractive = scene.getMaterials()[data.materialIndex].getIOR();

	CRTVector normal = data.hitNormal; // N
	const CRTVector& hitPoint = data.hitPoint;
	const CRTVector& rayDirection = ray.getRayDirection(); // I
	float dotRayNormal = dotProduct(rayDirection, normal);

	if (dotRayNormal > 0) {
		std::swap(ethaIncident, ethaRefractive);
		normal *= -1.0f;
	}

	float cosAlpha = -dotRayNormal;
	float sinAlpha = sqrtf(1.0f - cosAlpha * cosAlpha);
	float ethaRatio = ethaRefractive / ethaIncident;

	CRTRay reflectionRay = constructReflectionRay(ray, hitPoint, normal);
	CRTIntersectionResult reflectionResult = CRTRayTriangle::traceRay(reflectionRay, scene);

	if (sinAlpha >= ethaRatio) {
		// Total internal reflection
		return shade(reflectionRay, reflectionResult, scene);
	}

	float sinBetha = (sinAlpha * ethaIncident) / ethaRefractive;

	CRTRay refractedRay = constuctRefractionRay(ray, normal, hitPoint, cosAlpha, sinBetha);
	CRTIntersectionResult refractionResult = CRTRayTriangle::traceRay(refractedRay, scene);
	
	float fresnelCoefficient = fresnelFormula(dotRayNormal);

	return fresnelCoefficient * shade(reflectionRay, reflectionResult, scene) + 
		(1.0f - fresnelCoefficient) * shade(refractedRay, refractionResult, scene);
}

CRTVector CRTShader::shadeConstant(const CRTRay& ray, const CRTIntersectionResult& data, const CRTScene& scene)
{
	return scene.getMaterials()[data.materialIndex].getAlbedo();
}

CRTRay CRTShader::constuctRefractionRay(const CRTRay& inputRay, const CRTVector& normal,
	const CRTVector& hitPoint, float cosAlpha, float sinBetha)
{
	const CRTVector& incident = inputRay.getRayDirection();
	float cosBetha = sqrtf(1.0f - sinBetha * sinBetha);

	CRTVector C = incident + cosAlpha * normal; // C = I + cos(I, N) * N
	C.normalize();

	CRTVector B = C * sinBetha; // B = C * sin(Betha)
	CRTVector A = cosBetha * (normal * -1.f); // A = cos(Betha) * (-N)

	CRTVector refractedRayDir = A + B;
	CRTVector refractedRayOrigin = hitPoint + (normal * -1.f) * REFRACTION_BIAS;

	return CRTRay(refractedRayOrigin, refractedRayDir, RayType::REFRACTION, inputRay.getPathDepth() + 1);
}

CRTRay CRTShader::constructReflectionRay(const CRTRay& inputRay, const CRTVector& hitPoint, const CRTVector& normalVector)
{
	const CRTVector& rayDir = inputRay.getRayDirection();
	CRTVector reflectionDir = rayDir - 2 * dotProduct(rayDir, normalVector) * normalVector;
	reflectionDir.normalize();

	CRTVector reflectedOrigin = hitPoint + normalVector * SHADOW_BIAS;

	return CRTRay(reflectedOrigin, reflectionDir, RayType::REFLECTION, inputRay.getPathDepth() + 1);
}
