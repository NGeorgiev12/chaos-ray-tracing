#include "CRTScene.h"
#include <fstream>
#include "CRTSceneParser.h"

CRTScene::CRTScene(const std::string& sceneFileName)
{
	*this = std::move(CRTSceneParser::parseScene(sceneFileName));
	constructTextureIndices();
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

void CRTScene::constructTextureIndices()
{
	int index = 0;
	for (const auto& texture : textures)
	{
		textureIndices[texture.getName()] = index;
		index++;
	}
}
