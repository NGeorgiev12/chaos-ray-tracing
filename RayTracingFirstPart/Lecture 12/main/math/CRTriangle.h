#pragma once
#include "CRTVector.h"

class CRTriangle
{
public:

	CRTriangle() = default;
	CRTriangle(const Point& first, const Point& second, const Point& third);
	CRTriangle(const CRTVector& v0, const CRTVector& v1, const CRTVector& v2);

	CRTVector getFirstBaseVec() const;
	CRTVector getSecondBaseVec() const;
	const CRTVector* getVertices() const;

	const CRTVector& normalVector() const;
	float calculateArea() const;

private:
	
	CRTVector vertices[TRIANGLE_VERTICES];
	CRTVector normal;

	void calculateNormal();
};

