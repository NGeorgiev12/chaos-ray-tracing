#include "CRTMesh.h"

CRTMesh::CRTMesh(const std::vector<CRTVector>& vertices,
	const std::vector<std::vector<int>>& triangles, int materialIndex)
	: vertices(vertices), triangleIndices(triangles), materialIndex(materialIndex)
{
	computeVerticesNormals();
}

CRTMesh::CRTMesh(std::vector<CRTVector>&& vertices,
	std::vector<std::vector<int>>&& triangles, int materialIndex)
	: vertices(std::move(vertices)), triangleIndices(std::move(triangles)), materialIndex(materialIndex)
{
	computeVerticesNormals();
}

CRTriangle CRTMesh::createTriangle(const std::vector<int>& indices, const std::vector<CRTVector>& vertices)
{
	assert(indices.size() == 3 && "Triangle indices must have exactly 3 elements");

	CRTVector v0 = vertices[indices[0]];
	CRTVector v1 = vertices[indices[1]];
	CRTVector v2 = vertices[indices[2]];
	return CRTriangle(v0, v1, v2);
}

const std::vector<CRTVector>& CRTMesh::getVerticesNormals() const
{
	return normals;
}

const std::vector<CRTVector>& CRTMesh::getVertices() const
{
	return vertices;
}

const std::vector<std::vector<int>>& CRTMesh::getTriangleIndices() const
{
	return triangleIndices;
}

int CRTMesh::getMaterialIndex() const
{
	return materialIndex;
}

int CRTMesh::getTrianglesCount() const
{
	return triangleIndices.size();
}

void CRTMesh::computeVerticesNormals() {

    normals.resize(vertices.size(), CRTVector(0.0f, 0.0f, 0.0f));
    std::vector<int> normalCounts(vertices.size(), 0);

    for (const auto& triIndices : triangleIndices) {

        CRTriangle tri = createTriangle(triIndices, vertices);
        CRTVector triNormal = tri.normalVector();

        for (int vertexIdx : triIndices) {
            normals[vertexIdx] += triNormal;
            normalCounts[vertexIdx]++;
        }
    }

    for (size_t i = 0; i < vertices.size(); i++) {
        
			float coefficient = 1.0f / normalCounts[i];
            normals[i] *= coefficient;
            normals[i].normalize();
    }
}

