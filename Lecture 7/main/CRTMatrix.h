#pragma once  
#include <iostream>  
#include <vector>    
#include "CRTVector.h"
#include "rapidjson/document.h"
using namespace rapidjson;

class CRTVector;

class CRTMatrix
{
public:

	CRTMatrix();
	CRTMatrix(const std::vector<std::vector<float>>& numbers);
	static CRTMatrix loadJSONMatrix(const Value::ConstArray& arr);

	const std::vector<std::vector<float>>& getNumbers() const;

	CRTMatrix& operator*=(float scalar);
	CRTMatrix& operator+=(const CRTMatrix& other);
	CRTMatrix& operator-=(const CRTMatrix& other);
	
	void print() const;
	static std::vector<std::vector<float>> createSubmatrix(const CRTMatrix& mtx, int excludeRow, int excludeCol);
	static CRTMatrix rotationX(float radians);
	static CRTMatrix rotationY(float radians);
	static CRTMatrix rotationZ(float radians);
	static CRTMatrix identity();

private:
	std::vector<std::vector<float>> numbers;
};

float calculateDeterminant(const CRTMatrix& mtx);

CRTMatrix operator*(const CRTMatrix& lhs, const CRTMatrix& rhs);
CRTVector operator*(const CRTVector& lhs, const CRTMatrix& rhs);
CRTMatrix operator*(const CRTMatrix& mtx, float scalar);
CRTMatrix operator*(float scalar, const CRTMatrix& mtx);
CRTMatrix operator+(const CRTMatrix& lhs, const CRTMatrix& rhs);
CRTMatrix operator-(const CRTMatrix& lhs, const CRTMatrix& rhs);
