#include "CRTMaterial.h"

static MaterialType stringToEnum(const std::string& type)
{
	if (type == "diffuse") {

		return MaterialType::DIFFUSE;

	} else if (type == "reflective") {

		return MaterialType::REFLECTIVE;

	} else if (type == "refractive") {

		return MaterialType::REFRACTIVE;

	} else if (type == "constant") {

		return MaterialType::CONSTANT;
	}
		
	return MaterialType::INVALID;
}

CRTMaterial::CRTMaterial(const std::string& type, const std::string& textureName, bool smoothShading)
	: type(stringToEnum(type)), textureName(textureName), smoothShading(smoothShading)
{
}

CRTMaterial::CRTMaterial(const std::string& type, std::string&& textureName, bool smoothShading)
	: type(stringToEnum(type)), textureName(std::move(textureName)), smoothShading(smoothShading)
{
}
	
CRTMaterial::CRTMaterial(const std::string& type, float ior, bool smoothShading)
	: type(stringToEnum(type)), ior(ior), smoothShading(smoothShading)
{
}

float CRTMaterial::getIOR() const
{
	return ior;
}

MaterialType CRTMaterial::getType() const
{
	return type;
}

const std::string& CRTMaterial::getTextureName() const
{
	return textureName;
}

bool CRTMaterial::isSmoothShading() const
{
	return smoothShading;
}
