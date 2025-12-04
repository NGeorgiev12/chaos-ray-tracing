#include "CRTMatrix.h"
#include <assert.h>

static std::vector<std::vector<float>> multiplyMatrices(const std::vector<std::vector<float>>& lhs, const std::vector<std::vector<float>>& rhs)
{
	std::vector<std::vector<float>> result(lhs.size(), std::vector<float>(rhs[0].size(), 0.0f));

	for (int i = 0; i < result.size(); i++) {

		assert(lhs[i].size() == rhs.size());

		for (int j = 0; j < result[0].size(); j++) {

			result[i][j] = 0.0f;

			for (int k = 0; k < lhs[i].size(); k++) {

				result[i][j] += lhs[i][k] * rhs[k][j];
			}

		}
	}
	return result;
}

CRTMatrix CRTMatrix::rotationX(float radians) {
	float cosA = cosf(radians);
	float sinA = sinf(radians);

	return CRTMatrix({
		{1.0f,  0.0f,   0.0f}, 
		{0.0f,  cosA,  -sinA},  
		{0.0f,  sinA,   cosA}   
		});
}

CRTMatrix CRTMatrix::rotationY(float radians) {
	float cosA = cosf(radians);
	float sinA = sinf(radians);

	return CRTMatrix({
		{cosA,  0.0f,  sinA},  
		{0.0f,  1.0f,  0.0f}, 
		{-sinA, 0.0f,  cosA}  
		});
}

CRTMatrix CRTMatrix::rotationZ(float radians) {
	float cosA = cosf(radians);
	float sinA = sinf(radians);

	return CRTMatrix({
		{cosA,  -sinA,  0.0f},
		{sinA,   cosA,  0.0f},
		{0.0f,   0.0f,  1.0f}
		});
}

CRTMatrix CRTMatrix::identity()
{
	std::vector<std::vector<float>> identityData {
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f}
	};
	return CRTMatrix(identityData);
}

CRTMatrix::CRTMatrix() : numbers(ROWS_COUNT, std::vector<float>(COLS_COUNT, 0.0f))
{
}

CRTMatrix::CRTMatrix(const std::vector<std::vector<float>>& numbers) : numbers(numbers)
{
}

CRTMatrix CRTMatrix::loadJSONMatrix(const Value::ConstArray& arr)
{
	assert(arr.Size() == ROWS_COUNT * COLS_COUNT && "JSON array must have 9 elements for a 3x3 matrix");	std::vector<std::vector<float>> matrixData(ROWS_COUNT, std::vector<float>(COLS_COUNT));
	for (int i = 0; i < matrixData.size(); i++) {

		for (int j = 0; j < matrixData[i].size(); j++) {

			matrixData[i][j] = arr[i * COLS_COUNT + j].GetFloat();

		}

	}
	return CRTMatrix(matrixData);
}

const std::vector<std::vector<float>>& CRTMatrix::getNumbers() const
{
	return numbers;
}

CRTMatrix& CRTMatrix::operator*=(float scalar)
{
	for (auto& row : numbers) {

		for (auto& elem : row) {
			elem *= scalar;
		}
	}
	return *this;
}

CRTMatrix& CRTMatrix::operator+=(const CRTMatrix& other)
{
	for (int i = 0; i < numbers.size(); i++) {

		assert(numbers[i].size() == other.numbers[i].size());

		for (int j = 0; j < numbers[i].size(); j++) {

			numbers[i][j] += other.numbers[i][j];
		}
	}
	return *this;
}

CRTMatrix& CRTMatrix::operator-=(const CRTMatrix& other)
{
	for (int i = 0; i < numbers.size(); i++) {

		assert(numbers[i].size() == other.numbers[i].size());

		for (int j = 0; j < numbers[i].size(); j++) {
			numbers[i][j] -= other.numbers[i][j];
		}
	}
	return *this;
}

void CRTMatrix::print() const
{
	for (const auto& row : numbers) {

		for (const auto& elem : row) {

			std::cout << elem << " ";
		}

		std::cout << std::endl;
	}
}

std::vector<std::vector<float>> CRTMatrix::createSubmatrix(const CRTMatrix& mtx, int excludeRow, int excludeCol)
{
	std::vector<std::vector<float>> subMtx;
	auto& numbers = mtx.getNumbers();
	int rows = numbers.size();
	int cols = numbers[0].size();
	
	for (int i = 0; i < rows; i++) {
	
		if (i == excludeRow) {
			continue;
		}
	
		std::vector<float> row;
	
		for (int j = 0; j < cols; j++) {
	
			if (j == excludeCol) {
				continue;
			}
	
			row.push_back(mtx.getNumbers()[i][j]);
		}
	
		subMtx.push_back(row);
	}
	
	return subMtx;
}

CRTMatrix operator*(const CRTMatrix& lhs, const CRTMatrix& rhs)
{
	auto result = multiplyMatrices(lhs.getNumbers(), rhs.getNumbers());
	return CRTMatrix(result);
}

CRTVector operator*(const CRTVector& lhs, const CRTMatrix& rhs)
{
	std::vector<float> vecNumbers = { lhs.getVertex().x, lhs.getVertex().y, lhs.getVertex().z };
	std::vector<std::vector<float>> vecDataToMtx{ vecNumbers };

	auto mtxResult = multiplyMatrices(vecDataToMtx, rhs.getNumbers());
	auto& vecResult = mtxResult[0];

	return CRTVector(vecResult[0], vecResult[1], vecResult[2]);
}

CRTMatrix operator*(const CRTMatrix& mtx, float scalar)
{
	CRTMatrix result = mtx;
	result *= scalar;
	return result;
}

CRTMatrix operator*(float scalar, const CRTMatrix& mtx)
{
	return mtx * scalar;
}

CRTMatrix operator+(const CRTMatrix& lhs, const CRTMatrix& rhs)
{
	CRTMatrix result = lhs;
	result += rhs;
	return result;
}

CRTMatrix operator-(const CRTMatrix& lhs, const CRTMatrix& rhs)
{
	CRTMatrix result = lhs;
	result -= rhs;
	return result;
}

float calculateDeterminant(const CRTMatrix& mtx)
{
	auto& numbers = mtx.getNumbers();
	float firstElem = numbers[0][0];

	if (numbers.size() == 1) {
		return firstElem;
	}

	if (numbers.size() == 2) {
		return (mtx.getNumbers()[0][0] * mtx.getNumbers()[1][1])
			- (mtx.getNumbers()[1][0] * mtx.getNumbers()[0][1]);
	}

	float det = 0.0f;

	for (int i = 0; i < numbers.size(); i++) {

		int sign = (i % 2 == 0) ? 1 : -1;

		auto subMatrix = CRTMatrix::createSubmatrix(mtx, 0, i);
		float currentNumber = numbers[0][i];

		det += sign * currentNumber * calculateDeterminant(CRTMatrix(subMatrix));
	}

	return det;
}

