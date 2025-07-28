#include "CRTScene.h"
#include <fstream>
#include "CRTSceneParser.h"

CRTScene::CRTScene(const std::string& sceneFileName)
{
	*this = std::move(CRTSceneParser::parseScene(sceneFileName));
	constructTextureIndices();
	constructAABoxes();
	computeAllTriangles();
	tree.build(aabb, allTriangles);
}

const std::vector<CRTMesh>& CRTScene::getObjects() const
{
	return objects;
}

const CRTCamera& CRTScene::getCamera() const
{
	return camera;
}

const CRTSettings& CRTScene::getSettings() const
{
	return settings;
}

const std::vector<CRTLight>& CRTScene::getLights() const
{
	return lightSources;
}

const std::vector<CRTMaterial>& CRTScene::getMaterials() const
{
	return materials;
}

const std::vector<CRTexture>& CRTScene::getTextures() const
{
	return textures;
}

const CRTKDTree& CRTScene::getKDTree() const
{
	return tree;
}

void CRTScene::setCamera(CRTCamera&& camera)
{
	this->camera = std::move(camera);
}

void CRTScene::setSettings(CRTSettings&& settings)
{
	this->settings = std::move(settings);
}

void CRTScene::setObjects(std::vector<CRTMesh>&& objects)
{
	this->objects = std::move(objects);
}

void CRTScene::setLights(std::vector<CRTLight>&& lights)
{
	this->lightSources = std::move(lights);
}

void CRTScene::setMaterials(std::vector<CRTMaterial>&& materials)
{
	this->materials = std::move(materials);
}

void CRTScene::setTextures(std::vector<CRTexture>&& textures)
{
	this->textures = std::move(textures);
}

int CRTScene::getTextureIndex(const std::string& textureName) const
{
	return textureIndices.at(textureName);
}

const std::vector<MeshTriangleRef>& CRTScene::getTriangleMap() const
{
	return triangleMap;
}

void CRTScene::constructAABoxes()
{
	for (const auto& mesh : objects)
	{
		aabb.includeBox(mesh.getAABB());
	}
}

void CRTScene::constructTextureIndices()
{
	int index = 0;
	for (const auto& texture : textures)
	{
		textureIndices[texture.getName()] = index;
		index++;
	}
}

void CRTScene::computeAllTriangles()
{
	for (int meshIdx = 0; meshIdx < objects.size(); meshIdx++) 
	{
		const auto& mesh = objects[meshIdx];
		const auto& tris = mesh.getTriangleIndices();

		for (int localIdx = 0; localIdx < tris.size(); localIdx++) 
		{
			CRTriangle tri = CRTMesh::createTriangle(tris[localIdx], mesh.getVertices());
			allTriangles.push_back(tri);
			triangleMap.push_back({ meshIdx, localIdx });
		}
	}
}
