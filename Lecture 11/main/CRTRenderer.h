#pragma once
#include "math/CRTColor.h"
#include "scene/CRTScene.h"
#include "math/CRTriangle.h"
#include "math/CRTRay.h"
#include "algorithms/CRTShader.h"
#include "algorithms/CRTRayTriangle.h"

class CRTRenderer
{
public:

	CRTRenderer(const std::string& sceneFileName);

	void renderScene(const std::string& outputFileName);
	//void renderAnimation(const std::string& outputFileName);

private:

	CRTScene scene;

	void writeHeader(std::ofstream& ofs, const Grid& grid);
	CRTIntersectionResult traceRay(const CRTRay& ray, float maxT = std::numeric_limits<float>::infinity());
};

