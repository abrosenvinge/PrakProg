#pragma once
#include <cmath>
#include <complex>
#include <cstddef>
#include <ostream>
#include <vector>
#include "math.hpp"

namespace pp {
	constexpr size_t DYNAMIC = 0;

	template <typename T, size_t ROWS, size_t COLS>
	class ColumnView;

	template <typename T, size_t ROWS, size_t COLS>
	class MatrixBase {
	public:
		size_t n_rows, n_cols;
		size_t size;

		virtual T& operator[](size_t i, size_t j) = 0;
		virtual const T& operator[](size_t i, size_t j) const = 0;

		ColumnView<T, ROWS, COLS> col(size_t j) {
			return ColumnView<T, ROWS, COLS>(*this, j);
		}

		MatrixBase& operator+=(const MatrixBase& other) {
			for (size_t j = 0; j < n_cols; ++j) {
				for (size_t i = 0; i < n_rows; ++i) {
					(*this)[i,j] += other[i, j];
				}
			}
			return *this;
		}

		MatrixBase& operator-=(const MatrixBase& other) {
			for (size_t j = 0; j < n_cols; ++j) {
				for (size_t i = 0; i < n_rows; ++i) {
					(*this)[i,j] -= other[i, j];
				}
			}
			return *this;
		}

		MatrixBase& operator*=(const T& a) {
			for (size_t j = 0; j < n_cols; ++j) {
				for (size_t i = 0; i < n_rows; ++i) {
					(*this)[i,j] *= a;
				}
			}
			return *this;
		}
		
		MatrixBase& operator/=(const T& a) {
			for (size_t j = 0; j < n_cols; ++j) {
				for (size_t i = 0; i < n_rows; ++i) {
					(*this)[i,j] /= a;
				}
			}
			return *this;
		}
	};

	template <typename T, size_t ROWS = DYNAMIC, size_t COLS = DYNAMIC>
	class Matrix : public MatrixBase<T, ROWS, COLS> {
	private:
		std::vector<T> data;
	public:
		using MatrixBase<T, ROWS, COLS>::n_rows, 
			  MatrixBase<T, ROWS, COLS>::n_cols, 
			  MatrixBase<T, ROWS, COLS>::size;
		Matrix(size_t n_rows, size_t n_cols) {
			if constexpr (ROWS != DYNAMIC) this->n_rows = ROWS;
			else this->n_rows = n_rows;

			if constexpr (COLS != DYNAMIC) this->n_cols = COLS;
			else this->n_cols = n_cols;
			
			size = this->n_cols * this->n_rows;
			data = std::vector<T>(size);
		};

		Matrix() : 
			data(COLS * ROWS), 
			MatrixBase<T,ROWS,COLS>::n_rows(ROWS), 
			MatrixBase<T,ROWS,COLS>::n_cols(COLS) {};

		Matrix(const MatrixBase<T,ROWS,COLS>& other) : Matrix(other.n_rows, other.n_cols) {
			for (size_t j = 0; j < n_cols; ++j) {
				for(size_t i = 0; i < n_rows; ++i) (*this)[i,j] = other[i,j];
			}
		}

		Matrix(const Matrix&) = default;
		Matrix(Matrix&&) = default;
		~Matrix() = default;
		
		// assignment
		Matrix& operator =(const Matrix&) = default; //copy: a = b
		Matrix& operator =(Matrix&&) = default; //move: a = b + c

		// column-major indexing
		T& operator[](size_t i, size_t j) override {
			return data[n_cols * j + i];
		}

		const T& operator[](size_t i, size_t j) const override {
			return data[n_cols * j + i];
		}
	};

	template <typename T, size_t ROWS, size_t COLS>
	Matrix<T, ROWS, COLS> operator+(const MatrixBase<T, ROWS, COLS>& a, const Matrix<T, ROWS, COLS>& b) {
		Matrix<T,ROWS,COLS> out(a);
		out += b;
		return out;
	}

