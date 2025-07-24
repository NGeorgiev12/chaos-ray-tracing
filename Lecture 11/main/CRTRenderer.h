#pragma once
#include "math/CRTColor.h"
#include "scene/CRTScene.h"
#include "math/CRTriangle.h"
#include "math/CRTRay.h"
#include "algorithms/CRTShader.h"
#include "algorithms/CRTRayTriangle.h"
#include "algorithms/CRTReflector.h"
#include "algorithms/CRTRefracter.h"

class CRTRenderer
{
public:

	CRTRenderer(const std::string& sceneFileName);

	void renderScene(const std::string& outputFileName);
	//void renderAnimation(const std::string& outputFileName);

private:

	CRTScene scene;

	void writeHeader(std::ofstream& ofs, const Grid& grid);
	CRTVector traceRay(const CRTRay& ray);
};

