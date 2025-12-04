#include "CRTriangle.h"
#include <assert.h>

CRTriangle::CRTriangle(const Point& first, const Point& second, const Point& third) : 
	CRTriangle(CRTVector(first), CRTVector(second), CRTVector(third))
{
}

CRTriangle::CRTriangle(const CRTVector& v0, const CRTVector& v1, const CRTVector& v2) 
{
	CRTVector a = v1 - v0;
	CRTVector b = v2 - v0;

	assert(!areCollinear(a, b));

	vertices[0] = v0;
	vertices[1] = v1;
	vertices[2] = v2;
	calculateNormal();
}

CRTVector CRTriangle::getFirstBaseVec() const
{
	return vertices[1] - vertices[0];
}

CRTVector CRTriangle::getSecondBaseVec() const
{
	return vertices[2] - vertices[0];
}

const CRTVector* CRTriangle::getVertices() const
{
	return vertices;
}

const CRTVector& CRTriangle::normalVector() const
{
	return normal;
}

float CRTriangle::calculateArea() const
{
	CRTVector crossVec = crossProduct(getFirstBaseVec(), getSecondBaseVec());
	return crossVec.getLength() / 2.0f;
}

void CRTriangle::calculateNormal()
{
	normal = crossProduct(getFirstBaseVec(), getSecondBaseVec());
	normal.normalize();
}

