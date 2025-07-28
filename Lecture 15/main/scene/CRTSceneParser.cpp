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
		triangles.push_back(std::move(triangle));
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

	scene.setSettings(std::move(parseSettings(jsonDocument)));
	scene.setCamera(std::move(parseCamera(jsonDocument)));
	scene.setMaterials(std::move(parseMaterials(jsonDocument)));
	scene.setLights(std::move(parseLights(jsonDocument)));
	scene.setObjects(std::move(parseObjects(jsonDocument)));
	scene.setTextures(std::move(parseTextures(jsonDocument)));

	ifs.close();
	return scene;
}

CRTSettings CRTSceneParser::parseSettings(const Document& jsonDocument)
{
	CRTSettings finalSettings;

	const Value& settings = jsonDocument.FindMember(CRT_SCENE_SETTINGS)->value;
	if (!settings.IsNull() && settings.IsObject())
	{
		const Value& bgColor = settings.FindMember(CRT_SCENE_BACKGROUND_COLOR)->value;
		assert(!bgColor.IsNull() && bgColor.IsArray());
		CRTVector backgroundColor = CRTVector::loadJSONVector(bgColor.GetArray());
		finalSettings.setBackgroundColor(std::move(backgroundColor));

		const Value& imageSettings = settings.FindMember(CRT_SCENE_IMAGE_RESOLUTION)->value;
		assert(!imageSettings.IsNull() && imageSettings.IsObject());

		int imageWidth = imageSettings
			.FindMember(CRT_SCENE_IMAGE_RESOLUTION_WIDTH)->value.GetInt();
		int imageHeight = imageSettings
			.FindMember(CRT_SCENE_IMAGE_RESOLUTION_HEIGHT)->value.GetInt();

		Grid imageResolution{ imageWidth, imageHeight };
		finalSettings.setImageResolution(std::move(imageResolution));

		const Value& bucketsCount = imageSettings.FindMember(CRT_SCENE_IMAGE_BUCKETS_COUNT)->value;
		if (bucketsCount.IsNull() || !bucketsCount.IsNumber())
		{
			finalSettings.setBucketsCount(DEFAULT_BUCKETS_COUNT);
		}
		else
		{
			finalSettings.setBucketsCount(bucketsCount.GetInt());
		}
	
		const Value& giFeature = imageSettings.FindMember(CRT_SCENE_GI)->value;
		if (giFeature.IsNull() || !giFeature.IsBool())
		{
			finalSettings.setGI(false);
		}
		else
		{
			finalSettings.setGI(giFeature.GetBool());
		}
		
	}

	return finalSettings;
}

CRTCamera CRTSceneParser::parseCamera(const Document& jsonDocument)
{
	CRTCamera finalCamera;

	const Value& camera = jsonDocument.FindMember(CRT_SCENE_CAMERA)->value;
	if (!camera.IsNull() && camera.IsObject())
	{
		const Value& cameraMatrix = camera.FindMember(CRT_SCENE_CAMERA_MATRIX)->value;
		assert(!cameraMatrix.IsNull() && cameraMatrix.IsArray());
		finalCamera.setRotationMatrix(std::move(CRTMatrix::loadJSONMatrix(cameraMatrix.GetArray())));

		const Value& cameraPosition = camera.FindMember(CRT_SCENE_CAMERA_POSITION)->value;
		assert(!cameraPosition.IsNull() && cameraPosition.IsArray());
		finalCamera.setPosition(std::move(CRTVector::loadJSONVector(cameraPosition.GetArray())));
	}

	return finalCamera;
}

std::vector<CRTMaterial> CRTSceneParser::parseMaterials(const Document& jsonDocument)
{
	std::vector<CRTMaterial> materials;

	const Value& materialsArr = jsonDocument.FindMember(CRT_SCENE_MATERIALS)->value;
	if (!materialsArr.IsNull() && materialsArr.IsArray())
	{
		int materialsCount = materialsArr.Size();

		for (int i = 0; i < materialsCount; i++) {

			const Value& material = materialsArr[i];
			assert(!material.IsNull() && material.IsObject());
			materials.push_back(std::move(parseMaterial(material)));
		}
	}

	return materials;
}

