#include "CRTRenderer.h"

template <typename T>
T clamp(T value, T minVal, T maxVal) {
	return std::max(minVal, std::min(value, maxVal));
}

int scaleTo255(float value, float minVal, float maxVal) {
	float normalized = (value - minVal) / (maxVal - minVal);
	return static_cast<int>(clamp(normalized * 255.0f, 0.0f, 255.0f));
}

// Function to convert triangle to RGB color
CRTVector triangleToColor(const CRTVector& p1, const CRTVector& p2, const CRTVector& p3) {
	// Compute centroid
	float cx = (p1.getVertex().x + p2.getVertex().x + p3.getVertex().x) / 3.0f;
	float cy = (p1.getVertex().y + p2.getVertex().y + p3.getVertex().y) / 3.0f;
	float cz = (p1.getVertex().z + p2.getVertex().z + p3.getVertex().z) / 3.0f;

	// Map centroid to RGB (assume coordinate bounds for normalization)
	int r = scaleTo255(cx, -1.0f, 1.0f); // adjust min/max bounds if needed
	int g = scaleTo255(cy, -1.0f, 1.0f);
	int b = scaleTo255(cz, -1.0f, 1.0f);

	return CRTVector(r, g, b);
}

static CRTriangle createTriangle(const std::vector<int>& indices, const std::vector<CRTVector>& vertices) {

	assert(indices.size() == 3 && "Triangle indices must have exactly 3 elements");

	CRTVector v0 = vertices[indices[0]];
	CRTVector v1 = vertices[indices[1]];
	CRTVector v2 = vertices[indices[2]];
	return CRTriangle(v0, v1, v2);
}

static std::vector<CRTColor> createTrianglesColors(const CRTScene& scene) {
	std::vector<CRTColor> colors;

	size_t totalTriangles = 0;
	for (const auto& mesh : scene.getObjects()) {

		totalTriangles += mesh.getTrianglesCount();
	}

	colors.reserve(totalTriangles);
	
	for (const auto& mesh : scene.getObjects()) {

		for (size_t i = 0; i < mesh.getTriangleIndices().size(); ++i) {
			colors.push_back(CRTColor::generateRandomColor());
		}
	}

	return colors;
}

CRTRenderer::CRTRenderer(const std::string& sceneFileName) : scene(sceneFileName)
{
}

void CRTRenderer::renderScene()
{
	std::ofstream ofs("output5_image.ppm", std::ios::out | std::ios::binary);
	assert(ofs.is_open() && "Failed to open output file for writing");

	const CRTSettings& settings = scene.getSettings();
	const CRTCamera& camera = scene.getCamera();
	Grid imageResolution = settings.getImageResolution();
	const CRTVector& backgroundColor = settings.getBackgroundColor();

	std::vector<CRTColor> triangleColours = createTrianglesColors(scene);
	imageResolution.imageHeight /= 8;
	imageResolution.imageWidth /= 8;
	writeHeader(ofs, imageResolution);
	auto colours = createTrianglesColors(scene);

	const int totalPixels = imageResolution.imageWidth * imageResolution.imageHeight;
	int processedPixels = 0;
	const int updateInterval = totalPixels / 100;

	for (int i = 0; i < imageResolution.imageHeight; i++) {

		for (int j = 0; j < imageResolution.imageWidth; j++) {

			processedPixels++;
			if (processedPixels % updateInterval == 0) {
				int progress = (processedPixels * 100) / totalPixels;
				std::cout << "Progress: " << progress << "%" << std::endl;
			}

			Pixel currentPixel{ j, i };
			CRTColor pixelColor(backgroundColor);
			CRTRay cameraRay(camera, imageResolution, currentPixel);

			intersectsRayTriangle(cameraRay, scene, pixelColor);

			ofs << pixelColor << "\t";
		}

		ofs << "\n";
	}
}

void CRTRenderer::writeHeader(std::ofstream& ofs, const Grid& grid)
{
	ofs << "P3\n";
	ofs << grid.imageWidth << " " << grid.imageHeight << "\n";
	ofs << CRTColor::maxColorComponent << "\n";
}

int CRTRenderer::intersectsRayTriangle(const CRTRay& ray, const CRTScene& scene, CRTColor& colour)
{
	const CRTVector& rayDir = ray.getRayDirection();
	const CRTVector& cameraOrigin = ray.getCameraOrigin();
	const std::vector<CRTMesh>& objects = scene.getObjects();

	float minT = std::numeric_limits<float>::max();
	int closestTriangleIndex = -1;
	int currentIndex = 0;

	for (const auto& mesh : objects) {

		const auto& vertices = mesh.getVertices();
		const auto& triangles = mesh.getTriangleIndices();

		for (const auto& triangleIndices : triangles) {

			CRTriangle triangle = createTriangle(triangleIndices, vertices);
			const CRTVector& normal = triangle.normalVector();
			const CRTVector& v0 = triangle.getVertices()[0];

			float dotNormalCameraRay = dotProduct(normal, rayDir);
			float dotNormalToVertex0 = dotProduct(normal, v0 - cameraOrigin);

			if (areEqualFloats(dotNormalCameraRay, 0.f)) {
				currentIndex++;
				continue;
			}

			float t = dotNormalToVertex0 / dotNormalCameraRay;

			if (t < 0) {
				currentIndex++;
				continue;
			}

			CRTVector intersectionPoint = cameraOrigin + t * rayDir;

			if (isPointInsideTriangle(intersectionPoint, triangle)) {

				if (t < minT) {
					colour = triangleToColor(triangle.getVertices()[0], triangle.getVertices()[1], triangle.getVertices()[2]);
					minT = t;
					closestTriangleIndex = currentIndex;
				}

				currentIndex++;
			}
		}
	}

	return closestTriangleIndex;

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


