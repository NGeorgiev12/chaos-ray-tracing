#include "CRTLight.h"

CRTLight::CRTLight(const CRTVector& position, int intensity)
	: position(position), intensity(intensity)
{
}

CRTLight::CRTLight(CRTVector&& position, int intensity)
	: position(std::move(position)), intensity(intensity)
{
}

const CRTVector& CRTLight::getPosition() const 
{
	return position;
}

int CRTLight::getIntensity() const
{
	return intensity;
}

void CRTLight::setPosition(CRTVector&& position)
{
	this->position = std::move(position);
}

void CRTLight::setIntensity(int intensity)
{
	this->intensity = intensity;
}
