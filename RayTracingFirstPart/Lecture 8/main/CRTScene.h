#pragma once
#include <vector>
#include <string>
#include "CRTMesh.h"
#include "CRTCamera.h"
#include "CRTSettings.h"
#include "CRTLight.h"

class CRTScene
{
public:

	CRTScene() = default;
	CRTScene(const std::string& sceneFileName);

	const std::vector<CRTMesh>& getObjects() const;
	const CRTCamera& getCamera() const;
	const CRTSettings& getSettings() const;
	const std::vector<CRTLight>& getLights() const;

	void setCamera(const CRTCamera& camera);
	void setSettings(const CRTSettings& settings);
	void setObjects(const std::vector<CRTMesh>& objects);
	void setLights(const std::vector<CRTLight>& lights);

private:

	std::vector<CRTMesh> objects;
	CRTCamera camera;
	std::vector<CRTLight> lightSources;
	CRTSettings settings;
};

