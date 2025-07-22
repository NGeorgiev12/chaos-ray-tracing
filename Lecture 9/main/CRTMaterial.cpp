#include "CRTMaterial.h"

static MaterialType stringToEnum(const std::string& type)
{
	if (type == "diffuse") {

		return MaterialType::DIFFUSE;

	} else if (type == "reflective") {

		return MaterialType::REFLECTIVE;
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
