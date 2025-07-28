#pragma once
#include <thread>
#include <mutex>
#include <queue>
#include "../image/CRTColor.h"
#include "../scene/CRTScene.h"
#include "../math/CRTriangle.h"
#include "../math/CRTRay.h"
#include "../image/CRTImageBuffer.h"
#include "CRTShader.h"
#include "CRTRayTriangle.h"

enum class RenderType
{
	LINEAR,
	REGION,
	BUCKETS,
	INVALID
};

class CRTRenderer
{
public:

	CRTRenderer(const std::string& sceneFileName, RenderType renderType, TraceType traceType);

	void renderScene(const std::string& outputFileName);
	//void renderAnimation(const std::string& outputFileName);

private:

	CRTScene scene;
	RenderType renderType;
	TraceType traceType;

	CRTImageBuffer renderLinear(const std::string& outputFileName, const Grid& grid);
	CRTImageBuffer renderRegions(const std::string& outputFileName, const Grid& grid);
	void renderRegion(CRTImageBuffer& buffer, const Interval& region, const Grid& grid);
	CRTImageBuffer renderBuckets(const std::string& outputFileName, const Grid& grid);
};

