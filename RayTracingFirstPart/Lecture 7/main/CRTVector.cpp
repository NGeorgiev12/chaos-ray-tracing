#include "CRTVector.h"

static CRTMatrix createMatrixData(const CRTVector& lhs, const CRTVector& rhs) 
{
	std::vector<std::vector<float>> data;
	float lhsCoord[] = { lhs.getVertex().x, lhs.getVertex().y, lhs.getVertex().z };
	float rhsCoord[] = { rhs.getVertex().x, rhs.getVertex().y, rhs.getVertex().z };

	for (int i = 0; i < ROWS_COUNT; i++) {
		
		std::vector<float> row;
		row.push_back(UNIT_VEC_LENGTH);
		row.push_back(lhsCoord[i]);
		row.push_back(rhsCoord[i]);
		data.push_back(row);
	}

	return CRTMatrix(data);
}

CRTVector::CRTVector() : CRTVector(0, 0, 0)
{
}

CRTVector::CRTVector(const Point& vertex) : vertex(vertex)
{
}

CRTVector::CRTVector(float x, float y, float z) : vertex{x, y, z}
{
}

void CRTVector::normalize()
{
	float length = getLength();
	vertex.x /= length;
	vertex.y /= length;
	vertex.z /= length;
}

const Point& CRTVector::getVertex() const
{
	return vertex;
}

CRTVector& CRTVector::operator+=(const CRTVector& other)
{
	vertex.x += other.getVertex().x;
	vertex.y += other.getVertex().y;
	vertex.z += other.getVertex().z;
	return *this;
}

CRTVector& CRTVector::operator-=(const CRTVector& other)
{
	vertex.x -= other.getVertex().x;
	vertex.y -= other.getVertex().y;
	vertex.z -= other.getVertex().z;
	return *this;
}

CRTVector& CRTVector::operator*=(float scalar) 
{
	vertex.x *= scalar;
	vertex.y *= scalar;
	vertex.z *= scalar;
	return *this;
}

CRTVector CRTVector::loadJSONVector(const Value::ConstArray& arr) {

    assert(arr.Size() == 3 && "JSON array must have exactly 3 elements to form a vector");
    return CRTVector(arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat());
}

float CRTVector::getLength() const
{
	float vertexSquare = vertex.x * vertex.x + vertex.y * vertex.y + vertex.z * vertex.z;
	return sqrt(vertexSquare);
}

CRTVector crossProduct(const CRTVector& lhs, const CRTVector& rhs)
{
	CRTMatrix crossData = createMatrixData(lhs, rhs);
	float coords[3]{};

	for (int i = 0; i < 3; i++) {

		auto curMtxData = CRTMatrix::createSubmatrix(crossData, i, 0);
		CRTMatrix curMtx(curMtxData);
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
	CRTVector result(lhs);
	result += rhs;
	return result;
}

CRTVector operator-(const CRTVector& lhs, const CRTVector& rhs)
{
	CRTVector result(lhs);
	result -= rhs;
	return result;
}

CRTVector operator*(float scalar, const CRTVector& vec)
{
	CRTVector result(vec);
	result *= scalar;
	return result;
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
