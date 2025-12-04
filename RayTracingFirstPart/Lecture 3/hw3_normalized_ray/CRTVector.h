#pragma once
#include <iostream>

struct Point {

	float x;
	float y;
	float z;
};

class CRTVector
{
public:
	
	CRTVector(const Point& vertex);
	CRTVector(float x, float y, float z);

	void normalize();
	const Point& getVertex() const;

private:
	Point origin;
	Point vertex;
	float length;
	
	void calculateLength();
};

