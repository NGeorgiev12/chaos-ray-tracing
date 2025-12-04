#include "CRTMesh.h"

CRTMesh::CRTMesh(const std::vector<CRTVector>& vertices, const std::vector<std::vector<int>>& triangles)
	: vertices(vertices), triangleIndices(triangles)
{
}

CRTMesh::CRTMesh(std::vector<CRTVector>&& vertices, std::vector<std::vector<int>>&& triangles)
	: vertices(std::move(vertices)), triangleIndices(std::move(triangles))
{
}

const std::vector<CRTVector>& CRTMesh::getVertices() const
{
	return vertices;
}

const std::vector<std::vector<int>>& CRTMesh::getTriangleIndices() const
{
	return triangleIndices;
}

int CRTMesh::getTrianglesCount() const
{
	return triangleIndices.size();
}
