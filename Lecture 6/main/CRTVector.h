#pragma once
#include <iostream>
#include "Helper.h"
#include "CRTMatrix.h"
using namespace Helper;

class CRTVector
{
public:
	
	CRTVector();
	CRTVector(const Point& vertex);
	CRTVector(float x, float y, float z);

	void normalize();
	const Point& getVertex() const;
	float getLength() const;

	friend CRTVector crossProduct(const CRTVector& lhs, const CRTVector& rhs);
	friend float dotProduct(const CRTVector& lhs, const CRTVector& rhs);
	friend std::ostream& operator<<(std::ostream& os, const CRTVector& vec);

private:

	Point vertex;
	float length;
	
	void calculateLength();
};

CRTVector operator+(const CRTVector& lhs, const CRTVector& rhs);
CRTVector operator-(const CRTVector& lhs, const CRTVector& rhs);
CRTVector operator*(float scalar, const CRTVector& vec);
CRTVector operator*(const CRTVector& vec, float scalar);
bool operator==(const CRTVector& lhs, const CRTVector& rhs);
bool operator!=(const CRTVector& lhs, const CRTVector& rhs);

bool areCollinear(const CRTVector& lhs, const CRTVector& rhs);

