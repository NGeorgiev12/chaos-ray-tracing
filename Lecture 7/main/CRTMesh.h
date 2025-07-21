#pragma once
#include <vector>
#include "CRTVector.h"

class CRTMesh
{
public:

	CRTMesh() = default;
	CRTMesh(const std::vector<CRTVector>& vertices, const std::vector<std::vector<int>>& triangles);
	CRTMesh(std::vector<CRTVector>&& vertices, std::vector<std::vector<int>>&& triangles);

	const std::vector<CRTVector>& getVertices() const;
	const std::vector<std::vector<int>>& getTriangleIndices() const;
	int getTrianglesCount() const;

private:
	std::vector<CRTVector> vertices; 
	std::vector<std::vector<int>> triangleIndices;
};

