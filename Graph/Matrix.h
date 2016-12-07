#pragma once
#include <cstdlib>
#include <iostream>
namespace Khellendros
{
	template<typename T = int>
	class Matrix
	{
	public:
		typedef typename T ValueType;
		typedef std::size_t SizeType;

		Matrix(SizeType rows, SizeType cols) :_rows(rows), _cols(cols) { init(); }
		Matrix(SizeType rows, SizeType cols,const ValueType& value) :_rows(rows), _cols(cols) 
		{
			init(); 
			setAll(value);
		}
		Matrix(const Matrix& matrix) :_rows(matrix.getRows()), _cols(matrix.getCols()) 
		{ init(); copy(matrix); }
		Matrix(Matrix&& tempMatrix):_matrix(nullptr)
		{
			swap(*this, tempMatrix);
		}
		~Matrix()
		{
			if (_matrix != nullptr) 
			{
				for (SizeType i = 0; i < _rows; ++i)
				{
					delete[] _matrix[i];
				}
				delete[] _matrix;
				_matrix = nullptr;
			}
		}
		SizeType getRows()const { return _rows; }
		SizeType getCols()const { return _cols; }
		SizeType getSize()const { return getRows()*getCols(); }
		void swap(Matrix& m)
		{
			std::swap(_rows, m._rows);
			std::swap(_cols, m._cols);
			std::swap(_matrix, m._matrix);
		}
		void setAll(const ValueType& value)
		{
			for (SizeType i = 0; i < _rows; ++i)
			{
				for (SizeType j = 0; j < _cols; ++j)
				{
					_matrix[i][j] = value;
				}
			}
		}
		void copy(const Matrix& m)
		{
			if (getRows() < m.getRows() || getCols() < m.getCols())
			{
				//如果矩阵尺寸太小，抛出异常
				throw std::logic_error("matrix's size out of range.");
				return;
			}
			for (SizeType i = 0; i < _rows; ++i)
			{
				for (SizeType j = 0; j < _cols; ++j)
				{
					_matrix[i][j] = m._matrix[i][j];
				}
			}
		}
		void move(Matrix&& m)
		{
			if (getRows() < m.getRows() || getCols() < m.getCols())
			{
				//如果矩阵尺寸太小，抛出异常
				throw std::logic_error("matrix's size out of range.");
				return;
			}
			for (SizeType i = 0; i < m.getRows(); ++i)
			{
				for (SizeType j = 0; j < m.getCols(); ++j)
				{
					_matrix[i][j] = std::move(m._matrix[i][j]);
				}
			}
		}
		ValueType*& operator[] (SizeType index)
		{
			return _matrix[index];
		}
		ValueType* operator[](SizeType index) const
		{
			return _matrix[index];
		}
		Matrix& operator=(const Matrix& matrix)
		{
			Matrix temp = matrix;
			swap(*this, matrix);
		}
		Matrix& operator=(Matrix&& tempMatrix)
		{
			swap(*this, tempMatrix);
		}
		template<typename T>
		friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& m);
	private:
		ValueType** _matrix;
		SizeType _rows;
		SizeType _cols;

		void init()
		{
			_matrix = new ValueType* [_rows];
			for (SizeType i = 0; i < _rows; ++i)
			{
				_matrix[i] = new ValueType[_cols];
			}
		}
	};
	template<typename T>
	void swap(Matrix<T>& lhs, Matrix<T>& rhs)
	{
		lhs.swap(rhs);
	}
	template<typename T>
	std::ostream& operator<<(std::ostream& os, const Matrix<T>& m)
	{
		os << '{' << std::endl;
		for (std::size_t i = 0; i < m.getRows(); ++i)
		{
			os << ' ';
			for (std::size_t j = 0; j < m.getCols(); ++j)
			{
				os << m[i][j] << ", ";
			}
			os << std::endl;
		}
		os << "}";
		return os;
	}
}