#include "CRTSceneParser.h"
#include <fstream>

static std::vector<CRTVector> loadJSONVertices(const Value::ConstArray& arr)
{
	std::vector<CRTVector> vertices;
	assert(arr.Size() % FLOATS_PER_VERTEX == 0 && "Vertices array size must be a multiple of 3");
	vertices.reserve(arr.Size() / FLOATS_PER_VERTEX);

	for (int i = 0; i < arr.Size(); i += FLOATS_PER_VERTEX) {

		float x = arr[i].GetFloat();
		float y = arr[i + 1].GetFloat();
		float z = arr[i + 2].GetFloat();
		vertices.push_back(CRTVector(x, y, z));
	}

	return vertices;
}

static std::vector<std::vector<int>> loadJSONTriangles(const Value::ConstArray& arr)
{
	std::vector<std::vector<int>> triangles;
	assert(arr.Size() % TRIANGLE_VERTICES == 0 && "Triangles array size must be a multiple of 3");
	triangles.reserve(arr.Size() / TRIANGLE_VERTICES);

	for (int i = 0; i < arr.Size(); i += TRIANGLE_VERTICES) {

		std::vector<int> triangle;
		triangle.push_back(arr[i].GetInt());
		triangle.push_back(arr[i + 1].GetInt());
		triangle.push_back(arr[i + 2].GetInt());
		triangles.push_back(triangle);
	}

	return triangles;
}

CRTScene CRTSceneParser::parseScene(const std::string& sceneFileName)
{
	CRTScene scene;

	std::ifstream ifs(sceneFileName);
	assert(ifs.is_open() && "Failed to open scene file");
	IStreamWrapper isw(ifs);
	Document jsonDocument;
	jsonDocument.ParseStream(isw);

	scene.setSettings(parseSettings(jsonDocument));
	scene.setCamera(parseCamera(jsonDocument));
	scene.setObjects(parseObjects(jsonDocument));

	ifs.close();
	return scene;
}

CRTSettings CRTSceneParser::parseSettings(const Document& jsonDocument)
{
	const Value& settings = jsonDocument.FindMember(CRT_SCENE_SETTINGS)->value;
	assert(!settings.IsNull() && settings.IsObject());

	const Value& bgColor = settings.FindMember(CRT_SCENE_BACKGROUND_COLOR)->value;
	assert(!bgColor.IsNull() && bgColor.IsArray());

	const Value::ConstArray& bgColorArray = bgColor.GetArray();
	CRTVector backgroundColor = CRTVector::loadJSONVector(bgColorArray);

	const Value& imageSettings = settings.FindMember(CRT_SCENE_IMAGE_RESOLUTION)->value;
	assert(!imageSettings.IsNull() && imageSettings.IsObject());

	int imageWidth = imageSettings
		.FindMember(CRT_SCENE_IMAGE_RESOLUTION_WIDTH)->value.GetInt();
	int imageHeight = imageSettings
		.FindMember(CRT_SCENE_IMAGE_RESOLUTION_HEIGHT)->value.GetInt();

	Grid imageResolution{ imageWidth, imageHeight };
	return CRTSettings(std::move(backgroundColor), std::move(imageResolution));
}

CRTCamera CRTSceneParser::parseCamera(const Document& jsonDocument)
{
	const Value& camera = jsonDocument.FindMember(CRT_SCENE_CAMERA)->value;
	assert(!camera.IsNull() && camera.IsObject());

	const Value& cameraMatrix = camera.FindMember(CRT_SCENE_CAMERA_MATRIX)->value;
	assert(!cameraMatrix.IsNull() && cameraMatrix.IsArray());

	const Value& cameraPosition = camera.FindMember(CRT_SCENE_CAMERA_POSITION)->value;
	assert(!cameraPosition.IsNull() && cameraPosition.IsArray());

	return CRTCamera(CRTVector::loadJSONVector(cameraPosition.GetArray()),
		CRTMatrix::loadJSONMatrix(cameraMatrix.GetArray()));
}

std::vector<CRTMesh> CRTSceneParser::parseObjects(const Document& jsonDocument)
{
	std::vector<CRTMesh> objects;

	const Value& objectsArr = jsonDocument.FindMember(CRT_SCENE_OBJECTS)->value;
	assert(!objectsArr.IsNull() && objectsArr.IsArray());
	int objectsCount = objectsArr.Size();

    for (int i = 0; i < objectsCount; i++) {

		const Value& object = objectsArr[i];
		assert(!object.IsNull() && object.IsObject());
		objects.push_back(std::move(parseMesh(object)));
    }
		
	return objects;
}

CRTMesh CRTSceneParser::parseMesh(const Value& jsonMesh)
{
	const Value& verticesArr = jsonMesh.FindMember(CRT_SCENE_VERTICES)->value;
	assert(!verticesArr.IsNull() && verticesArr.IsArray());
	auto vertices = loadJSONVertices(verticesArr.GetArray());

	const Value& trianglesArr = jsonMesh.FindMember(CRT_SCENE_TRIANGLES)->value;
	assert(!trianglesArr.IsNull() && trianglesArr.IsArray());
	auto triangles = loadJSONTriangles(trianglesArr.GetArray());

	return CRTMesh(std::move(vertices), std::move(triangles));
}
