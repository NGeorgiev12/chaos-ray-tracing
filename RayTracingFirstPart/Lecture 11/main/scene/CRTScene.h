#pragma once
#include <vector>
#include <string>
#include "CRTMesh.h"
#include "CRTCamera.h"
#include "CRTSettings.h"
#include "CRTLight.h"
#include "CRTMaterial.h"

class CRTScene
{
public:

	CRTScene() = default;
	CRTScene(const std::string& sceneFileName);

	const std::vector<CRTMesh>& getObjects() const;
	const CRTCamera& getCamera() const;
	const CRTSettings& getSettings() const;
	const std::vector<CRTLight>& getLights() const;
	const std::vector<CRTMaterial>& getMaterials() const;

	void setCamera(const CRTCamera& camera);
	void setSettings(const CRTSettings& settings);
	void setObjects(const std::vector<CRTMesh>& objects);
	void setLights(const std::vector<CRTLight>& lights);
	void setMaterials(const std::vector<CRTMaterial>& materials);

private:

	std::vector<CRTMesh> objects;
	CRTCamera camera;
	std::vector<CRTMaterial> materials;
	std::vector<CRTLight> lightSources;
	CRTSettings settings;
};

