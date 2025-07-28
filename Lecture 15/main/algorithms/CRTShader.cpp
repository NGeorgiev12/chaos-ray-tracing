#include "CRTShader.h"

static float getRandomPiAngle(float k)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(0.0f, k * PI_DEGREES);
	return dist(gen);
}

CRTVector CRTShader::shade(const CRTRay& ray, const CRTIntersectionResult& result,
	const CRTScene& scene, TraceType traceType)
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
			return shadeDiffuse(ray, result, scene, traceType);
		}
		case MaterialType::REFLECTIVE:
		{
			return shadeReflection(ray, result, scene, traceType);
		}
		case MaterialType::REFRACTIVE:
		{
			return shadeRefraction(ray, result, scene, traceType);
		}
		case MaterialType::CONSTANT:
		{
			return shadeConstant(result, scene);
		}
		default:
		{
			throw std::runtime_error("Unsupported material type encountered during ray tracing.");
		}
	}
}

CRTVector CRTShader::shadeDiffuse(const CRTRay& ray, const CRTIntersectionResult& result, const CRTScene& scene, TraceType traceType)
{
	int countGI = scene.getSettings().getGICount();
	CRTVector dirColor = shadeDirectIllumination(ray, result, scene, traceType);

	if (countGI == 0) 
	{
		return dirColor;
	}

	CRTVector giColor = shadeGlobalIllumination(ray, result, scene, traceType);
	float denom = 1.f / countGI;
	return (dirColor + giColor) * denom;
}

CRTVector CRTShader::shadeGlobalIllumination(const CRTRay& ray, const CRTIntersectionResult& result,
	const CRTScene& scene, TraceType traceType)
{
	const auto& material = scene.getMaterials()[result.materialIndex];
	const CRTriangle& triangle = result.triangle;
	const CRTVector& hitPoint = result.hitPoint;
	const CRTVector& normalVector = result.hitNormal;
	const CRTVector& rayDir = ray.getRayDirection();
	const CRTVector& rayOrigin = ray.getCameraOrigin();
	const CRTSettings& settings = scene.getSettings();

	CRTVector finalColor;

	CRTVector right = crossProduct(rayDir, normalVector);
	right.normalize();
	const CRTVector& up = normalVector;
	CRTVector forward = crossProduct(right, up);
	forward.normalize();

	CRTMatrix localHitMatrix(right, up, forward);

	for (int i = 0; i < settings.getGICount(); i++)
	{

		float randomAngle180 = getRandomPiAngle(1.f);
		float radians1 = degreesToRadians(randomAngle180);
		CRTVector unitRandXY(cosf(radians1), sinf(radians1), 0.f);

		float randomAngle360 = getRandomPiAngle(2.f);
		float radians2 = degreesToRadians(randomAngle360);
		CRTMatrix rotationY = CRTMatrix::rotationY(radians2);

		unitRandXY = unitRandXY * rotationY;

		CRTVector diffReflRayDir = unitRandXY * localHitMatrix;
		CRTVector diffReflRayOrigin = hitPoint + normalVector * SHADOW_BIAS;

		CRTRay diffReflRay(diffReflRayOrigin, diffReflRayDir, RayType::REFLECTION, ray.getPathDepth() + 1);
		CRTIntersectionResult reflResult = CRTRayTriangle::traceRay(diffReflRay, scene, traceType);

		finalColor += shade(diffReflRay, reflResult, scene, traceType);
	}

	return finalColor;
}

CRTVector CRTShader::shadeDirectIllumination(const CRTRay & ray, const CRTIntersectionResult & result,
	const CRTScene & scene, TraceType traceType)
{
	const CRTriangle& triangle = result.triangle;
	const CRTVector& hitPoint = result.hitPoint;
	const std::vector<CRTLight>& lightSources = scene.getLights();
	const CRTVector& normalVector = result.hitNormal;
	CRTVector albedo = CRTAlbedo::albedo(scene, result);

	CRTVector finalColorVec;

	for (const auto& light : lightSources) {

		CRTVector lightContribution;

		CRTVector lightDir = light.getPosition() - hitPoint;
		float sphereRadius = lightDir.getLength();
		float maxT = lightDir.getLength() - SHADOW_BIAS;
		lightDir.normalize();

		float cosineLaw = std::max(0.0f, dotProduct(lightDir, normalVector));
		int intensity = light.getIntensity();

		float sphereArea = 4.0f * PI * sphereRadius * sphereRadius;
		CRTVector shadowRayOrigin = hitPoint + normalVector * SHADOW_BIAS;

		CRTRay shadowRay(shadowRayOrigin, lightDir, RayType::SHADOW, ray.getPathDepth() + 1);

		CRTIntersectionResult shadowResult = CRTRayTriangle::traceRay(shadowRay, scene, traceType, maxT);

		if (shadowResult.hit == true) {

			const CRTMaterial& hitMat = scene.getMaterials()[shadowResult.materialIndex];

			if (hitMat.getType() != MaterialType::REFRACTIVE) {
				continue; 
			}
			
		}

		lightContribution = intensity / sphereArea * albedo * cosineLaw;
		finalColorVec += lightContribution;
	}

	return finalColorVec;
}

CRTVector CRTShader::shadeReflection(const CRTRay& ray, const CRTIntersectionResult& data,
	const CRTScene& scene, TraceType traceType)
{
	const CRTVector& normal = data.hitNormal;

	CRTVector rayDir = ray.getRayDirection();
	rayDir.normalize();

	CRTRay reflectionRay = constructReflectionRay(ray, data.hitPoint, normal);

	CRTIntersectionResult reflectionResult = CRTRayTriangle::traceRay(reflectionRay, scene, traceType);

	CRTVector finalColor = shade(reflectionRay, reflectionResult, scene, traceType);

	const CRTVector& albedo = CRTAlbedo::albedo(scene, data);
	finalColor = CRTVector::multiplyColors(finalColor, albedo);

	return finalColor;
}

CRTVector CRTShader::shadeRefraction(const CRTRay& ray, const CRTIntersectionResult& data,
	const CRTScene& scene, TraceType traceType)
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
		dotRayNormal *= -1.f;
	}

	float cosAlpha = -dotRayNormal;
	float sinAlpha = sqrtf(1.0f - cosAlpha * cosAlpha);
	float ethaRatio = ethaRefractive / ethaIncident;

	CRTRay reflectionRay = constructReflectionRay(ray, hitPoint, normal);
	CRTIntersectionResult reflectionResult = CRTRayTriangle::traceRay(reflectionRay, scene, traceType);

	if (sinAlpha >= ethaRatio) {
		// Total internal reflection
		return shade(reflectionRay, reflectionResult, scene, traceType);
	}

	float sinBetha = (sinAlpha * ethaIncident) / ethaRefractive;

	CRTRay refractedRay = constuctRefractionRay(ray, normal, hitPoint, cosAlpha, sinBetha);
	CRTIntersectionResult refractionResult = CRTRayTriangle::traceRay(refractedRay, scene, traceType);
	
	float fresnelCoefficient = fresnelFormula(dotRayNormal);

	return fresnelCoefficient * shade(reflectionRay, reflectionResult, scene, traceType) + 
		(1.0f - fresnelCoefficient) * shade(refractedRay, refractionResult, scene, traceType);
}

CRTVector CRTShader::shadeConstant(const CRTIntersectionResult& data, const CRTScene& scene)
{
	return CRTAlbedo::albedo(scene, data);
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