CRTMaterial CRTSceneParser::parseMaterial(const Value& jsonMaterial)
{
	CRTMaterial finalMaterial;

	const Value& type = jsonMaterial.FindMember(CRT_SCENE_MATERIAL_TYPE)->value;
	assert(!type.IsNull() && type.IsString());
	finalMaterial.setType(std::move(type.GetString()));

	if (finalMaterial.getType() == MaterialType::REFRACTIVE) {

		return parseRefractiveMaterial(jsonMaterial);
	}

	const Value& smoothShading = jsonMaterial.FindMember(CRT_SCENE_MATERIAL_SHADING)->value;
	assert(!smoothShading.IsNull() && smoothShading.IsBool());
	finalMaterial.setSmoothShading(smoothShading.GetBool());

	const Value& albedo = jsonMaterial.FindMember(CRT_SCENE_MATERIAL_ALBEDO)->value;

	assert(!albedo.IsNull());

	if (albedo.IsString())
	{
		finalMaterial.setTextureName(std::move(albedo.GetString()));
	}
	else if (albedo.IsArray())
	{
		CRTVector albedoVec = CRTVector::loadJSONVector(albedo.GetArray());
		finalMaterial.setAlbedo(std::move(albedoVec));
	}
	
	return finalMaterial;
}

CRTMaterial CRTSceneParser::parseRefractiveMaterial(const Value& jsonMaterial)
{
	const Value& ior = jsonMaterial.FindMember(CRT_SCENE_MATERIAL_IOR)->value;
	assert(!ior.IsNull() && ior.IsNumber());

	const Value& smoothShading = jsonMaterial.FindMember(CRT_SCENE_MATERIAL_SHADING)->value;
	assert(!smoothShading.IsNull() && smoothShading.IsBool());

	const Value& albedoArr = jsonMaterial.FindMember(CRT_SCENE_MATERIAL_ALBEDO)->value;
	assert(!albedoArr.IsNull() && albedoArr.IsArray());
	CRTVector albedoVec = CRTVector::loadJSONVector(albedoArr.GetArray());

	return CRTMaterial("refractive", ior.GetFloat(), albedoVec, smoothShading.GetBool());
}

