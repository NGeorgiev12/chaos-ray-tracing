#include "CameraRay.h"

CRTVector CameraRay::generateCameraRay(const Grid& grid, int curWidth, int curHeight, float zDir)
{
    float centreX = curWidth + 0.5f;
    float centreY = curHeight + 0.5f;

    centreX /= grid.imageWidth;
    centreY /= grid.imageHeight;

    centreX = (2.0f * centreX - 1.0f);
    centreY = (1.0f - 2.0f * centreY);

    centreX *= (float)(grid.imageWidth / grid.imageHeight);

    CRTVector rayDir(centreX, centreY, zDir);
    rayDir.normalize();
    return rayDir;
}

