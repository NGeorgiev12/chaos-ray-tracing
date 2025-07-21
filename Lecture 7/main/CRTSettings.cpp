#include "CRTSettings.h"
#include <fstream>
#include "CRTVector.h"


CRTSettings::CRTSettings() : backgroundColor(CRTVector(0.0f, 0.0f, 0.0f)), imageResolution({ 0, 0 })
{
}

CRTSettings::CRTSettings(const CRTVector& backgroundColor, const Grid& imageResolution)
	: backgroundColor(backgroundColor), imageResolution(imageResolution)
{
}

CRTSettings::CRTSettings(CRTVector&& backgroundColor, Grid&& imageResolution)
	: backgroundColor(std::move(backgroundColor)), imageResolution(std::move(imageResolution))
{
}

const CRTVector& CRTSettings::getBackgroundColor() const
{
	return backgroundColor;
}

const Grid& CRTSettings::getImageResolution() const
{
	return imageResolution;
}
