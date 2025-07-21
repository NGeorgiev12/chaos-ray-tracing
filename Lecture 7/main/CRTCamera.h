#pragma once
#include "CRTVector.h"

enum class CameraMovement {
    DOLLY,    
    TRUCK,    
    PEDESTAL, 
};

enum class CameraRotation {
	PAN,      
	TILT,     
	ROLL      
};

class CRTCamera
{
public:

    CRTCamera();
	CRTCamera(const CRTVector& position, const CRTMatrix& rotationMtx = CRTMatrix::identity());

	void move(CameraMovement movement, float distance);
	void rotate(CameraRotation rotation, float angle);

	void executeMovementSequence(const std::vector<std::pair<CameraMovement, float>>& movements,
		const std::vector<std::pair<CameraRotation, float>>& rotations);

	const CRTVector& getPosition() const;
	const CRTMatrix& getRotationMatrix() const;

private:

	CRTMatrix rotationMatrix;
	CRTVector position;

	void moveDolly(float distance);
	void moveTruck(float distance);
	void movePedestal(float distance);

	void rotatePan(float radians);
	void rotateTilt(float radians);
	void rotateRoll(float radians);
};