#pragma once
#include "CRTScene.h"

class CRTSceneParser
{
public:

	static CRTScene parseScene(const std::string& sceneFileName);
	
private:

	static CRTSettings parseSettings(const Document& jsonDocument);
	static CRTCamera parseCamera(const Document& jsonDocument);
	static std::vector<CRTLight> parseLights(const Document& jsonDocument);
	static CRTLight parseLight(const Value& jsonLight);
	static std::vector<CRTMesh> parseObjects(const Document& jsonDocument);
	static CRTMesh parseMesh(const Value& jsonMesh);
};

