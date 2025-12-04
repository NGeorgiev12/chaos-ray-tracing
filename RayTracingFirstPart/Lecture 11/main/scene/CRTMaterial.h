#pragma once
#include "../math/CRTVector.h"

enum class MaterialType
{
	DIFFUSE,
	REFLECTIVE,
	REFRACTIVE,
	CONSTANT,
	INVALID
};

class CRTMaterial
{
public:

	CRTMaterial() = default;
	CRTMaterial(const std::string& type, const CRTVector& albedo, bool smoothShading);
	CRTMaterial(const std::string& type, CRTVector&& albedo, bool smoothShading);
	CRTMaterial(const std::string& type, float ior, bool smoothShading);

	float getIOR() const;
	MaterialType getType() const;
	const CRTVector& getAlbedo() const;
	bool isSmoothShading() const;

private:
	
	MaterialType type = MaterialType::INVALID;
	CRTVector albedo;
	float ior = 0.f;
	bool smoothShading = false;
};

