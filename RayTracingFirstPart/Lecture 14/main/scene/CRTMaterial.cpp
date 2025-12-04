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

CRTMaterial::CRTMaterial(const std::string& type, const CRTVector& albedo, bool smoothShading)
	: type(stringToEnum(type)), albedo(albedo), smoothShading(smoothShading)
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

const CRTVector& CRTMaterial::getAlbedo() const
{
	return albedo;
}

bool CRTMaterial::isSmoothShading() const
{
	return smoothShading;
}

void CRTMaterial::setType(std::string&& type)
{
	this->type = stringToEnum(std::move(type));
}

void CRTMaterial::setTextureName(std::string&& textureName)
{
	this->textureName = std::move(textureName);
}

void CRTMaterial::setAlbedo(CRTVector&& albedo)
{
	this->albedo = std::move(albedo);
}

void CRTMaterial::setIOR(float ior)
{
	this->ior = ior;
}

void CRTMaterial::setSmoothShading(bool smoothShading)
{
	this->smoothShading = smoothShading;
}
