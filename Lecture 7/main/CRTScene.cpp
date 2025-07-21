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
