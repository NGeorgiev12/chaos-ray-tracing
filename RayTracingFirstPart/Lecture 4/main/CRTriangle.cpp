#include "CRTriangle.h"
#include <assert.h>

CRTriangle::CRTriangle(const Point& first, const Point& second, const Point& third)
{
	CRTVector v0(first);
	CRTVector v1(second);
	CRTVector v2(third);

	CRTVector a = v1 - v0;
	CRTVector b = v2 - v0;

	assert(!areCollinear(a, b));

	firstBaseVec = a;
	secondBaseVec = b;

	calculateArea();
}

CRTriangle::CRTriangle(const CRTVector& first, const CRTVector& second)
{
	assert(!areCollinear(first, second));
	firstBaseVec = first;
	secondBaseVec = second;
	calculateArea();
}

float CRTriangle::getArea() const
{
	return area;
}

CRTVector CRTriangle::normalVector()
{
	CRTVector normal = crossProduct(firstBaseVec, secondBaseVec);
	normal.normalize();
	return normal;
}

void CRTriangle::calculateArea()
{
	CRTVector crossVec = crossProduct(firstBaseVec, secondBaseVec);
	area = crossVec.getLength() / 2.0f;
}
