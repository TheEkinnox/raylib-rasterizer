#include "Matrix.h"

#include <stdexcept>

using namespace LibMath::Exceptions;

namespace LibMath
{
	Matrix2x2::Matrix2x2() : Matrix(2, 2)
	{
	}

	Matrix2x2::Matrix2x2(const float scalar) : Matrix(2, 2, scalar)
	{
	}

	Matrix2x3::Matrix2x3() : Matrix(2, 3)
	{
	}

	Matrix2x3::Matrix2x3(const float scalar) : Matrix(2, 3, scalar)
	{
	}

	Matrix2x4::Matrix2x4() : Matrix(2, 4)
	{
	}

	Matrix2x4::Matrix2x4(const float scalar) : Matrix(2, 4, scalar)
	{
	}

	Matrix3x2::Matrix3x2() : Matrix(3, 2)
	{
	}

	Matrix3x2::Matrix3x2(const float scalar) : Matrix(3, 2, scalar)
	{
	}

	Matrix3x3::Matrix3x3() : Matrix(3, 3)
	{
	}

	Matrix3x3::Matrix3x3(const float scalar) : Matrix(3, 3, scalar)
	{
	}

	Matrix3x4::Matrix3x4() : Matrix(3, 4)
	{
	}

	Matrix3x4::Matrix3x4(const float scalar) : Matrix(3, 4, scalar)
	{
	}

	Matrix4x2::Matrix4x2() : Matrix(4, 2)
	{
	}

	Matrix4x2::Matrix4x2(const float scalar) : Matrix(4, 2, scalar)
	{
	}

	Matrix4x3::Matrix4x3() : Matrix(4, 3)
	{
	}

	Matrix4x3::Matrix4x3(const float scalar) : Matrix(4, 3, scalar)
	{
	}

	Matrix4x4::Matrix4x4() : Matrix(4, 4)
	{
	}

	Matrix4x4::Matrix4x4(const float scalar) : Matrix(4, 4, scalar)
	{
	}

	Matrix::Matrix(const length_t rows, const length_t columns)
	{
		if (rows == 0 || columns == 0)
			throw std::invalid_argument("Invalid matrix size");

		m_rows = rows;
		m_columns = columns;

		const length_t size = m_rows * m_columns;

		m_values = new float[size]();

		// Builds a matrix filled with zeros
		for (length_t i = 0; i < size; i++)
			m_values[i] = 0;
	}

	Matrix::Matrix(const length_t rows, const length_t columns, const float scalar)
	{
		if (rows == 0 || columns == 0)
			throw std::invalid_argument("Invalid matrix size");

		m_rows = rows;
		m_columns = columns;

		const length_t size = m_rows * m_columns;

		m_values = new float[size]();

		// Builds a diagonal matrix with the given scalar
		for (length_t i = 0; i < size; i++)
		{
			const length_t row = i / m_columns;
			const length_t col = i % m_columns;

			m_values[i] = row == col ? scalar : 0;
		}
	}

	Matrix::length_t Matrix::getIndex(const length_t row, const length_t column) const
	{
		return row * m_columns + column;
	}

	Matrix::~Matrix()
	{
		delete[] m_values;
	}

	Matrix::Matrix(Matrix const& other)
	{
		const length_t size = other.m_columns * other.m_rows;
		m_values = new float[size]();

		for (length_t i = 0; i < size; i++)
			m_values[i] = other[i];

		m_columns = other.m_columns;
		m_rows = other.m_rows;
	}

	Matrix::Matrix(Matrix&& other) noexcept
	{
		m_columns = other.m_columns;
		m_rows = other.m_rows;
		m_values = other.m_values;

		other.m_columns = other.m_rows = 0;
		other.m_values = nullptr;
	}

	Matrix& Matrix::operator=(Matrix const& other)
	{
		if (this == &other)
			return *this;

		delete[] m_values;

		const length_t size = other.m_columns * other.m_rows;
		m_values = new float[size]();

		for (length_t i = 0; i < size; i++)
			m_values[i] = other[i];

		m_columns = other.m_columns;
		m_rows = other.m_rows;

		return *this;
	}

	Matrix& Matrix::operator=(Matrix&& other) noexcept
	{
		if (this == &other)
			return *this;

		delete[] m_values;

		m_columns = other.m_columns;
		m_rows = other.m_rows;
		m_values = other.m_values;

		other.m_columns = other.m_rows = 0;
		other.m_values = nullptr;

		return *this;
	}

	float Matrix::operator[](const length_t index) const
	{
		if (index < 0 || index >= m_rows * m_columns)
			throw std::out_of_range("Index out of range");

		return m_values[index];
	}

	float& Matrix::operator[](const length_t index)
	{
		if (index < 0 || index >= m_rows * m_columns)
			throw std::out_of_range("Index out of range");

		return m_values[index];
	}

	Matrix& Matrix::operator+=(Matrix const& other)
	{
		if (other.m_columns != m_columns || other.m_rows != m_rows)
			throw IncompatibleMatrix();

		for (length_t i = 0; i < other.m_columns * other.m_rows; i++)
			m_values[i] += other[i];

		return *this;
	}

	Matrix& Matrix::operator-=(Matrix const& other)
	{
		if (other.m_columns != m_columns || other.m_rows != m_rows)
			throw IncompatibleMatrix();

		for (length_t i = 0; i < other.m_columns * other.m_rows; i++)
			m_values[i] -= other[i];

		return *this;
	}

	Matrix& Matrix::operator+=(const float scalar)
	{
		for (length_t i = 0; i < m_columns * m_rows; i++)
			m_values[i] += scalar;

		return *this;
	}

