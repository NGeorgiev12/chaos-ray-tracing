#include "CRTRenderer.h"

template <typename T>
T clamp(T value, T minVal, T maxVal) {
	return std::max(minVal, std::min(value, maxVal));
}

static CRTVector multiplyColors(const CRTColor& lhs, const CRTVector& rhs) {
	return CRTVector(lhs.getRedComponent() * rhs.getVertex().x,
		lhs.getGreenComponent() * rhs.getVertex().y,
		lhs.getBlueComponent() * rhs.getVertex().z);
}

CRTRenderer::CRTRenderer(const std::string& sceneFileName) : scene(sceneFileName)
{
}

void CRTRenderer::renderScene(const std::string& outputFileName)
{
	std::ofstream ofs(outputFileName, std::ios::out | std::ios::binary);
	assert(ofs.is_open() && "Failed to open output file for writing");

	const CRTSettings& settings = scene.getSettings();
	const CRTCamera& camera = scene.getCamera();
	Grid imageResolution = settings.getImageResolution();
	const CRTVector& backgroundColor = settings.getBackgroundColor();

	imageResolution.imageHeight /= 8;
	imageResolution.imageWidth /= 8;
	writeHeader(ofs, imageResolution);

	const int totalPixels = imageResolution.imageWidth * imageResolution.imageHeight;
	int processedPixels = 0;
	const int updateInterval = totalPixels / 200;

	for (int i = 0; i < imageResolution.imageHeight; i++) {

		for (int j = 0; j < imageResolution.imageWidth; j++) {

			processedPixels++;
			if (processedPixels % updateInterval == 0) {

				int progress = (processedPixels * 200) / totalPixels;
				std::cout << "Progress: " << progress << "%" << std::endl;
			}

			Pixel currentPixel{ j, i };
			CRTColor pixelColor(backgroundColor);
			CRTRay cameraRay = CRTRay::generateRay(camera, imageResolution, currentPixel);
			pixelColor = traceRay(cameraRay, scene);
			
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

CRTColor CRTRenderer::traceRay(const CRTRay& ray, const CRTScene& scene, int depth)
{
	CRTColor backgroundColor = CRTColor(scene.getSettings().getBackgroundColor());

	if (depth >= MAX_RAY_DEPTH) {
		return backgroundColor;
	}

	CRTIntersectionResult result = CRTRayTriangle::intersectsRayTriangle(ray, scene);

	if (!result.hit) {
		return backgroundColor;
	}

	const CRTMaterial& material = scene.getMaterials()[result.materialIndex];

	switch (material.getType())
	{
		case MaterialType::DIFFUSE: 
		{
			return CRTShader::shade(result, scene);
		}
		case MaterialType::REFLECTIVE:
		{
			CRTRay reflectedRay = CRTReflector::reflectRay(ray, result.hitPoint, result.hitNormal);
			CRTColor reflectedColor = traceRay(reflectedRay, scene, depth + 1);
			return CRTColor(multiplyColors(reflectedColor, material.getAlbedo()));
		}
	}

	return backgroundColor;
}
