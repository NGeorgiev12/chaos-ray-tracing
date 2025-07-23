#include "CRTRenderer.h"

template <typename T>
T clamp(T value, T minVal, T maxVal) {
	return std::max(minVal, std::min(value, maxVal));
}

static CRTVector multiplyColors(const CRTVector& lhs, const CRTVector& rhs) {
	CRTVector colorVec(lhs.getComponent(RED_COMPONENT) * rhs.getVertex().x,
		lhs.getComponent(GREEN_COMPONENT) * rhs.getVertex().y,
		lhs.getComponent(BLUE_COMPONENT) * rhs.getVertex().z);

	return colorVec;
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
	ofs << CRTColor::maxColorComponent << "\n";
}

CRTVector CRTRenderer::traceRay(const CRTRay& ray, int depth)
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
			return handleReflection(ray, result, depth);

		}
		default:
		{
			throw std::runtime_error("Unsupported material type encountered during ray tracing.");
		}
	}
}

CRTVector CRTRenderer::handleReflection(const CRTRay& ray, CRTIntersectionResult& result, int depth)
{
	const CRTVector& backgroundColor = scene.getSettings().getBackgroundColor();
	const CRTMaterial& material = scene.getMaterials()[result.materialIndex];
	const CRTVector& albedo = material.getAlbedo();

	if (depth >= MAX_RAY_DEPTH) {
		return multiplyColors(backgroundColor, albedo);
	}

	CRTRay reflectedRay = CRTReflector::reflectRay(ray, result.hitPoint, result.hitNormal);

	CRTIntersectionResult reflectionResult = CRTRayTriangle::intersectsRayTriangle(reflectedRay, scene);
	const CRTMaterial& reflectedMaterial = scene.getMaterials()[reflectionResult.materialIndex];

	if (reflectionResult.hit == false) {
		return multiplyColors(backgroundColor, albedo);
	}

	if (reflectedMaterial.getType() == MaterialType::DIFFUSE) {
		
		CRTVector diffuseColor = CRTShader::shade(reflectionResult, this->scene);
		return multiplyColors(diffuseColor, albedo);
	}

	return handleReflection(reflectedRay, reflectionResult, depth + 1);
}
