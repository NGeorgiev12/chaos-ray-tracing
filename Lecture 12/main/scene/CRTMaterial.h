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
	CRTMaterial(const std::string& type, const std::string& textureName, bool smoothShading);
	CRTMaterial(const std::string& type, std::string&& textureName, bool smoothShading);
	CRTMaterial(const std::string& type, float ior, bool smoothShading);

	float getIOR() const;
	MaterialType getType() const;
	const std::string& getTextureName() const;
	bool isSmoothShading() const;

private:
	
	MaterialType type = MaterialType::INVALID;
	std::string textureName;
	float ior = 0.f;
	bool smoothShading = false;
};

