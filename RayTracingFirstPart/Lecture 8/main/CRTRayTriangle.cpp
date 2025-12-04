#include "CRTRayTriangle.h"

static CRTriangle createTriangle(const std::vector<int>& indices, const std::vector<CRTVector>& vertices) 
{
	assert(indices.size() == 3 && "Triangle indices must have exactly 3 elements");

	CRTVector v0 = vertices[indices[0]];
	CRTVector v1 = vertices[indices[1]];
	CRTVector v2 = vertices[indices[2]];
	return CRTriangle(v0, v1, v2);
}

CRTIntersectionResult CRTRayTriangle::intersectsRayTriangle(const CRTRay& ray, const CRTScene& scene)
{
	const CRTVector& rayDir = ray.getRayDirection();
	const CRTVector& rayOrigin = ray.getCameraOrigin();
	const std::vector<CRTMesh>& objects = scene.getObjects();

	CRTIntersectionResult result;

	for (const auto& mesh : objects) {

		const auto& vertices = mesh.getVertices();
		const auto& triangles = mesh.getTriangleIndices();

		for (const auto& triangleIndices : triangles) {

			CRTriangle triangle = createTriangle(triangleIndices, vertices);
			const CRTVector& normal = triangle.normalVector();
			const CRTVector& v0 = triangle.getVertices()[0];

			float dotNormalCameraRay = dotProduct(normal, rayDir);
			float dotNormalToVertex0 = dotProduct(normal, v0 - rayOrigin);

			if (areEqualFloats(dotNormalCameraRay, 0.f)) {
				continue;
			}

			float t = dotNormalToVertex0 / dotNormalCameraRay;

			if (t < 0) {
				continue;
			}

			CRTVector intersectionPoint = rayOrigin + t * rayDir;

			if (isPointInsideTriangle(intersectionPoint, triangle)) {

				if (t < result.t) {

					result.t = t;
					result.hit = true;
					result.triangle = triangle;
					result.hitPoint = intersectionPoint;
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

	bool checkE0 = dotProduct(normal, crossProduct(edge0, toP0)) > 0 ? true : false;
	bool checkE1 = dotProduct(normal, crossProduct(edge1, toP1)) > 0 ? true : false;
	bool checkE2 = dotProduct(normal, crossProduct(edge2, toP2)) > 0 ? true : false;

	return checkE0 && checkE1 && checkE2;
}