#pragma once
#include "CRTColor.h"
#include "CRTScene.h"
#include "CRTriangle.h"
#include "CRTRay.h"

class CRTRenderer
{
public:

	CRTRenderer(const std::string& sceneFileName);

	void renderScene();
	//void renderAnimation(std::ofstream& ofs, const CRTScene& scene);

private:

	CRTScene scene;

	void writeHeader(std::ofstream& ofs, const Grid& grid);
	bool isPointInsideTriangle(const CRTVector& p, const CRTriangle& triangle);
	int intersectsRayTriangle(const CRTRay& ray, const CRTScene& scene, CRTColor& colour);
};

