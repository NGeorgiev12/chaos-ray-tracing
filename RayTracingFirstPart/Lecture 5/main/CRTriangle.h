#pragma once
#include "CRTVector.h"

class CRTriangle
{
public:

	CRTriangle(const Point& first, const Point& second, const Point& third);
	CRTriangle(const CRTVector& v0, const CRTVector& v1, const CRTVector& v2);

	float getArea() const;
	CRTVector getFirstBaseVec() const;
	CRTVector getSecondBaseVec() const;
	const CRTVector* getVertices() const;
	const CRTVector& normalVector() const;

	void ensureCCWOrientation(const CRTVector& viewDir);

private:
	
	CRTVector vertices[TRIANGLE_VERTICES];
	float area;
	CRTVector normalVec;

	void calculateArea();
	void createNormal();
};

