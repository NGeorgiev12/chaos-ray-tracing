#include "CRTRayTriangle.h"

CRTIntersectionResult CRTRayTriangle::traceRay(const CRTRay& ray, const CRTScene& scene, TraceType type, float maxT)
{
	switch (type)
	{
	case TraceType::TRIANGLE:
		return traceRayTriangle(ray, scene, maxT);
	case TraceType::AABB:
		return traceRayAABB(ray, scene, maxT);
	case TraceType::KD:
		return traceRayKD(ray, scene, maxT);
	case TraceType::INVALID:
		throw std::runtime_error("Incorrect trace type");
	}
}

CRTIntersectionResult CRTRayTriangle::traceRayTriangle(const CRTRay& ray, const CRTScene& scene, float maxT)
{
	const std::vector<CRTMesh>& objects = scene.getObjects();

	CRTIntersectionResult result;

	for (const auto& mesh : objects) 
	{
		updateIntersectionData(ray, mesh, result, scene, maxT);
	}

	return result;
}

CRTIntersectionResult CRTRayTriangle::traceRayAABB(const CRTRay& ray, const CRTScene& scene, float maxT)
{
	CRTIntersectionResult result;

	const std::vector<CRTMesh>& objects = scene.getObjects();

	for (const auto& mesh : objects)
	{
		const auto& aabb = mesh.getAABB();

		if (!intersectsRayAABB(ray, aabb))
		{
			continue;
		}

		updateIntersectionData(ray, mesh, result, scene, maxT);

	}
	return result;
}

