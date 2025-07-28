#include "CRTSettings.h"
#include <fstream>

CRTSettings::CRTSettings() : backgroundColor(CRTVector(0.0f, 0.0f, 0.0f)), imageResolution({ 0, 0 })
{
}

CRTSettings::CRTSettings(const CRTVector& backgroundColor, const Grid& imageResolution, int bucketsCount)
	: backgroundColor(backgroundColor), imageResolution(imageResolution), bucketsCount(bucketsCount)
{
}

CRTSettings::CRTSettings(CRTVector&& backgroundColor, Grid&& imageResolution, int bucketsCount)
	: backgroundColor(std::move(backgroundColor)), imageResolution(std::move(imageResolution)), bucketsCount(bucketsCount)
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

int CRTSettings::getBucketsCount() const
{
	return bucketsCount;
}

void CRTSettings::setBackgroundColor(CRTVector&& backgroundColor)
{
	this->backgroundColor = std::move(backgroundColor);
}

void CRTSettings::setImageResolution(Grid&& resolution)
{
	this->imageResolution = std::move(resolution);
}

void CRTSettings::setBucketsCount(int bucketsCount)
{
	this->bucketsCount = bucketsCount;
}

void CRTSettings::print() const {
	std::cout << "Background Color: " << backgroundColor << std::endl;
	std::cout << "Image Resolution: " << imageResolution.imageWidth << "x" << imageResolution.imageHeight << std::endl;
}
