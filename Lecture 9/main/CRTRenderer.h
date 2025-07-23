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
	CRTColor traceRay(const CRTRay& ray, int depth = 0);
	CRTColor handleDiffuse(const CRTIntersectionResult& result);
	CRTColor handleReflection(const CRTRay& ray, const CRTIntersectionResult& result, int depth);
};

