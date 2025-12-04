#pragma once
#include <iostream>
#include "Helper.h"
#include "CRTMatrix.h"
#include "rapidjson/document.h"
using namespace Helper;
using namespace rapidjson;

class CRTVector
{
public:
	
	CRTVector();
	CRTVector(const Point& vertex);
	CRTVector(float x, float y, float z);
	static CRTVector loadJSONVector(const Value::ConstArray& arr);
	static CRTVector multiplyColors(const CRTVector& lhs, const CRTVector& rhs);

	void normalize();
	const Point& getVertex() const;
	float getLength() const;
	float getComponent(int index) const;

	friend CRTVector crossProduct(const CRTVector& lhs, const CRTVector& rhs);
	friend float dotProduct(const CRTVector& lhs, const CRTVector& rhs);
	friend std::ostream& operator<<(std::ostream& os, const CRTVector& vec);

	CRTVector& operator+=(const CRTVector& other);
	CRTVector& operator-=(const CRTVector& other);
	CRTVector& operator*=(float scalar);

private:

	Point vertex;	
};

CRTVector operator+(const CRTVector& lhs, const CRTVector& rhs);
CRTVector operator-(const CRTVector& lhs, const CRTVector& rhs);
CRTVector operator*(float scalar, const CRTVector& vec);
CRTVector operator*(const CRTVector& vec, float scalar);

bool operator==(const CRTVector& lhs, const CRTVector& rhs);
bool operator!=(const CRTVector& lhs, const CRTVector& rhs);

bool areCollinear(const CRTVector& lhs, const CRTVector& rhs);

