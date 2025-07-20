#include <iostream>
#include "CRTCamera.h"
#include "CRTriangle.h"
#include "CRTRay.h"
#include "CRTColor.h"
#include "CRTRenderer.h"

//bool isPointInsideTriangle(const CRTVector& p, const CRTriangle& triangle) {
//
//	const CRTVector& normal = triangle.normalVector();
//	const CRTVector& v0 = triangle.getVertices()[0];
//	const CRTVector& v1 = triangle.getVertices()[1];
//	const CRTVector& v2 = triangle.getVertices()[2];
//
//	CRTVector edge0 = v1 - v0;
//	CRTVector edge1 = v2 - v1;
//	CRTVector edge2 = v0 - v2;
//
//	CRTVector toP0 = p - v0;
//	CRTVector toP1 = p - v1;
//	CRTVector toP2 = p - v2;
//
//	bool checkE0 = dotProduct(normal, crossProduct(edge0, toP0)) > 0 ? true : false;
//	bool checkE1 = dotProduct(normal, crossProduct(edge1, toP1)) > 0 ? true : false;
//	bool checkE2 = dotProduct(normal, crossProduct(edge2, toP2)) > 0 ? true : false;
//
//	return checkE0 && checkE1 && checkE2;
//}
//
//void renderTriangleToPPM(std::ofstream& ppmFs, const Grid& grid,
//	const CRTriangle& triangle, const CRTCamera& camera) {
//
//	const CRTVector& normal = triangle.normalVector();
//	const CRTVector& v0 = triangle.getVertices()[0];
//	const CRTVector& cameraOrigin = camera.getPosition();
//	const CRTMatrix& rotationMatrix = camera.getRotationMatrix();
//
//	for (int i = 0; i < grid.imageHeight; i++) {
//
//		for (int j = 0; j < grid.imageWidth; j++) {
//
//			Pixel currentPixel{ j, i };
//			CRTColor pixelColor;
//			CRTRay cameraRay(cameraOrigin, grid, currentPixel, rotationMatrix);
//			const CRTVector& rayDir = cameraRay.getRayDirection();
//			
//			float dotNormalCameraRay = dotProduct(normal, rayDir);
//			float dotNormalToVertex0 = dotProduct(normal, v0 - cameraOrigin);
//
//			if (areEqualFloats(dotNormalCameraRay, 0.f)) {
//				continue;
//			}
//
//			float t = dotNormalToVertex0 / dotNormalCameraRay;
//
//			if (t < 0) {
//				continue; 
//			}
//
//			CRTVector intersectionPoint = cameraOrigin + t * rayDir;
//
//			if (isPointInsideTriangle(intersectionPoint, triangle)) {
//				pixelColor.setGreen(255);
//				pixelColor.setBlue(255);
//			}
//
//			ppmFs << pixelColor << "\t";
//		}
//		ppmFs << "\n";
//	}
//}

void task1() {

	CRTVector cameraOrigin(0.0f, 0.0f, -1.0f);
	CRTCamera camera(cameraOrigin);
	camera.rotate(CameraRotation::PAN, 30);
}

void task2(const Grid& grid) {
	
	CRTVector v0(-1.75f, -1.75f, -3.f);
	CRTVector v1(1.75f, -1.75f, -3.f);
	CRTVector v2(0.f, 1.75f, -3.f);

	CRTriangle triangle(v0, v1, v2);
	CRTVector cameraOrigin(0.0f, 0.0f, -1.0f);
	CRTCamera camera(cameraOrigin);

	std::ofstream ppmFs("output_image.ppm", std::ios::out | std::ios::binary);
	ppmFs << "P3\n";
	ppmFs << grid.imageWidth << " " << grid.imageHeight << "\n";
	ppmFs << CRTColor::maxColorComponent << "\n";

	CRTRenderer::renderTriangleToPPM(ppmFs, grid, triangle, camera);
	ppmFs.close();
}

void task3(const Grid& grid) {

	CRTVector v0(-1.75f, -1.75f, -3.f);
	CRTVector v1(1.75f, -1.75f, -3.f);
	CRTVector v2(0.f, 1.75f, -3.f);
	CRTriangle triangle(v0, v1, v2);

	CRTCamera camera;
	std::ofstream ppmFs1("output_image3.ppm", std::ios::out | std::ios::binary);
	ppmFs1 << "P3\n";
	ppmFs1 << grid.imageWidth << " " << grid.imageHeight << "\n";
	ppmFs1 << CRTColor::maxColorComponent << "\n";

	CRTRenderer::renderTriangleToPPM(ppmFs1, grid, triangle, camera);

	camera.rotate(CameraRotation::PAN, 30);
	std::ofstream ppmFs2("output_image4.ppm", std::ios::out | std::ios::binary);
	ppmFs2 << "P3\n";
	ppmFs2 << grid.imageWidth << " " << grid.imageHeight << "\n";
	ppmFs2 << CRTColor::maxColorComponent << "\n";

	CRTRenderer::renderTriangleToPPM(ppmFs2, grid, triangle, camera);
	ppmFs1.close();
	ppmFs2.close();
}

void task4(const Grid& grid) {
	
	CRTVector v0(-1.75f, -1.75f, -3.f);
	CRTVector v1(1.75f, -1.75f, -3.f);
	CRTVector v2(0.f, 1.75f, -3.f);
	CRTriangle triangle(v0, v1, v2);

	CRTCamera camera;
	std::ofstream ppmFs1("before_movements_task4.ppm", std::ios::out | std::ios::binary);
	ppmFs1 << "P3\n";
	ppmFs1 << grid.imageWidth << " " << grid.imageHeight << "\n";
	ppmFs1 << CRTColor::maxColorComponent << "\n";
	CRTRenderer::renderTriangleToPPM(ppmFs1, grid, triangle, camera);

	std::vector<std::pair<CameraRotation, float>> rotations = {
		{CameraRotation::PAN, 30},   
		{CameraRotation::TILT, -15}
	};

	std::vector<std::pair<CameraMovement, float>> movements = {
		{CameraMovement::DOLLY, 2}, 
		{CameraMovement::TRUCK, 1} 
	};

	std::ofstream ppmFs2("after_movements_task4.ppm", std::ios::out | std::ios::binary);
	ppmFs2 << "P3\n";
	ppmFs2 << grid.imageWidth << " " << grid.imageHeight << "\n";
	ppmFs2 << CRTColor::maxColorComponent << "\n";
	camera.executeMovementSequence(movements, rotations);
	CRTRenderer::renderTriangleToPPM(ppmFs2, grid, triangle, camera);

}

int main() {
	
	Grid grid{ 400, 300 };
	task2(grid);
	return 0;
}

