#include "CRTCamera.h"
using namespace Helper;

CRTCamera::CRTCamera() : position(0.f, 0.f, 0.f), rotationMatrix(CRTMatrix::identity())
{
}

CRTCamera::CRTCamera(const CRTVector& position) : position(position), rotationMatrix(CRTMatrix::identity())
{
}

void CRTCamera::move(CameraMovement movement, float distance)
{
	switch (movement)
	{
	case CameraMovement::DOLLY:
		moveDolly(distance);
		break;
	case CameraMovement::TRUCK:
		moveTruck(distance);
		break;
	case CameraMovement::PEDESTAL:
		movePedestal(distance);
		break;
	default:
		break;
	}
}

void CRTCamera::rotate(CameraRotation rotation, float angle)
{
	float radians = degreesToRadians(angle);

	switch (rotation)
	{
	case CameraRotation::PAN:
		rotatePan(radians);
		break;
	case CameraRotation::TILT:
		rotateTilt(radians);
		break;
	case CameraRotation::ROLL:
		rotateRoll(radians);
		break;
	default:
		break;
	}
}

void CRTCamera::executeMovementSequence(const std::vector<std::pair<CameraMovement, float>>& movements,
	const std::vector<std::pair<CameraRotation, float>>& rotations)
{

	for (const auto& rotationPair : rotations) {
		rotate(rotationPair.first, rotationPair.second);
	}

	for (const auto& movementPair : movements) {
		move(movementPair.first, movementPair.second);
	}
}

const CRTVector& CRTCamera::getPosition() const
{
	return position;
}

const CRTMatrix& CRTCamera::getRotationMatrix() const
{
	return rotationMatrix;
}

void CRTCamera::moveDolly(float distance)
{
	CRTVector initialVec(0.f, 0.f, 1.f);
	CRTVector moveDirInWorldSpace = initialVec * rotationMatrix;
	position += (moveDirInWorldSpace * distance);
}

void CRTCamera::moveTruck(float distance)
{
	CRTVector initialVec(1.f, 0.f, 0.f);
	CRTVector moveDirInWorldSpace = initialVec * rotationMatrix;
	position += (moveDirInWorldSpace * distance);
}

void CRTCamera::movePedestal(float distance)
{
	CRTVector initialVec(0.f, 1.f, 0.f);
	CRTVector moveDirInWorldSpace = initialVec * rotationMatrix;
	position += (moveDirInWorldSpace * distance);
}

void CRTCamera::rotatePan(float radians)
{
	rotationMatrix = rotationMatrix * CRTMatrix::rotationY(radians);
}

void CRTCamera::rotateTilt(float radians)
{
	rotationMatrix = rotationMatrix * CRTMatrix::rotationX(radians);
}

void CRTCamera::rotateRoll(float radians)
{
	rotationMatrix = rotationMatrix * CRTMatrix::rotationZ(radians);
}
