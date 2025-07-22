#pragma once
#include "CRTRay.h"

class CRTReflector
{
public:
	static CRTRay reflectRay(const CRTRay& inputRay, const CRTVector& hitPoint, const CRTVector& normalVector);
};