CRTIntersectionResult CRTRayTriangle::traceRayKD(const CRTRay& ray, const CRTScene& scene, float maxT)
{
	const auto& kdTree = scene.getKDTree();
	const auto& nodes = kdTree.getNodes();
	const auto& objects = scene.getObjects();

	CRTIntersectionResult result;

	std::stack<int> nodeIndicesToCheck;
	nodeIndicesToCheck.push(0);

	while (nodeIndicesToCheck.size() > 0)
	{
		int currentIndexToCheck = nodeIndicesToCheck.top();
		nodeIndicesToCheck.pop();

		const CRTKDTreeNode& node = nodes[currentIndexToCheck];

		if (intersectsRayAABB(ray, node.aabb))
		{
			if (node.triangleIndices.size() > 0) 
			{
				for (int triIdx : node.triangleIndices)
				{
					const MeshTriangleRef& ref = scene.getTriangleMap()[triIdx];
					const CRTMesh& mesh = objects[ref.meshIndex];

					auto& triIndices = mesh.getTriangleIndices()[ref.localTriIdx];
					CRTriangle tri = CRTMesh::createTriangle(triIndices, mesh.getVertices());

					intersectSingleTriangle(ray, tri, mesh, scene, maxT, result);
				}
			}
			else 
			{
				if (node.leftChild != -1) 
				{
					nodeIndicesToCheck.push(node.leftChild);
				}
				if (node.rightChild != -1) 
				{
					nodeIndicesToCheck.push(node.rightChild);
				}
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

	bool checkE0 = (dotProduct(normal, crossProduct(edge0, toP0)) >= -EPSILON) ? true : false;
	bool checkE1 = (dotProduct(normal, crossProduct(edge1, toP1)) >= -EPSILON) ? true : false;
	bool checkE2 = (dotProduct(normal, crossProduct(edge2, toP2)) >= -EPSILON) ? true : false;

	return checkE0 && checkE1 && checkE2;
}

bool CRTRayTriangle::intersectsRayAABB(const CRTRay& ray, const CRTAxisAlignedBB& box)
{

	float tMin = 0.f;
	float tMax = FLT_MAX;          
	const CRTVector& rayOrigin = ray.getCameraOrigin();
	const CRTVector& rayDir = ray.getRayDirection();
	const CRTVector& lowerBound = box.getLowerBound();
	const CRTVector& upperBound = box.getUpperBound();

	for (int axis = 0; axis < AXIS_COUNT; axis++) {

		float dirComponent = rayDir.getComponent(axis);

		if (areEqualFloats(dirComponent, 0.f)) {

			if (rayOrigin.getComponent(axis) < lowerBound.getComponent(axis) ||
				rayOrigin.getComponent(axis) > upperBound.getComponent(axis)) {
				return false;
			}
			continue;
		}

		float t1 = (lowerBound.getComponent(axis) - rayOrigin.getComponent(axis)) / dirComponent;
		float t2 = (upperBound.getComponent(axis) - rayOrigin.getComponent(axis)) / dirComponent;

		if (t1 > t2) {
			std::swap(t1, t2);
		}

		tMin = std::max(tMin, t1);
		tMax = std::min(tMax, t2);

		if (tMax < tMin) 
			return false;

		if (tMax < 0) 
			return false;

	}

	return true;
}

void CRTRayTriangle::updateIntersectionData(const CRTRay& ray, const CRTMesh& mesh, CRTIntersectionResult& result,
	const CRTScene& scene, float maxT)
{
	const auto& vertices = mesh.getVertices();
	const auto& triangles = mesh.getTriangleIndices();

	for (const auto& triangleIndices : triangles)
	{
		CRTriangle triangle = CRTMesh::createTriangle(triangleIndices, vertices);
		intersectSingleTriangle(ray, triangle, mesh, scene, maxT, result);
	}
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
	const auto& uvVectors = mesh.getUvs();
	const auto& triangleIndices = result.vertexIndices;

	auto barycentricCoords = computeBarycentricCoordinates(mesh, result);
	result.baryCoords = barycentricCoords;

	float u = barycentricCoords.u;
	float v = barycentricCoords.v;
	float w = barycentricCoords.w;

	if (!uvVectors.empty())
	{
		const CRTVector& uv0 = uvVectors[triangleIndices[0]];
		const CRTVector& uv1 = uvVectors[triangleIndices[1]];
		const CRTVector& uv2 = uvVectors[triangleIndices[2]];

		result.pUV = uv1 * u + uv2 * v + uv0 * w;
	}

	if (material.isSmoothShading() == false) {
		result.hitNormal = result.triangle.normalVector();
		return;
	}

	const auto& verticesNormals = mesh.getVerticesNormals();

	const CRTVector& v0Normal = verticesNormals[triangleIndices[0]];
	const CRTVector& v1Normal = verticesNormals[triangleIndices[1]];
	const CRTVector& v2Normal = verticesNormals[triangleIndices[2]];

	result.hitNormal = v1Normal * u + v2Normal * v + v0Normal *	w;
}

bool CRTRayTriangle::intersectSingleTriangle(
	const CRTRay& ray,
	const CRTriangle& triangle,
	const CRTMesh& mesh,
	const CRTScene& scene,
	float maxT,
	CRTIntersectionResult& result)
{
	const auto& rayDir = ray.getRayDirection();
	const auto& rayOrigin = ray.getCameraOrigin();
	int materialIndex = mesh.getMaterialIndex();

	const CRTVector& normal = triangle.normalVector();
	const CRTVector& v0 = triangle.getVertices()[0];

	float projection = dotProduct(normal, rayDir);
	float distanceToPlane = dotProduct(normal, v0 - rayOrigin);

	if (areEqualFloats(projection, 0.f)) {
		return false; 
	}

	float t = distanceToPlane / projection;

	if (t <= 0.f || t > maxT || t >= result.t) {
		return false;
	}

	CRTVector intersectionPoint = rayOrigin + t * rayDir;

	if (!isPointInsideTriangle(intersectionPoint, triangle)) {
		return false;
	}

	result.t = t;
	result.hit = true;
	result.triangle = triangle;
	result.hitPoint = intersectionPoint;
	result.materialIndex = materialIndex;

	const auto& materials = scene.getMaterials();
	const auto& textures = scene.getTextures();
	const auto& albedoName = materials[materialIndex].getTextureName();

	if (!textures.empty()) {
		result.textureIndex = scene.getTextureIndex(albedoName);
	}

	calculateIntersectionData(result, mesh, materials);

	return true;
}
