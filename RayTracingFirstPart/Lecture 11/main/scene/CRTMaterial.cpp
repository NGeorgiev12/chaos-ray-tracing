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

CRTMaterial::CRTMaterial(const std::string& type, const CRTVector& albedo, bool smoothShading)
	: type(stringToEnum(type)), albedo(albedo), smoothShading(smoothShading)
{
}

CRTMaterial::CRTMaterial(const std::string& type, CRTVector&& albedo, bool smoothShading)
	: type(stringToEnum(type)), albedo(std::move(albedo)), smoothShading(smoothShading)
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

const CRTVector& CRTMaterial::getAlbedo() const
{
	return albedo;
}

bool CRTMaterial::isSmoothShading() const
{
	return smoothShading;
}
