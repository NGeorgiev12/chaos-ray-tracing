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
	CRTMaterial(const std::string& type, const CRTVector& albedo, bool smoothShading);
	CRTMaterial(const std::string& type, std::string&& textureName, bool smoothShading);
	CRTMaterial(const std::string& type, float ior, bool smoothShading);

	float getIOR() const;
	MaterialType getType() const;
	const std::string& getTextureName() const;
	const CRTVector& getAlbedo() const;
	bool isSmoothShading() const;

	void setType(std::string&& type);
	void setTextureName(std::string&& textureName);
	void setAlbedo(CRTVector&& albedo);
	void setIOR(float ior);
	void setSmoothShading(bool smoothShading);

private:
	
	MaterialType type = MaterialType::INVALID;
	std::string textureName;
	CRTVector albedo;
	float ior = 0.f;
	bool smoothShading = false;
};

