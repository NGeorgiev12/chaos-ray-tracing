#pragma once
#include "CRTriangle.h"
#include "CRTCamera.h"
#include "CRTRay.h"
#include "CRTColor.h"

class CRTRenderer
{
public:
	static void renderTriangleToPPM(std::ofstream& ppmFs, const Grid& grid,
		const CRTriangle& triangle, const CRTCamera& camera);

private:
	static bool isPointInsideTriangle(const CRTVector& p, const CRTriangle& triangle);
};

