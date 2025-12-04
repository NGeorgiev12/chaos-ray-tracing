#include "CRTRay.h"

CRTRay CRTRay::generateCameraRay(const CRTCamera& camera, const Grid& grid,
	const Pixel& currentPixel, float angle)
{
	const CRTVector& cameraPosition = camera.getPosition();
	const CRTMatrix& rotationMatrix = camera.getRotationMatrix();

	float radians = degreesToRadians(angle / 2.0f);

	float centreX = currentPixel.x + 0.5f;
	float centreY = currentPixel.y + 0.5f;

	centreX /= grid.imageWidth;
	centreY /= grid.imageHeight;

	centreX = (2.0f * centreX - 1.0f);
	centreY = (1.0f - 2.0f * centreY);

	centreX *= (float)grid.imageWidth / grid.imageHeight;

	centreX *= 1 / tanf(radians);
	centreY *= 1 / tanf(radians);

	CRTVector rayDirection(centreX, centreY, INITIAL_CAMERA_Z_DIR);
	rayDirection = rayDirection * rotationMatrix;
	rayDirection.normalize();

	return CRTRay(cameraPosition, rayDirection, RayType::CAMERA, 0);
}

CRTRay::CRTRay(const CRTVector& cameraOrigin, const CRTVector& rayDirection, RayType rayType, int depth)
	: cameraOrigin(cameraOrigin), rayDirection(rayDirection), rayType(rayType), pathDepth(depth)
{
	this->rayDirection.normalize();
}

CRTRay::CRTRay(CRTVector&& cameraOrigin, CRTVector&& rayDirection, RayType rayType, int depth)
	: cameraOrigin(std::move(cameraOrigin)), rayDirection(std::move(rayDirection)), rayType(rayType), pathDepth(depth)
{
	this->rayDirection.normalize();
}

const CRTVector& CRTRay::getCameraOrigin() const
{
	return cameraOrigin;
}

const CRTVector& CRTRay::getRayDirection() const
{
	return rayDirection;
}

RayType CRTRay::getRayType() const
{
	return rayType;
}

int CRTRay::getPathDepth() const
{
	return pathDepth;	
}
