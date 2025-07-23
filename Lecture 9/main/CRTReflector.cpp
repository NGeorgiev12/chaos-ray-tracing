#include "CRTReflector.h"

CRTRay CRTReflector::reflectRay(const CRTRay& inputRay, const CRTVector& hitPoint, const CRTVector& normalVector)
{
    const CRTVector& rayDir = inputRay.getRayDirection();
    CRTVector reflectionDir = rayDir - 2 * dotProduct(rayDir, normalVector) * normalVector;
    reflectionDir.normalize();

    CRTVector reflectedOrigin = hitPoint + normalVector * SHADOW_BIAS;

    return CRTRay(reflectedOrigin, reflectionDir);
}

CRTVector CRTReflector::handleReflection(const CRTRay& ray, const CRTIntersectionResult& result,
	const CRTScene& scene, int depth)
{
	const CRTVector& backgroundColor = scene.getSettings().getBackgroundColor();
	const CRTMaterial& material = scene.getMaterials()[result.materialIndex];
	const CRTVector& albedo = material.getAlbedo();

	if (depth >= MAX_RAY_DEPTH) {
		return CRTVector::multiplyColors(backgroundColor, albedo);
	}

	CRTRay reflectedRay = CRTReflector::reflectRay(ray, result.hitPoint, result.hitNormal);

	CRTIntersectionResult reflectionResult = CRTRayTriangle::intersectsRayTriangle(reflectedRay, scene);
	const CRTMaterial& reflectedMaterial = scene.getMaterials()[reflectionResult.materialIndex];

	if (reflectionResult.hit == false) {
		return CRTVector::multiplyColors(backgroundColor, albedo);
	}

	if (reflectedMaterial.getType() == MaterialType::DIFFUSE) {

		CRTVector diffuseColor = CRTShader::shade(reflectionResult, scene);
		return CRTVector::multiplyColors(diffuseColor, albedo);
	}

	return handleReflection(reflectedRay, reflectionResult, scene, depth + 1);
}
