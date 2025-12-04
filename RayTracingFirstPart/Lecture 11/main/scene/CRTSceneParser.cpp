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
	scene.setMaterials(parseMaterials(jsonDocument));
	scene.setLights(parseLights(jsonDocument));
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

std::vector<CRTMaterial> CRTSceneParser::parseMaterials(const Document& jsonDocument)
{
	std::vector<CRTMaterial> materials;

	const Value& materialsArr = jsonDocument.FindMember(CRT_SCENE_MATERIALS)->value;
	assert(!materialsArr.IsNull() && materialsArr.IsArray());
	int materialsCount = materialsArr.Size();

	for (int i = 0; i < materialsCount; i++) {

		const Value& material = materialsArr[i];
		assert(!material.IsNull() && material.IsObject());
		materials.push_back(std::move(parseMaterial(material)));
	}

	return materials;
}

CRTMaterial CRTSceneParser::parseMaterial(const Value& jsonMaterial)
{
	const Value& type = jsonMaterial.FindMember(CRT_SCENE_MATERIAL_TYPE)->value;
	assert(!type.IsNull() && type.IsString());

	if (strcmp(type.GetString(), "refractive") == 0) {

		return parseRefractiveMaterial(jsonMaterial);
	}

	const Value& albedoArr = jsonMaterial.FindMember(CRT_SCENE_MATERIAL_ALBEDO)->value;
	assert(!albedoArr.IsNull() && albedoArr.IsArray());
	CRTVector albedo = CRTVector::loadJSONVector(albedoArr.GetArray());

	const Value& smoothShading = jsonMaterial.FindMember(CRT_SCENE_MATERIAL_SHADING)->value;
	assert(!smoothShading.IsNull() && smoothShading.IsBool());

	return CRTMaterial(type.GetString(), std::move(albedo), smoothShading.GetBool());
}

CRTMaterial CRTSceneParser::parseRefractiveMaterial(const Value& jsonMaterial)
{
	const Value& ior = jsonMaterial.FindMember(CRT_SCENE_MATERIAL_IOR)->value;
	assert(!ior.IsNull() && ior.IsNumber());

	const Value& smoothShading = jsonMaterial.FindMember(CRT_SCENE_MATERIAL_SHADING)->value;
	assert(!smoothShading.IsNull() && smoothShading.IsBool());

	return CRTMaterial("refractive", ior.GetFloat(), smoothShading.GetBool());
}

std::vector<CRTLight> CRTSceneParser::parseLights(const Document& jsonDocument)
{
	std::vector<CRTLight> lights;

	const Value& lightsArr = jsonDocument.FindMember(CRT_SCENE_LIGHTS)->value;
	assert(!lightsArr.IsNull() && lightsArr.IsArray());
	int lightsCount = lightsArr.Size();

	for (int i = 0; i < lightsCount; i++) {

		const Value& light = lightsArr[i];
		assert(!light.IsNull() && light.IsObject());
		lights.push_back(std::move(parseLight(light)));
	}

	return lights;
}

CRTLight CRTSceneParser::parseLight(const Value& jsonLight)
{
	const Value& intensity = jsonLight.FindMember(CRT_SCENE_LIGHT_INTENSITY)->value;
	assert(!intensity.IsNull() && intensity.IsNumber());

	const Value& position = jsonLight.FindMember(CRT_SCENE_LIGHT_POSITION)->value;
	assert(!position.IsNull() && position.IsArray());
	CRTVector lightPosition = CRTVector::loadJSONVector(position.GetArray());

	return CRTLight(std::move(lightPosition), intensity.GetInt());
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
	const Value& materialIndex = jsonMesh.FindMember(CRT_SCENE_MATERIAL_INDEX)->value;
	assert(!materialIndex.IsNull() && materialIndex.IsInt() && materialIndex.GetInt() >= 0);

	const Value& verticesArr = jsonMesh.FindMember(CRT_SCENE_VERTICES)->value;
	assert(!verticesArr.IsNull() && verticesArr.IsArray());
	auto vertices = loadJSONVertices(verticesArr.GetArray());

	const Value& trianglesArr = jsonMesh.FindMember(CRT_SCENE_TRIANGLES)->value;
	assert(!trianglesArr.IsNull() && trianglesArr.IsArray());
	auto triangles = loadJSONTriangles(trianglesArr.GetArray());

	return CRTMesh(std::move(vertices), std::move(triangles), materialIndex.GetInt());
}
