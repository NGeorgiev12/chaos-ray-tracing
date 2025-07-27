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

	imageResolution.imageHeight /= 4;
	imageResolution.imageWidth /= 4;
	writeHeader(ofs, imageResolution);

	const int totalPixels = imageResolution.imageWidth * imageResolution.imageHeight;
	int processedPixels = 0;
	const int updateInterval = totalPixels / 200;

	for (int i = 0; i < imageResolution.imageHeight; i++) {

		for (int j = 0; j < imageResolution.imageWidth; j++) {

			processedPixels++;
			if (processedPixels == 27600) {
				std::cout << "Here" << std::endl;
			}
			if (processedPixels % updateInterval == 0) {

				int progress = (processedPixels * 200) / totalPixels;
				std::cout << "Progress: " << progress << "%" << std::endl;
			}

			Pixel currentPixel{ j, i };
			CRTVector pixelColorVec = backgroundColor;

			CRTRay cameraRay = CRTRay::generateCameraRay(camera, imageResolution, currentPixel);

			CRTIntersectionResult intersectionResult = CRTRayTriangle::traceRay(cameraRay, this->scene);

			pixelColorVec = CRTShader::shade(cameraRay, intersectionResult, this->scene);
			
			ofs << CRTColor(pixelColorVec) << "\t";
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
