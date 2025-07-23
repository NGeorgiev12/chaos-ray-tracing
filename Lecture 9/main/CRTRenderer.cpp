#include "CRTRenderer.h"

template <typename T>
T clamp(T value, T minVal, T maxVal) {
	return std::max(minVal, std::min(value, maxVal));
}

static CRTColor multiplyColors(const CRTColor& lhs, const CRTVector& rhs) {
	CRTVector colorVec(lhs.getRedComponent() * rhs.getVertex().x,
		lhs.getGreenComponent() * rhs.getVertex().y,
		lhs.getBlueComponent() * rhs.getVertex().z);

	return CRTColor(colorVec);
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

	imageResolution.imageHeight /= 12;
	imageResolution.imageWidth /= 12;
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
			pixelColor = traceRay(cameraRay);
			
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

CRTColor CRTRenderer::traceRay(const CRTRay& ray, int depth)
{
	CRTIntersectionResult result = CRTRayTriangle::intersectsRayTriangle(ray, scene);
	const CRTMaterial& material = scene.getMaterials()[result.materialIndex];

	if (result.hit == false) {
		return CRTColor(scene.getSettings().getBackgroundColor());
	}

	switch (material.getType())
	{
		case MaterialType::DIFFUSE: 
		{
			return handleDiffuse(result);
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

CRTColor CRTRenderer::handleDiffuse(const CRTIntersectionResult& result)
{
	return CRTShader::shade(result, scene);
}

CRTColor CRTRenderer::handleReflection(const CRTRay& ray, const CRTIntersectionResult& result, int depth)
{
	const CRTColor& backgroundColor = CRTColor(scene.getSettings().getBackgroundColor());
	const CRTMaterial& material = scene.getMaterials()[result.materialIndex];
	const CRTVector& albedo = material.getAlbedo();

	if (depth >= MAX_RAY_DEPTH) {
		return multiplyColors(backgroundColor, albedo);
	}

	CRTRay reflectedRay = CRTReflector::reflectRay(ray, result.hitPoint, result.hitNormal);

	CRTIntersectionResult reflectionResult = CRTRayTriangle::intersectsRayTriangle(reflectedRay, scene);

	if (reflectionResult.hit == false) {
		return multiplyColors(backgroundColor, albedo);
	}

	if (material.getType() == MaterialType::DIFFUSE) {
		
		CRTColor diffuseColor = handleDiffuse(reflectionResult);
		return multiplyColors(diffuseColor, albedo);
	}

	return handleReflection(reflectedRay, reflectionResult, depth + 1);
}
