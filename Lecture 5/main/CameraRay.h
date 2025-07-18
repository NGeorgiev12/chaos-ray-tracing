#pragma once
#include "CRTVector.h"

class CameraRay
{
public:
	static CRTVector generateCameraRay(const Grid& grid, int curWidth, int curHeight, float zDir);
};

