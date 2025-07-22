#include "CRTScene.h"
#include <fstream>
#include "CRTSceneParser.h"

CRTScene::CRTScene(const std::string& sceneFileName)
{
	*this = CRTSceneParser::parseScene(sceneFileName);
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

void CRTScene::setCamera(const CRTCamera& camera)
{
	this->camera = camera;
}

void CRTScene::setSettings(const CRTSettings& settings)
{
	this->settings = settings;
}

void CRTScene::setObjects(const std::vector<CRTMesh>& objects)
{
	this->objects = objects;
}

void CRTScene::setLights(const std::vector<CRTLight>& lights)
{
	this->lightSources = lights;
}

void CRTScene::setMaterials(const std::vector<CRTMaterial>& materials)
{
	this->materials = materials;
}
