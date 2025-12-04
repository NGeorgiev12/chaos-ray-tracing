#pragma once
#include "../scene/CRTScene.h"
#include "../math/CRTriangle.h"
#include "../math/CRTRay.h"

struct CRTIntersectionResult
{
    bool hit = false;
    float t = std::numeric_limits<float>::max();
    CRTriangle triangle;
    CRTVector hitPoint;
    CRTVector hitNormal;
    std::vector<int> vertexIndices;
	int materialIndex = -1;
    int textureIndex = -1;
    BarycentricCoordinates baryCoords;
    CRTVector pUV;
};

class CRTRayTriangle
{
public:

    static CRTIntersectionResult traceRay(const CRTRay& ray, const CRTScene& scene,
        float maxT = std::numeric_limits<float>::infinity());

private:

    static bool isPointInsideTriangle(const CRTVector& p, const CRTriangle& triangle);
	static BarycentricCoordinates computeBarycentricCoordinates(const CRTMesh& mesh,
        const CRTIntersectionResult& result);
    static void calculateIntersectionData(CRTIntersectionResult& result, const CRTMesh& mesh,
         const std::vector<CRTMaterial>& materials);
};