	Matrix& Matrix::operator-=(const float scalar)
	{
		for (length_t i = 0; i < m_columns * m_rows; i++)
			m_values[i] -= scalar;

		return *this;
	}

	Matrix& Matrix::operator*=(const float scalar)
	{
		for (length_t i = 0; i < m_columns * m_rows; i++)
			m_values[i] *= scalar;

		return *this;
	}

	Matrix& Matrix::operator/=(const float scalar)
	{
		for (length_t i = 0; i < m_columns * m_rows; i++)
			m_values[i] /= scalar;

		return *this;
	}

	Matrix Matrix::operator+(Matrix const& other) const
	{
		Matrix mat = *this;
		return mat += other;
	}

	Matrix Matrix::operator-(Matrix const& other) const
	{
		Matrix mat = *this;
		return mat -= other;
	}

	Matrix Matrix::operator*(Matrix const& other) const
	{
		if (other.m_rows != m_columns)
			throw IncompatibleMatrix();

		Matrix result(m_rows, other.m_columns);

		for (length_t i = 0; i < other.m_columns; i++)
		{
			for (length_t j = 0; j < m_rows; j++)
			{
				float scalar = 0;

				for (length_t k = 0; k < m_columns; k++)
				{
					const length_t curIndex = getIndex(j, k);
					const length_t otherIndex = other.getIndex(k, i);

					scalar += m_values[curIndex] * other[otherIndex];
				}

				const length_t resultIndex = j * result.m_columns * i;
				result[resultIndex] = scalar;
			}
		}

		return result;
	}

	Matrix Matrix::operator/(Matrix const& other) const
	{
		return *this * other.inverse();
	}

	Matrix Matrix::operator+(const float scalar) const
	{
		Matrix mat = *this;
		return mat += scalar;
	}

	Matrix Matrix::operator-(const float scalar) const
	{
		Matrix mat = *this;
		return mat -= scalar;
	}

	Matrix Matrix::operator*(const float scalar) const
	{
		Matrix mat = *this;
		return mat *= scalar;
	}

	Matrix Matrix::operator/(const float scalar) const
	{
		Matrix mat = *this;
		return mat /= scalar;
	}

	Matrix::length_t Matrix::getRowCount() const
	{
		return m_rows;
	}

	Matrix::length_t Matrix::getColumnCount() const
	{
		return m_columns;
	}

	float Matrix::cofactor(const length_t row, const length_t column) const
	{
		if (m_rows != m_columns)
			throw NonSquareMatrix();

		// The multiplier is (-1)^(i+j) so 1 when i + j is pair and -1 otherwise
		const float multiplier = (row + column) % 2 == 0 ? 1.f : -1.f;

		return multiplier * minor(row, column).determinant();
	}

	Matrix Matrix::minor(const length_t row, const length_t column) const
	{
		Matrix minor(m_rows - 1, m_columns - 1);
		length_t minorLine = 0, minorCol = 0;

		for (length_t i = 0; i < m_rows; i++)
		{
			if (i == row)
				continue;

			for (length_t j = 0; j < m_columns; j++)
			{
				if (j == column)
					continue;

				minor[getIndex(minorLine, minorCol++)];
			}

			minorLine++;
		}

		return minor;
	}

	float Matrix::determinant2x2() const
	{
		// 0 1
		// 2 3
		return (*this)[0] * (*this)[2] - (*this)[1] * (*this)[3];
	}

	float Matrix::determinant3x3() const
	{
		// 0 1 2
		// 3 4 5
		// 6 7 8
		const float positive = (*this)[0] * (*this)[4] * (*this)[8] +
			(*this)[1] * (*this)[5] * (*this)[6] +
			(*this)[2] * (*this)[3] * (*this)[7];

		const float negative = (*this)[2] * (*this)[4] * (*this)[6] +
			(*this)[1] * (*this)[3] * (*this)[8] +
			(*this)[0] * (*this)[5] * (*this)[7];

		return positive - negative;
	}

	float Matrix::determinant() const
	{
		if (m_rows != m_columns)
			throw NonSquareMatrix();

		if (m_rows == 1)
			return m_values[0];

		if (m_rows == 2)
			return determinant2x2();

		if (m_rows == 3)
			return determinant3x3();

		float determinant = 0;

		for (length_t col = 0; col < m_columns; col++)
			determinant += m_values[col] * cofactor(0, col);

		return determinant;
	}

	Matrix Matrix::transpose() const
	{
		if (m_rows != m_columns)
			throw NonSquareMatrix();

		Matrix transposed(m_columns, m_rows);

		for (length_t i = 0; i < m_rows; i++)
			for (length_t j = 0; j < m_columns; j++)
				transposed[transposed.getIndex(j, i)] = m_values[getIndex(i, j)];

		return transposed;
	}

	Matrix Matrix::coMatrix() const
	{
		if (m_rows != m_columns)
			throw NonSquareMatrix();

		Matrix coMatrix(m_rows, m_columns);

		for (length_t row = 0; row < m_rows; row++)
			for (length_t col = 0; col < m_columns; col++)
				coMatrix[getIndex(row, col)] = cofactor(row, col);

		return coMatrix;
	}

	Matrix Matrix::adjugate() const
	{
		return coMatrix().transpose();
	}

	Matrix Matrix::inverse() const
	{
		if (m_rows != m_columns)
			throw NonSquareMatrix();

		const float det = determinant();

		if (det == 0.f)
			throw NonInvertibleMatrix();

		return adjugate() / det;
	}
}