std::vector<CRTLight> CRTSceneParser::parseLights(const Document& jsonDocument)
{
	std::vector<CRTLight> lights;

	const Value& lightsArr = jsonDocument.FindMember(CRT_SCENE_LIGHTS)->value;
	if (!lightsArr.IsNull() && lightsArr.IsArray())
	{
		int lightsCount = lightsArr.Size();

		for (int i = 0; i < lightsCount; i++) {

			const Value& light = lightsArr[i];
			assert(!light.IsNull() && light.IsObject());
			lights.push_back(std::move(parseLight(light)));
		}
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
	if (!objectsArr.IsNull() && objectsArr.IsArray())
	{
		int objectsCount = objectsArr.Size();

		for (int i = 0; i < objectsCount; i++) {

			const Value& object = objectsArr[i];
			assert(!object.IsNull() && object.IsObject());
			objects.push_back(std::move(parseMesh(object)));
		}
	}
		
	return objects;
}

CRTMesh CRTSceneParser::parseMesh(const Value& jsonMesh)
{
	CRTMesh finalMesh;

	const Value& materialIndex = jsonMesh.FindMember(CRT_SCENE_MATERIAL_INDEX)->value;
	if (!materialIndex.IsNull() && materialIndex.IsInt() && materialIndex.GetInt() >= 0)
	{
		finalMesh.setMaterialIndex(materialIndex.GetInt());

	}

	const Value& verticesArr = jsonMesh.FindMember(CRT_SCENE_VERTICES)->value;
	assert(!verticesArr.IsNull() && verticesArr.IsArray());
	auto vertices = loadJSONVertices(verticesArr.GetArray());
	finalMesh.setVertices(std::move(vertices));

	const Value& uvsArr = jsonMesh.FindMember(CRT_SCENE_OBJECT_UVS)->value;
	if (!uvsArr.IsNull() && uvsArr.IsArray())
	{
		auto uvs = loadJSONVertices(uvsArr.GetArray());
		finalMesh.setUvs(std::move(uvs));
	}

	const Value& trianglesArr = jsonMesh.FindMember(CRT_SCENE_TRIANGLES)->value;
	assert(!trianglesArr.IsNull() && trianglesArr.IsArray());
	auto triangles = loadJSONTriangles(trianglesArr.GetArray());
	finalMesh.setTriangleIndices(std::move(triangles));

	finalMesh.computeVerticesNormals();
	finalMesh.constructAABB();

	return finalMesh;
}

std::vector<CRTexture> CRTSceneParser::parseTextures(const Document& jsonDocument)
{
	std::vector<CRTexture> textures;

	const Value& texturesArr = jsonDocument.FindMember(CRT_SCENE_TEXTURES)->value;
	if (!texturesArr.IsNull() && texturesArr.IsArray())
	{
		int texturesSize = texturesArr.Size();

		for (int i = 0; i < texturesSize; i++) {

			const Value& texture = texturesArr[i];
			assert(!texture.IsNull() && texture.IsObject());
			textures.push_back(std::move(parseTexture(texture)));
		}
	}

	return textures;
}

CRTexture CRTSceneParser::parseTexture(const Value& jsonTexture)
{
	const Value& name = jsonTexture.FindMember(CRT_SCENE_TEXTURE_NAME)->value;
	assert(!name.IsNull() && name.IsString());

	const Value& type = jsonTexture.FindMember(CRT_SCENE_TEXTURE_TYPE)->value;
	assert(!type.IsNull() && type.IsString());

	CRTexture texture(name.GetString(), type.GetString());

	if (strcmp(type.GetString(), "albedo") == 0)
	{
		const Value& albedoArr = jsonTexture.FindMember(CRT_SCENE_TEXTURE_ALBEDO)->value;
		assert(!albedoArr.IsNull() && albedoArr.IsArray());
		auto albedo = CRTVector::loadJSONVector(albedoArr.GetArray());

		AlbedoData data = { albedo };
		texture.setAlbedoData(std::move(data));
	}
	else if (strcmp(type.GetString(), "edges") == 0)
	{
		const Value& edgeColorArr = jsonTexture.FindMember(CRT_SCENE_TEXTURE_EDGE_COLOR)->value;
		assert(!edgeColorArr.IsNull() && edgeColorArr.IsArray());
		auto edgeColor = CRTVector::loadJSONVector(edgeColorArr.GetArray());

		const Value& innerColorArr = jsonTexture.FindMember(CRT_SCENE_TEXTURE_INNER_COLOR)->value;
		assert(!innerColorArr.IsNull() && innerColorArr.IsArray());
		auto innerColor = CRTVector::loadJSONVector(innerColorArr.GetArray());

		const Value& edgeWidth = jsonTexture.FindMember(CRT_SCENE_TEXTURE_EDGE_WIDTH)->value;
		assert(!edgeWidth.IsNull() && edgeWidth.IsNumber());

		EdgesData data = { edgeColor, innerColor, edgeWidth.GetFloat() };
		texture.setEdgesData(std::move(data));
	}
	else if (strcmp(type.GetString(), "checker") == 0)
	{
		const Value& colorAArr = jsonTexture.FindMember(CRT_SCENE_TEXTURE_COLOR_A)->value;
		assert(!colorAArr.IsNull() && colorAArr.IsArray());
		auto colorA = CRTVector::loadJSONVector(colorAArr.GetArray());

		const Value& colorBArr = jsonTexture.FindMember(CRT_SCENE_TEXTURE_COLOR_B)->value;
		assert(!colorBArr.IsNull() && colorBArr.IsArray());
		auto colorB = CRTVector::loadJSONVector(colorBArr.GetArray());

		const Value& squareSize = jsonTexture.FindMember(CRT_SCENE_TEXTURE_SQUARE_SIZE)->value;
		assert(!squareSize.IsNull() && squareSize.IsNumber());

		CheckerData data = { colorA, colorB, squareSize.GetFloat() };
		texture.setCheckerData(std::move(data));
	}
	else if (strcmp(type.GetString(), "bitmap") == 0)
	{
		const Value& filePath = jsonTexture.FindMember(CRT_SCENE_TEXTURE_FILEPATH)->value;
		assert(!filePath.IsNull() && filePath.IsString());

		std::string filePathSubtr(filePath.GetString());
		std::string filePathStr = "image" + filePathSubtr;
		BitmapData data(std::move(filePathStr));
		texture.setBitmapData(std::move(data));
	}

	return texture;
}
