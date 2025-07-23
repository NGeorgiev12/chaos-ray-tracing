#pragma once
#include "CRTColor.h"
#include "CRTScene.h"
#include "CRTriangle.h"
#include "CRTRay.h"
#include "CRTShader.h"
#include "CRTRayTriangle.h"
#include "CRTReflector.h"

class CRTRenderer
{
public:

	CRTRenderer(const std::string& sceneFileName);

	void renderScene(const std::string& outputFileName);
	//void renderAnimation(const std::string& outputFileName);

private:

	CRTScene scene;

	void writeHeader(std::ofstream& ofs, const Grid& grid);
	CRTVector traceRay(const CRTRay& ray, int depth = 0);
	CRTVector handleReflection(const CRTRay& ray, CRTIntersectionResult& result, int depth);
};

