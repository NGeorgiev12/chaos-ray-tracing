#pragma once
#include <vector>
#include "../math/CRTriangle.h"

class CRTMesh
{
public:

	CRTMesh() = default;
	CRTMesh(const std::vector<CRTVector>& vertices,
		const std::vector<std::vector<int>>& triangles, int materialIndex);
	CRTMesh(std::vector<CRTVector>&& vertices,
		std::vector<std::vector<int>>&& triangles, int materialIndex);

	static CRTriangle createTriangle(const std::vector<int>& indices, const std::vector<CRTVector>& vertices);
	
	const std::vector<CRTVector>& getVerticesNormals() const;
	const std::vector<CRTVector>& getVertices() const;
	const std::vector<std::vector<int>>& getTriangleIndices() const;
	int getMaterialIndex() const;
	int getTrianglesCount() const;

private:

	int materialIndex = -1;
	std::vector<CRTVector> vertices;
	std::vector<std::vector<int>> triangleIndices;
	std::vector<CRTVector> normals;

	void computeVerticesNormals();
};

