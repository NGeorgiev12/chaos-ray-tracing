#include "CRTRayTriangle.h"

CRTIntersectionResult CRTRayTriangle::intersectsRayTriangle(const CRTRay& ray, const CRTScene& scene)
{
	const CRTVector& rayDir = ray.getRayDirection();
	const CRTVector& rayOrigin = ray.getCameraOrigin();
	const std::vector<CRTMesh>& objects = scene.getObjects();
	const std::vector<CRTMaterial> materials = scene.getMaterials();

	CRTIntersectionResult result;

	for (const auto& mesh : objects) {

		const auto& vertices = mesh.getVertices();
		const auto& triangles = mesh.getTriangleIndices();
		const auto& verticesNormals = mesh.getVerticesNormals();

		for (const auto& triangleIndices : triangles) {

			CRTriangle triangle = CRTMesh::createTriangle(triangleIndices, vertices);
			const CRTVector& normal = triangle.normalVector();
			const CRTVector& v0 = triangle.getVertices()[0];

			float projection = dotProduct(normal, rayDir);
			float distanceToPlane = dotProduct(normal, v0 - rayOrigin);

			if (areEqualFloats(projection, 0.f)) {
				continue;
			}

			float t = distanceToPlane / projection;

			if (t <= 0.f || t >= result.t) {
				continue;
			}

			CRTVector intersectionPoint = rayOrigin + t * rayDir;

			if (isPointInsideTriangle(intersectionPoint, triangle)) {

				result.t = t;
				result.hit = true;
				result.triangle = triangle;
				result.hitPoint = intersectionPoint;
				result.vertexIndices = triangleIndices;
				result.materialIndex = mesh.getMaterialIndex();

				calculateIntersectionData(result, mesh, materials);
			}
		}
	}

	return result;
}

bool CRTRayTriangle::isPointInsideTriangle(const CRTVector& p, const CRTriangle& triangle) 
{
	const CRTVector& normal = triangle.normalVector();
	const CRTVector& v0 = triangle.getVertices()[0];
	const CRTVector& v1 = triangle.getVertices()[1];
	const CRTVector& v2 = triangle.getVertices()[2];

	CRTVector edge0 = v1 - v0;
	CRTVector edge1 = v2 - v1;
	CRTVector edge2 = v0 - v2;

	CRTVector toP0 = p - v0;
	CRTVector toP1 = p - v1;
	CRTVector toP2 = p - v2;

	bool checkE0 = dotProduct(normal, crossProduct(edge0, toP0)) > 0 ? true : false;
	bool checkE1 = dotProduct(normal, crossProduct(edge1, toP1)) > 0 ? true : false;
	bool checkE2 = dotProduct(normal, crossProduct(edge2, toP2)) > 0 ? true : false;

	return checkE0 && checkE1 && checkE2;
}

BarycentricCoordinates CRTRayTriangle::computeBarycentricCoordinates(const CRTMesh& mesh, const CRTIntersectionResult& result)
{
	const CRTriangle& triangle = result.triangle;
	const CRTVector& hitPoint = result.hitPoint;
	const CRTVector& v0 = triangle.getVertices()[0];

	CRTVector base1 = triangle.getFirstBaseVec();
	CRTVector base2 = triangle.getSecondBaseVec();
	float vecArea = crossProduct(base1, base2).getLength();

	CRTVector v0ToHit = hitPoint - v0;

	float u = crossProduct(v0ToHit, base2).getLength() / vecArea;
	float v = crossProduct(base1, v0ToHit).getLength() / vecArea;

	return { u, v, 1.0f - u - v };

}

void CRTRayTriangle::calculateIntersectionData(CRTIntersectionResult& result, const CRTMesh& mesh,
	 const std::vector<CRTMaterial>& materials)
{
	const auto& material = materials[result.materialIndex];

	if (material.isSmoothShading() == false) {
		result.hitNormal = result.triangle.normalVector();
		return;
	}

	const auto& verticesNormals = mesh.getVerticesNormals();
	const auto& triangleIndices = result.vertexIndices;

	const CRTVector& v0Normal = verticesNormals[triangleIndices[0]];
	const CRTVector& v1Normal = verticesNormals[triangleIndices[1]];
	const CRTVector& v2Normal = verticesNormals[triangleIndices[2]];

	auto barycentricCoords = computeBarycentricCoordinates(mesh, result);

	float u = barycentricCoords.u;
	float v = barycentricCoords.v;
	float w = barycentricCoords.w;

	result.hitNormal = v1Normal * u + v2Normal * v + v0Normal *	w;
}

