#pragma once
#include "CRTScene.h"
#include "CRTriangle.h"
#include "CRTRay.h"

struct CRTIntersectionResult
{
    bool hit = false;
    float t = std::numeric_limits<float>::max();
    CRTriangle triangle;
    CRTVector hitPoint;
};

class CRTRayTriangle
{
public:

    static CRTIntersectionResult intersectsRayTriangle(const CRTRay& ray, const CRTScene& scene);

private:

    static bool isPointInsideTriangle(const CRTVector& p, const CRTriangle& triangle);
};

