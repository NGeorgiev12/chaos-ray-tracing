#include "CRTReflector.h"

CRTRay CRTReflector::reflectRay(const CRTRay& inputRay, const CRTVector& hitPoint, const CRTVector& normalVector)
{
    const CRTVector& rayDir = inputRay.getRayDirection();
    CRTVector reflectionDir = rayDir - 2 * dotProduct(rayDir, normalVector) * normalVector;
    reflectionDir.normalize();

    CRTVector reflectedOrigin = hitPoint + normalVector * SHADOW_BIAS;

    return CRTRay(reflectedOrigin, reflectionDir);
}
