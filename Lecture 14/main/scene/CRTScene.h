#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "CRTMesh.h"
#include "CRTCamera.h"
#include "CRTSettings.h"
#include "CRTLight.h"
#include "CRTMaterial.h"
#include "CRTexture.h"
#include "../image/CRTKDTree.h"

struct MeshTriangleRef
{
	int meshIndex;
	int localTriIdx;
};

class CRTScene
{
public:

	CRTScene() = default;
	CRTScene(const std::string& sceneFileName);

	CRTScene(const CRTScene&) = delete;
	CRTScene& operator=(const CRTScene&) = delete;

	CRTScene(CRTScene&&) noexcept = default;
	CRTScene& operator=(CRTScene&&) noexcept = default;

	const std::vector<CRTMesh>& getObjects() const;
	const CRTCamera& getCamera() const;
	const CRTSettings& getSettings() const;
	const std::vector<CRTLight>& getLights() const;
	const std::vector<CRTMaterial>& getMaterials() const;
	const std::vector<CRTexture>& getTextures() const;
	const CRTKDTree& getKDTree() const;

	void setCamera(CRTCamera&& camera);
	void setSettings(CRTSettings&& settings);
	void setObjects(std::vector<CRTMesh>&& objects);
	void setLights(std::vector<CRTLight>&& lights);
	void setMaterials(std::vector<CRTMaterial>&& materials);
	void setTextures(std::vector<CRTexture>&& textures);

	int getTextureIndex(const std::string& textureName) const;
	const std::vector<MeshTriangleRef>& getTriangleMap() const;

private:

	std::vector<CRTMesh> objects;
	CRTCamera camera;
	std::vector<CRTexture> textures;
	std::vector<CRTMaterial> materials;
	std::vector<CRTLight> lightSources;
	CRTSettings settings;

	std::unordered_map<std::string, int> textureIndices;
	CRTAxisAlignedBB aabb;
	CRTKDTree tree;
	std::vector<CRTriangle> allTriangles;
	std::vector<MeshTriangleRef> triangleMap;

	void constructAABoxes();
	void constructTextureIndices();
	void computeAllTriangles();

};

