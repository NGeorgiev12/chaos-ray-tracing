#pragma once
#include <vector>
#include "../math/CRTriangle.h"
#include "../image/CRTAxisAlignedBB.h"

class CRTMesh
{
public:

	CRTMesh() = default;
	CRTMesh(const std::vector<CRTVector>& vertices, const std::vector<CRTVector>& uvs,
		const std::vector<std::vector<int>>& triangles, int materialIndex);
	CRTMesh(std::vector<CRTVector>&& vertices, std::vector<CRTVector>&& uvs,
		std::vector<std::vector<int>>&& triangles, int materialIndex);

	static CRTriangle createTriangle(const std::vector<int>& indices,
		const std::vector<CRTVector>& vertices);
	
	const std::vector<CRTVector>& getVerticesNormals() const;
	const std::vector<CRTVector>& getVertices() const;
	const std::vector<std::vector<int>>& getTriangleIndices() const;
	const std::vector<CRTVector>& getUvs() const;
	const CRTAxisAlignedBB& getAABB() const;
	int getMaterialIndex() const;
	int getTrianglesCount() const;

	void setMaterialIndex(int materialIndex);
	void setVertices(std::vector<CRTVector>&& vertices);
	void setUvs(std::vector<CRTVector>&& uvs);
	void setTriangleIndices(std::vector<std::vector<int>>&& triangleIndices);
	void setNormals(std::vector<CRTVector>&& normals);

	void computeVerticesNormals();
	void constructAABB();

private:

	int materialIndex = -1;
	std::vector<CRTVector> vertices;
	std::vector<CRTVector> uvs;
	std::vector<std::vector<int>> triangleIndices;
	std::vector<CRTVector> normals;
	CRTAxisAlignedBB aabb;
};

