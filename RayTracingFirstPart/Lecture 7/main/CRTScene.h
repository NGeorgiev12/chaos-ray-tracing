#pragma once
#include <vector>
#include <string>
#include "CRTMesh.h"
#include "CRTCamera.h"
#include "CRTSettings.h"

class CRTScene
{
public:

	CRTScene() = default;
	CRTScene(const std::string& sceneFileName);

	const std::vector<CRTMesh>& getObjects() const;
	const CRTCamera& getCamera() const;
	const CRTSettings& getSettings() const;

	void setCamera(const CRTCamera& camera);
	void setSettings(const CRTSettings& settings);
	void setObjects(const std::vector<CRTMesh>& objects);

private:

	std::vector<CRTMesh> objects;
	CRTCamera camera;
	CRTSettings settings;
};

