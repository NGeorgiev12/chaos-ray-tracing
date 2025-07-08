#include "CRTVector.h"

CRTVector::CRTVector(const Point& vertex) : origin{ 0.f, 0.f, 0.f }
{
	this->vertex = vertex;
	calculateLength();
}

CRTVector::CRTVector(float x, float y, float z) : origin{ 0.f, 0.f, 0.f }
{
	vertex.x = x;
	vertex.y = y;
	vertex.z = z;
	calculateLength();
}

void CRTVector::normalize()
{
	vertex.x /= length;
	vertex.y /= length;
	vertex.z /= length;
	length = 1;
}

const Point& CRTVector::getVertex() const
{
	return vertex;
}

void CRTVector::calculateLength()
{
	float originSquare = origin.x * origin.x + origin.y * origin.y + origin.z * origin.z;
	float vertexSquare = vertex.x * vertex.x + vertex.y * vertex.y + vertex.z * vertex.z;
	length = sqrt(vertexSquare - originSquare);
}
