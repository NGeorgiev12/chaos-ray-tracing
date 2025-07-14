#pragma once
#include "CRTVector.h"

class CRTriangle
{
public:

	CRTriangle(const Point& first, const Point& second, const Point& third);
	CRTriangle(const CRTVector& first, const CRTVector& second);

	float getArea() const;

	CRTVector normalVector();

private:
	
	CRTVector firstBaseVec;
	CRTVector secondBaseVec;
	float area;

	void calculateArea();
};

