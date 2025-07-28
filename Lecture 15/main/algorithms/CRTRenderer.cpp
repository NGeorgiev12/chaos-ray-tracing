#include "CRTRenderer.h"

CRTRenderer::CRTRenderer(const std::string& sceneFileName, RenderType renderType, TraceType traceType) 
	: scene(sceneFileName), renderType(renderType), traceType(traceType)
{
}

CRTImageBuffer CRTRenderer::renderLinear(const std::string& outputFileName, const Grid& imageResolution)
{
	CRTImageBuffer imageBuffer(imageResolution);

	Interval region({ 0, 0 }, { imageResolution.imageWidth, imageResolution.imageHeight });
	renderRegion(imageBuffer, region, imageResolution);

	return imageBuffer;
}

CRTImageBuffer CRTRenderer::renderRegions(const std::string& outputFileName, const Grid& imageResolution)
{

	CRTImageBuffer imageBuffer(imageResolution);

	int numThreads = std::thread::hardware_concurrency();
	int cols = static_cast<int>(sqrt(numThreads));
	int rows = static_cast<int>((float)numThreads / cols);

	while ((cols * rows) < numThreads) {
		rows++;
	}

	int regionWidth = imageResolution.imageWidth / cols;
	int regionHeight = imageResolution.imageHeight / rows;

	std::vector<Interval> regions(numThreads);

	for (int row = 0; row < rows; row++) 
	{
		for (int col = 0; col < cols; col++) 
		{
			int startX = col * regionWidth;
			int startY = row * regionHeight;
			int width = (col == cols - 1) ? (imageResolution.imageWidth - startX) : regionWidth;
			int height = (row == rows - 1) ? (imageResolution.imageHeight - startY) : regionHeight;

			Interval currentRegion({ startX, startY }, { startX + width, startY + height });
			regions.push_back(std::move(currentRegion));
		}
	}

	std::vector<std::thread> threads;
	threads.reserve(regions.size());


	for (int i = 0; i < numThreads; i++)
	{

		Interval threadRegion = regions[i];

		threads.emplace_back(
			&CRTRenderer::renderRegion, 
			this,                      
			std::ref(imageBuffer),     
			threadRegion,
			imageResolution
		);
	}

	for (auto& thread : threads)
	{
		thread.join();
	}


	return imageBuffer;
}

void CRTRenderer::renderRegion(CRTImageBuffer& buffer, const Interval& region, const Grid& grid)
{
	const CRTSettings& settings = scene.getSettings();
	const CRTCamera& camera = scene.getCamera();
	const CRTVector& backgroundColor = settings.getBackgroundColor();

	for (int i = region.start.y; i < region.end.y; i++)
	{
		for (int j = region.start.x; j < region.end.x; j++)
		{
			Pixel currentPixel{ j, i };
			CRTVector pixelColorVec = backgroundColor;

			CRTRay cameraRay = CRTRay::generateCameraRay(camera, grid, currentPixel);

			CRTIntersectionResult intersectionResult = CRTRayTriangle::traceRay(cameraRay, this->scene, traceType);

			pixelColorVec = CRTShader::shade(cameraRay, intersectionResult, this->scene, traceType);

			buffer.setPixel(j, i, CRTColor(pixelColorVec));
		}
	}
}

CRTImageBuffer CRTRenderer::renderBuckets(const std::string& outputFileName, const Grid& grid)
{
	CRTImageBuffer buffer(grid);
	int bucketSize = scene.getSettings().getBucketsCount();

	std::queue<Interval> workQueue;

	for (int i = 0; i < grid.imageHeight; i += bucketSize)
	{
		int endY = std::min(i + bucketSize, grid.imageHeight);

		for (int j = 0; j < grid.imageWidth; j += bucketSize)
		{
			int endX = std::min(j + bucketSize, grid.imageWidth);
			Interval currentBucket({ j, i }, { endX, endY });
			workQueue.emplace(currentBucket);
		}
	}

	std::vector<std::thread> workers;
	workers.reserve(std::thread::hardware_concurrency());
	std::mutex queueMutex;
	std::atomic<bool> shouldStop = false;

	auto worker = [&]() 
	{
		while (!shouldStop) 
		{
			Interval bucket;
			{
				std::lock_guard<std::mutex> lock(queueMutex);
				if (workQueue.empty()) 
				{
					shouldStop = true;
					break;
				}
				bucket = workQueue.front();
				workQueue.pop();
			}

			renderRegion(buffer, bucket, grid);
		}
	};

	const unsigned int threadsCount = std::thread::hardware_concurrency();

	for (unsigned int i = 0; i < threadsCount; i++) 
	{
		workers.emplace_back(worker);
	}

	for (auto& t : workers) 
	{
		t.join();
	}

	return buffer;
}

void CRTRenderer::renderScene(const std::string& outputFileName)
{
	const CRTSettings& settings = scene.getSettings();
	const CRTCamera& camera = scene.getCamera();
	Grid imageResolution = settings.getImageResolution();

	imageResolution.imageWidth /= 2;
	imageResolution.imageHeight /= 2;
	CRTImageBuffer buffer(imageResolution);

	std::cout << "Started rendering" << std::endl;

	switch (renderType)
	{
		case RenderType::LINEAR:
		{
			buffer = renderLinear(outputFileName, imageResolution);
			break;
		}
		case RenderType::REGION:
		{
			buffer = renderRegions(outputFileName, imageResolution);
			break;
		}
		case RenderType::BUCKETS:
		{
			buffer = renderBuckets(outputFileName, imageResolution);
			break;
		}
		case RenderType::INVALID:
		{
			assert(false && "Invalid render type");
		}
	}

	std::cout << "Finished rendering" << std::endl;
	buffer.save(outputFileName, imageResolution);
}

void CRTRenderer::renderAnimation(const std::string& outputFileName)
{

	for (int i = 0; i < FRAMES_PER_ANIMATION; i++)
	{
		CRTCamera camera = scene.getCamera();
		const CRTVector& sceneCenter = scene.computeSceneCenter();
		camera.rotateAround(30.f, sceneCenter, CameraRotation::PAN);
		scene.setCamera(std::move(camera));

		std::string currentFileName = outputFileName + std::to_string(i) + ".ppm";
		renderScene(currentFileName);
	}
}
