#include "CRTRenderer.h"

void CRTRenderer::renderTriangleToPPM(std::ofstream& ppmFs, const Grid& grid, const CRTriangle& triangle, const CRTCamera& camera)
{
	const CRTVector& normal = triangle.normalVector();
	const CRTVector& v0 = triangle.getVertices()[0];
	const CRTVector& cameraOrigin = camera.getPosition();
	const CRTMatrix& rotationMatrix = camera.getRotationMatrix();

	for (int i = 0; i < grid.imageHeight; i++) {

		for (int j = 0; j < grid.imageWidth; j++) {

			Pixel currentPixel{ j, i };
			CRTColor pixelColor;
			CRTRay cameraRay(cameraOrigin, grid, currentPixel, rotationMatrix);
			const CRTVector& rayDir = cameraRay.getRayDirection();

			float dotNormalCameraRay = dotProduct(normal, rayDir);
			float dotNormalToVertex0 = dotProduct(normal, v0 - cameraOrigin);

			if (areEqualFloats(dotNormalCameraRay, 0.f)) {
				continue;
			}


			float t = dotNormalToVertex0 / dotNormalCameraRay;

			if (t < 0) {
				continue;
			}

			CRTVector intersectionPoint = cameraOrigin + t * rayDir;

			if (isPointInsideTriangle(intersectionPoint, triangle)) {
				pixelColor.setGreen(255);
				pixelColor.setBlue(255);
			}

			ppmFs << pixelColor << "\t";
		}
		ppmFs << "\n";
	}
}

bool CRTRenderer::isPointInsideTriangle(const CRTVector& p, const CRTriangle& triangle)
{
	const CRTVector& normal = triangle.normalVector();
	const CRTVector& v0 = triangle.getVertices()[0];
	const CRTVector& v1 = triangle.getVertices()[1];
	const CRTVector& v2 = triangle.getVertices()[2];

	CRTVector edge0 = v1 - v0;
	CRTVector edge1 = v2 - v1;
	CRTVector edge2 = v0 - v2;

	CRTVector toP0 = p - v0;
	CRTVector toP1 = p - v1;
	CRTVector toP2 = p - v2;

	bool checkE0 = dotProduct(normal, crossProduct(edge0, toP0)) > 0 ? true : false;
	bool checkE1 = dotProduct(normal, crossProduct(edge1, toP1)) > 0 ? true : false;
	bool checkE2 = dotProduct(normal, crossProduct(edge2, toP2)) > 0 ? true : false;

	return checkE0 && checkE1 && checkE2;
}
