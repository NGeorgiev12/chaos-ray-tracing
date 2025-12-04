#include "CRTVector.h"

static Matrix createMatrixData(const CRTVector& lhs, const CRTVector& rhs) 
{
	std::vector<std::vector<float>> data;
	float lhsCoord[] = { lhs.getVertex().x, lhs.getVertex().y, lhs.getVertex().z };
	float rhsCoord[] = { rhs.getVertex().x, rhs.getVertex().y, rhs.getVertex().z };

	for (int i = 0; i < 3; i++) {
		
		std::vector<float> row;
		row.push_back(UNIT_VEC_LENGTH);
		row.push_back(lhsCoord[i]);
		row.push_back(rhsCoord[i]);
		data.push_back(row);
	}

	return Matrix(3, 3, data);
}

CRTVector::CRTVector() : CRTVector(0, 0, 0)
{
}

CRTVector::CRTVector(const Point& vertex) : vertex(vertex)
{
	calculateLength();
}

CRTVector::CRTVector(float x, float y, float z) : vertex{x, y, z}
{
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

float CRTVector::getLength() const
{
	return length;
}

void CRTVector::calculateLength()
{
	float vertexSquare = vertex.x * vertex.x + vertex.y * vertex.y + vertex.z * vertex.z;
	length = sqrt(vertexSquare);
}

CRTVector crossProduct(const CRTVector& lhs, const CRTVector& rhs)
{
	Matrix crossData = createMatrixData(lhs, rhs);
	float coords[3];

	for (int i = 0; i < 3; i++) {

		auto curMtxData = createSubmatrix(crossData, i, 0);
		Matrix curMtx(2, 2, curMtxData);
		int sign = (i % 2 == 0) ? 1 : -1;
		coords[i] = sign * calculateDeterminant(curMtx);

	}

	return CRTVector(coords[0], coords[1], coords[2]);
}

float dotProduct(const CRTVector& lhs, const CRTVector& rhs)
{
	return lhs.getVertex().x * rhs.getVertex().x +
		lhs.getVertex().y * rhs.getVertex().y +
		lhs.getVertex().z * rhs.getVertex().z;
}

std::ostream& operator<<(std::ostream& os, const CRTVector& vec)
{
	return os << vec.vertex.x << ", " << vec.vertex.y << ", " << vec.vertex.z;
}

CRTVector operator+(const CRTVector& lhs, const CRTVector& rhs)
{
	float newX = lhs.getVertex().x + rhs.getVertex().x;
	float newY = lhs.getVertex().y + rhs.getVertex().y;
	float newZ = lhs.getVertex().z + rhs.getVertex().z;
	return CRTVector(newX, newY, newZ);
}

CRTVector operator-(const CRTVector& lhs, const CRTVector& rhs)
{
	float newX = lhs.getVertex().x - rhs.getVertex().x;
	float newY = lhs.getVertex().y - rhs.getVertex().y;
	float newZ = lhs.getVertex().z - rhs.getVertex().z;
	return CRTVector(newX, newY, newZ);
}

CRTVector operator*(float scalar, const CRTVector& vec)
{
	float newX = vec.getVertex().x * scalar;
	float newY = vec.getVertex().y * scalar;
	float newZ = vec.getVertex().z * scalar;
	return CRTVector(newX, newY, newZ);
}

CRTVector operator*(const CRTVector& vec, float scalar)
{
	return scalar * vec;
}

bool operator==(const CRTVector& lhs, const CRTVector& rhs)
{
	return lhs.getVertex() == rhs.getVertex();
}

bool operator!=(const CRTVector& lhs, const CRTVector& rhs)
{
	return !(lhs == rhs);
}

bool areCollinear(const CRTVector& lhs, const CRTVector& rhs)
{
	float proportion1 = lhs.getVertex().x / rhs.getVertex().x;
	float proportion2 = lhs.getVertex().y / rhs.getVertex().y;
	float proportion3 = lhs.getVertex().z / rhs.getVertex().z;

	return areEqualFloats(proportion1, proportion2) && areEqualFloats(proportion1, proportion3);
}
