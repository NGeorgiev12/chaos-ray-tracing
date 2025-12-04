#pragma once
#include "CRTVector.h"

enum class MaterialType
{
	DIFFUSE,
	REFLECTIVE,
	INVALID
};

class CRTMaterial
{
public:

	CRTMaterial() = default;
	CRTMaterial(const std::string& type, const CRTVector& albedo, bool smoothShading);
	CRTMaterial(const std::string& type, CRTVector&& albedo, bool smoothShading);

	MaterialType getType() const;
	const CRTVector& getAlbedo() const;
	bool isSmoothShading() const;

private:
	
	MaterialType type = MaterialType::INVALID;
	CRTVector albedo;
	bool smoothShading = false;
};

