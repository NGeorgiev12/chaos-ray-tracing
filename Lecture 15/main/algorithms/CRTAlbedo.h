#pragma once  
#include "../scene/CRTexture.h"  
#include "CRTRayTriangle.h"  

class CRTAlbedo  
{  
public:  
    static CRTVector albedo(const CRTScene& scene, const CRTIntersectionResult& result);  
};