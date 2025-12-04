#pragma once
#include <stack>
#include "../scene/CRTScene.h"
#include "../math/CRTriangle.h"
#include "../math/CRTRay.h"
#include "../image/CRTAxisAlignedBB.h"

enum class TraceType
{
    TRIANGLE,
    AABB,
    KD,
    INVALID
};

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
        TraceType type, float maxT = std::numeric_limits<float>::infinity());

private:

    static CRTIntersectionResult traceRayTriangle(const CRTRay& ray, const CRTScene& scene,
        float maxT = std::numeric_limits<float>::infinity());

    static CRTIntersectionResult traceRayAABB(const CRTRay& ray, const CRTScene& scene,
        float maxT = std::numeric_limits<float>::infinity());

    static CRTIntersectionResult traceRayKD(const CRTRay& ray, const CRTScene& scene,
        float maxT = std::numeric_limits<float>::infinity());

    static bool isPointInsideTriangle(const CRTVector& p, const CRTriangle& triangle);
    static bool intersectsRayAABB(const CRTRay& ray, const CRTAxisAlignedBB& box);
    static bool intersectSingleTriangle(
        const CRTRay& ray,
        const CRTriangle& triangle,
        const CRTMesh& mesh,
        const CRTScene& scene,
        float maxT,
        CRTIntersectionResult& result);

    static void updateIntersectionData(const CRTRay& ray, const CRTMesh& mesh, CRTIntersectionResult& result,
        const CRTScene& scene, float maxT);

	static BarycentricCoordinates computeBarycentricCoordinates(const CRTMesh& mesh,
        const CRTIntersectionResult& result);

    static void calculateIntersectionData(CRTIntersectionResult& result, const CRTMesh& mesh,
         const std::vector<CRTMaterial>& materials);
};