	template <typename T, size_t ROWS, size_t COLS>
	Matrix<T, ROWS, COLS> operator-(const MatrixBase<T, ROWS, COLS>& a, const Matrix<T, ROWS, COLS>& b) {
		Matrix<T,ROWS,COLS> out(a);
		out -= b;
		return out;
	}

	template <typename T, size_t ROWS, size_t COLS>
	Matrix<T, ROWS, COLS> operator*(const T& c, const MatrixBase<T, ROWS, COLS>& a) {
		Matrix<T,ROWS,COLS> out(a);
		out *= c;
		return out;
	}

	template <typename T, size_t ROWS, size_t COLS>
	Matrix<T, ROWS, COLS> operator*(const MatrixBase<T, ROWS, COLS>& a, const T& c) {
		Matrix<T,ROWS,COLS> out(a);
		out *= c;
		return out;
	}

	template <typename T, size_t ROWS, size_t COLS>
	Matrix<T, ROWS, COLS> operator/(const MatrixBase<T, ROWS, COLS>& a, const T& c) {
		Matrix<T,ROWS,COLS> out(a);
		out /= c;
		return out;
	}

	template <typename T, size_t AROWS, size_t ACOLS, size_t BCOLS>
	Matrix<T, AROWS, BCOLS> mat_mult(const MatrixBase<T, AROWS, ACOLS>& a, const MatrixBase<T, ACOLS, BCOLS>& b) {
		size_t n_rows = a.n_rows;
		size_t n_cols = b.n_cols;
		size_t kmax = a.n_cols;

		Matrix<T, AROWS, BCOLS> out(n_rows, n_cols);

		for (size_t i = 0; i < n_rows; ++i) {
			for (size_t j = 0; j < n_cols; ++j) {
				for (size_t k = 0; k < kmax; ++k) {
					out[i,j] += a[i,k] * b[k,j];
				}
			}
		}
		return out;
	}

	template <typename T, size_t AROWS, size_t ACOLS, size_t BCOLS>
	inline Matrix<T, AROWS, BCOLS> operator*(const MatrixBase<T, AROWS, ACOLS>& a, const MatrixBase<T, ACOLS, BCOLS>& b) {
		return mat_mult(a,b);
	}

	template <typename T, size_t ROWS, size_t COLS>
	bool approx(const MatrixBase<T,ROWS,COLS>& a, const MatrixBase<T,ROWS,COLS> b) {
		bool result = true;
		for (size_t j = 0; (j < a.n_cols) && result; ++j) {
			for (size_t i = 0; (i < a.n_rows) && result; ++i) result &= approx(a[i,j], b[i,j]);
		}
	}

	template <typename T, size_t ROWS, size_t COLS>
	std::ostream& operator<<(std::ostream& s, const MatrixBase<T, ROWS, COLS>& m) {
		for (size_t i = 0; i < m.n_rows; ++i) {
			for (size_t j = 0; j < m.n_cols; ++j) {
				s << m[i,j];
				if (j < m.n_cols - 1) s << " ";
			}
			if (i < m.n_rows - 1) s << "\n";
		}
		return s;
	}

	template <typename T, size_t LEN>
	class VectorBase : public MatrixBase<T, LEN, 1> {
	public:
		using MatrixBase<T,LEN,1>::operator[];
		virtual T& operator[](size_t) = 0;
		virtual const T& operator[](size_t) const = 0;
	};

	template <typename T, size_t LEN>
	class Matrix<T, LEN, 1> : public VectorBase<T, LEN> {
	private:
		std::vector<T> data;
	public:
		using MatrixBase<T, LEN, 1>::n_rows, 
			  MatrixBase<T, LEN, 1>::n_cols, 
			  MatrixBase<T, LEN, 1>::size;

