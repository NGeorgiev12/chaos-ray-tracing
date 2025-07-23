#include "CRTRenderer.h"

template <typename T>
T clamp(T value, T minVal, T maxVal) {
	return std::max(minVal, std::min(value, maxVal));
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
			pixelColor = CRTColor(traceRay(cameraRay));
			
			ofs << pixelColor << "\t";
		}

		ofs << "\n";
	}

	ofs.close();
}

void CRTRenderer::writeHeader(std::ofstream& ofs, const Grid& grid)
{
	ofs << "P3\n";
	ofs << grid.imageWidth << " " << grid.imageHeight << "\n";
	ofs << MAX_COLOR_COMPONENT << "\n";
}

CRTVector CRTRenderer::traceRay(const CRTRay& ray)
{
	CRTIntersectionResult result = CRTRayTriangle::intersectsRayTriangle(ray, scene);
	const CRTMaterial& material = scene.getMaterials()[result.materialIndex];

	if (result.hit == false) {
		return scene.getSettings().getBackgroundColor();
	}

	switch (material.getType())
	{
		case MaterialType::DIFFUSE: 
		{
			return CRTShader::shade(result, this->scene);
		}
		case MaterialType::REFLECTIVE:
		{
			return CRTReflector::handleReflection(ray, result, this->scene);

		}
		default:
		{
			throw std::runtime_error("Unsupported material type encountered during ray tracing.");
		}
	}
}
