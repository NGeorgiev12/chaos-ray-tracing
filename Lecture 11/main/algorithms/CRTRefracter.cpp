#include "CRTRefracter.h"

CRTVector CRTRefracter::handleRefraction(const CRTRay& ray, const CRTIntersectionResult& result, const CRTScene& scene, int depth)
{
	const CRTVector& backgroundColor = scene.getSettings().getBackgroundColor();

	if (depth >= MAX_RAY_DEPTH) {
		return backgroundColor;
	}

	float ethaIncident = IOR_AIR;
	float ethaRefractive = scene.getMaterials()[result.materialIndex].getIOR();

	CRTVector normal = result.triangle.normalVector(); // N
	const CRTVector& hitPoint = result.hitPoint;
	const CRTVector& rayDirection = ray.getRayDirection(); // I
	float dotRayNormal = dotProduct(rayDirection, normal);

	if (dotRayNormal > 0) {
		std::swap(ethaIncident, ethaRefractive);
		normal *= -1.0f; 
	}

	float cosAlpha = -dotRayNormal;
	float sinAlpha = sqrtf(1.0f - cosAlpha * cosAlpha);
	float ethaRatio = ethaRefractive / ethaIncident;

	if (sinAlpha >= ethaRatio) {
		// Total internal reflection
		return CRTReflector::handleReflection(ray, result, scene, depth + 1); 
	}

	float sinBetha = (sinAlpha * ethaIncident) / ethaRefractive;

	CRTVector refractedRayDir = computeRefractedVector(rayDirection, normal, cosAlpha, sinBetha);
	
	CRTVector refractedRayOrigin = hitPoint + (normal * -1.f) * REFRACTION_BIAS;

	CRTRay refractedRay(refractedRayOrigin, refractedRayDir);
	CRTRay reflectedRay = CRTReflector::reflectRay(ray, result.hitPoint, normal);

	CRTIntersectionResult refractedResult = CRTRayTriangle::intersectsRayTriangle(refractedRay, scene);
	CRTIntersectionResult reflectedResult = CRTRayTriangle::intersectsRayTriangle(reflectedRay, scene);

	CRTVector reflectedColor = (reflectedResult.hit) ?
		CRTReflector::handleReflection(reflectedRay, reflectedResult, scene, depth + 1) : backgroundColor;

	CRTVector refractedColor = backgroundColor; // Default value
	/*CRTVector refractedColor = (refractedResult.hit) ? 
		CRTRefracter::handleRefraction(refractedRay, refractedResult, scene, depth + 1) : backgroundColor;*/
	if (refractedResult.hit) {

		const CRTMaterial& refractedMaterial = scene.getMaterials()[refractedResult.materialIndex];

		if (refractedMaterial.getType() == MaterialType::DIFFUSE) {
			refractedColor = CRTShader::shade(refractedResult, scene);
		}
		else if (refractedMaterial.getType() == MaterialType::CONSTANT) {
			refractedColor = refractedMaterial.getAlbedo();
		}
		else if (refractedMaterial.getType() == MaterialType::REFRACTIVE) {
			refractedColor = CRTRefracter::handleRefraction(refractedRay, refractedResult, scene, depth + 1);
		} 
		else {
			refractedColor = CRTReflector::handleReflection(refractedRay, refractedResult, scene, depth + 1);
		}
	}

	float fresnelCoefficient = fresnelFormula(dotRayNormal);

	return fresnelCoefficient * reflectedColor + (1.0f - fresnelCoefficient) * refractedColor;

}

CRTVector CRTRefracter::computeRefractedVector(const CRTVector& incident, const CRTVector& normal,
	 float cosAlpha, float sinBetha)
{
	float cosBetha = sqrtf(1.0f - sinBetha * sinBetha);

	CRTVector C = incident + cosAlpha * normal; // C = I + cos(I, N) * N
	C.normalize();

	CRTVector B = C * sinBetha; // B = C * sin(Betha)
	CRTVector A = cosBetha * (normal * -1.f); // A = cos(Betha) * (-N)

	return A + B;
}