		/*
		 * This constructor is only here to support functions that work on matrices.
		*/
		Matrix(size_t n_rows, size_t) {
			if constexpr (LEN != DYNAMIC) this->n_rows = LEN;
			else this->n_rows = n_rows;
			this->n_cols = 1;
			size = this->n_cols * this->n_rows;
			data = std::vector<T>(size);
		}
		Matrix(size_t len) : Matrix(len, 1) {}
		Matrix(const MatrixBase<T,LEN,1>& other) : Matrix(other.n_rows, other.n_cols) {
			for (size_t j = 0; j < n_cols; ++j) {
				for(size_t i = 0; i < n_rows; ++i) (*this)[i,j] = other[i,j];
			}
		}

		T& operator[](size_t i) override {
			return data[i];
		}

		const T& operator[](size_t i) const override {
			return data[i];
		}

		// column-major indexing
		T& operator[](size_t i, size_t j) override {
			return data[n_cols * j + i];
		}

		const T& operator[](size_t i, size_t j) const override {
			return data[n_cols * j + i];
		}
	};

	template <typename T, size_t ROWS = DYNAMIC, size_t PARENT_COLS = DYNAMIC>
	class ColumnView : public VectorBase<T, ROWS> {
	private:
		MatrixBase<T, ROWS, PARENT_COLS>& parent;
		size_t col;
	public:
		using MatrixBase<T, ROWS, 1>::n_rows, MatrixBase<T, ROWS, 1>::n_cols;

		ColumnView(MatrixBase<T, ROWS, PARENT_COLS>& parent, size_t col) : 
			parent(parent),
			col(col) 
		{
			this->n_rows = parent.n_rows;
			this->n_cols = 1;
		}

		/*
		* These assignments are made like this to support operations like A.col(0) = B.col(1) etc.
		* They affect the parent matrix directly.
		*/
		ColumnView& operator =(const VectorBase<T, ROWS>& other) {
			for (size_t i = 0; i < n_rows; ++i) (*this)[i] = other[i];
			return *this;
		}; //copy: a = b
		ColumnView& operator =(const ColumnView<T, ROWS, PARENT_COLS>& other) {
			for (size_t i = 0; i < n_rows; ++i) (*this)[i] = other[i];
			return *this;
		};
		ColumnView& operator =(VectorBase<T,ROWS>&& other) {
			for (size_t i = 0; i < n_rows; ++i) (*this)[i] = other[i];
			return *this;
		} //move: a = b + c


		T& operator[](size_t i) override {
			return parent[i,col];
		}
		const T& operator[](size_t i) const override {
			return parent[i,col];
		}

		T& operator[](size_t i, size_t) override {
			return parent[i,col];
		}
		const T& operator[](size_t i, size_t) const override {
			return parent[i,col];
		}
	};

	template <typename T, size_t LEN>
	T dot(const VectorBase<T,LEN>& a, const VectorBase<T,LEN>& b) {
		T out = 0.;
		for (size_t i = 0; i < a.size; ++i) {
			out += a[i] * b[i];
		}
		return out;
	}

	template <typename T, size_t LEN>
	std::complex<T> dot(const VectorBase<std::complex<T>, LEN>& a, const VectorBase<std::complex<T>, LEN>& b) {
		std::complex<T> out(0.);
		for (size_t i = 0; i < a.size; ++i) {
			out += std::conj(a[i]) * b[i];
		}
		return out;
	}

	template <typename T, size_t LEN>
	T norm(const VectorBase<T,LEN>& a) {
		return std::sqrt(dot(a,a));
	}

	template <typename T, size_t LEN = DYNAMIC>
	using Vector = Matrix<T, LEN, 1>;

	template <typename T, size_t LEN = DYNAMIC>
	using RowVector = Matrix<T, 1, LEN>;

	template <typename T>
	using VectorC = Vector<std::complex<T>>;
	
	template <typename T>
	using MatrixC = Matrix<std::complex<T>>;
}
