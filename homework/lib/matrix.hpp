#pragma once
#include <cmath>
#include <complex>
#include <cstddef>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <vector>
#include <random>

namespace pp {
	template <typename S> class VectorBase;
	template <typename S> class Vector;
	template <typename S> class ColumnView;
	template <typename S> class TransposeView;
	template <typename S> class Matrix;
	template <typename S> class MatrixBase;

	template <typename S>
	bool approx(const S& a, const S& b, double acc = 1e-9, double eps = 1e-9);

	template <typename S>
	bool mat_approx(const MatrixBase<S>& a, const MatrixBase<S>& b, double acc = 1e-9, double eps = 1e-9);
	
	template <typename S>
	bool mat_approx(const VectorBase<S>& a, const VectorBase<S>& b, double acc = 1e-9, double eps = 1e-9);

	template <typename S>
	class MatrixBase {
	public:
		size_t n_rows, n_cols;
		size_t size;

		virtual S& operator[](size_t i, size_t j) = 0;
		virtual const S& operator[](size_t i, size_t j) const = 0;

		ColumnView<S> col(size_t j) {
			return ColumnView<S>(*this, j);
		}

		Vector<S> col_copy(size_t j) {
			Vector<S> out(n_rows);
			for (size_t i = 0; i < n_rows; ++i) out[i] = (*this)[i,j];
			return out;
		}

		TransposeView<S> T() {
			return TransposeView<S>(*this);
		}

		bool is_utriangular() const {
			bool result = n_rows == n_cols; // should be square
			for (size_t j = 0; (j < n_cols) && result; ++j) {
				for (size_t i = j + 1; (i < n_rows) && result; ++i) {
					result &= approx((*this)[i,j], 0.);
				}
			}
			return result;
		}

		bool is_square() const {
			return n_rows == n_cols;
		}

		Matrix<S> T_copy() {
			Matrix<S> out(n_cols, n_rows);
			for (size_t j = 0; j < n_cols; ++j) {
				for (size_t i = 0; i < n_rows; ++i) {
					out[j,i] = (*this)[i,j];
				}
			}
			return out;
		}

		MatrixBase& operator+=(const MatrixBase& other) {
			if (n_rows != other.n_rows || n_cols != other.n_cols) throw std::out_of_range("Dimensions not compatible");
			for (size_t j = 0; j < n_cols; ++j) {
				for (size_t i = 0; i < n_rows; ++i) {
					(*this)[i,j] += other[i, j];
				}
			}
			return *this;
		}

		MatrixBase& operator-=(const MatrixBase& other) {
			if (n_rows != other.n_rows || n_cols != other.n_cols) throw std::out_of_range("Dimensions not compatible");
			for (size_t j = 0; j < n_cols; ++j) {
				for (size_t i = 0; i < n_rows; ++i) {
					(*this)[i,j] -= other[i, j];
				}
			}
			return *this;
		}

		MatrixBase& operator*=(const S& a) {
			for (size_t j = 0; j < n_cols; ++j) {
				for (size_t i = 0; i < n_rows; ++i) {
					(*this)[i,j] *= a;
				}
			}
			return *this;
		}
		
		MatrixBase& operator/=(const S& a) {
			for (size_t j = 0; j < n_cols; ++j) {
				for (size_t i = 0; i < n_rows; ++i) {
					(*this)[i,j] /= a;
				}
			}
			return *this;
		}
	};

	template <typename S>
	class TransposeView : public MatrixBase<S> {
	private:
		MatrixBase<S>& parent;
	public:
		using MatrixBase<S>::n_rows, 
			  MatrixBase<S>::n_cols, 
			  MatrixBase<S>::size;
		TransposeView(MatrixBase<S>& a) : parent(a) {
			n_rows = a.n_cols;
			n_cols = a.n_rows;
			size = a.size;
		}

		S& operator[](size_t i, size_t j) override {
			return parent[j,i];
		}
		const S& operator[](size_t i, size_t j) const override {
			return parent[j,i];
		}

		/*
		* These assignments are made like this to support operations like  = B.col(1) etc.
		* They affect the parent matrix directly.
		*/
		TransposeView& operator =(const MatrixBase<S>& other) {
			if (n_rows != other.n_rows || n_cols != other.n_cols) throw std::out_of_range("Dimensions not compatible");
			for (size_t j = 0; j < n_cols; ++j) 
				for (size_t i = 0; i < n_rows; ++i)
					(*this)[i,j] = other[i,j];
			return *this;
		}; //copy: a = b
		TransposeView& operator =(const TransposeView<S>& other) {
			if (n_rows != other.n_rows || n_cols != other.n_cols) throw std::out_of_range("Dimensions not compatible");
			for (size_t j = 0; j < n_cols; ++j)
				for (size_t i = 0; i < n_rows; ++i) 
					(*this)[i] = other[i];
			return *this;
		};
		TransposeView& operator =(MatrixBase<S>&& other) {
			if (n_rows != other.n_rows || n_cols != other.n_cols) throw std::out_of_range("Dimensions not compatible");
			for (size_t i = 0; i < n_rows; ++i) (*this)[i] = other[i];
			return *this;
		} //move: a = b + c
	};

	template <typename S>
	class Matrix : public MatrixBase<S> {
	private:
		std::vector<S> data;
	public:
		using MatrixBase<S>::n_rows, 
			  MatrixBase<S>::n_cols, 
			  MatrixBase<S>::size;
		Matrix(size_t n_rows, size_t n_cols) :
			data(n_rows * n_cols) {
			this->n_rows = n_rows;
			this->n_cols = n_cols;
			size = n_cols * n_rows;
		}

		Matrix() = default;

		Matrix(const MatrixBase<S>& other) : Matrix(other.n_rows, other.n_cols) {
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
		S& operator[](size_t i, size_t j) override {
			return data[n_rows * j + i];
		}

		const S& operator[](size_t i, size_t j) const override {
			return data[n_rows * j + i];
		}

		static Matrix<S> ident(size_t n) {
			Matrix<S> out = Matrix<S>(n, n);
			for (size_t j = 0; j < out.n_cols; ++j) {
				for (size_t i = 0; i < out.n_rows; ++i) {
					if (i == j) out[i,j] = S(1.);
					else out[i,j] = S(0.);
				}
			}
			return out;
		}
	};

	template <typename S>
	Matrix<S> operator+(const MatrixBase<S>& a, const MatrixBase<S>& b) {
		Matrix<S> out(a);
		out += b;
		return out;
	}

	template <typename S>
	Matrix<S> operator-(const MatrixBase<S>& a, const MatrixBase<S>& b) {
		Matrix<S> out(a);
		out -= b;
		return out;
	}

	template <typename S>
	Matrix<S> operator*(const S& c, const MatrixBase<S>& a) {
		Matrix<S> out(a);
		out *= c;
		return out;
	}

	template <typename S>
	Matrix<S> operator*(const MatrixBase<S>& a, const S& c) {
		Matrix<S> out(a);
		out *= c;
		return out;
	}

	template <typename S>
	Matrix<S> operator/(const MatrixBase<S>& a, const S& c) {
		Matrix<S> out(a);
		out /= c;
		return out;
	}

	template <typename S>
	Matrix<S> mat_mult(const MatrixBase<S>& a, const MatrixBase<S>& b) {
		if (a.n_cols != b.n_rows) throw std::out_of_range("Dimensions not compatible");
		size_t n_rows = a.n_rows;
		size_t n_cols = b.n_cols;
		size_t kmax = a.n_cols;

		Matrix<S> out(n_rows, n_cols);

		for (size_t i = 0; i < n_rows; ++i) {
			for (size_t j = 0; j < n_cols; ++j) {
				for (size_t k = 0; k < kmax; ++k) {
					out[i,j] += a[i,k] * b[k,j];
				}
			}
		}
		return out;
	}

	template <typename S>
	inline Matrix<S> operator*(const MatrixBase<S>& a, const MatrixBase<S>& b) {
		return mat_mult(a,b);
	}

	template <typename S>
	std::ostream& operator<<(std::ostream& s, const MatrixBase<S>& m) {
		for (size_t i = 0; i < m.n_rows; ++i) {
			for (size_t j = 0; j < m.n_cols; ++j) {
				s << m[i,j];
				if (j < m.n_cols - 1) s << " ";
			}
			if (i < m.n_rows - 1) s << "\n";
		}
		return s;
	}

	template <typename S>
	class VectorBase{
	public:
		virtual S& operator[](size_t) = 0;
		virtual const S& operator[](size_t) const = 0;
		size_t size;

		VectorBase& operator+=(const VectorBase& other) {
			if (size != other.size) throw std::out_of_range("Dimensions not compatible");
			for (size_t i = 0; i < size; ++i) {
				(*this)[i] += other[i];
			}
			return *this;
		}

		VectorBase& operator-=(const VectorBase& other) {
			if (size != other.size) throw std::out_of_range("Dimensions not compatible");
			for (size_t i = 0; i < size; ++i) {
				(*this)[i] -= other[i];
			}
			return *this;
		}

		VectorBase& operator*=(const S& a) {
			for (size_t i = 0; i < size; ++i) {
				(*this)[i] *= a;
			}
			return *this;
		}
		
		VectorBase& operator/=(const S& a) {
			for (size_t i = 0; i < size; ++i) {
				(*this)[i] /= a;
			}
			return *this;
		}
	};

	template <typename S>
	class Vector : public VectorBase<S> {
	private:
		std::vector<S> data;
	public:
		using VectorBase<S>::size;

		Vector(size_t size) :
			data(size) {
			this->size = size;
		}

		Vector(const VectorBase<S>& other) : Vector(other.size) {
			for(size_t i = 0; i < size; ++i) (*this)[i] = other[i];
		}

		S& operator[](size_t i) override {
			return data[i];
		}

		const S& operator[](size_t i) const override {
			return data[i];
		}
	};

	template <typename S>
	class ColumnView : public VectorBase<S> {
	private:
		MatrixBase<S>& parent;
		size_t col;
	public:
		using VectorBase<S>::size;
		ColumnView(MatrixBase<S>& parent, size_t col) : 
			parent(parent),
			col(col) 
		{
			size = parent.n_rows;
		}
		/*
		* These assignments are made like this to support operations like A.col(0) = B.col(1) etc.
		* They affect the parent matrix directly.
		*/
		ColumnView& operator =(const VectorBase<S>& other) {
			if (size != other.size) throw std::out_of_range("Dimensions not compatible");
			for (size_t i = 0; i < size; ++i) (*this)[i] = other[i];
			return *this;
		}; //copy: a = b
		ColumnView& operator =(const ColumnView<S>& other) {
			if (size != other.size) throw std::out_of_range("Dimensions not compatible");
			for (size_t i = 0; i < size; ++i) (*this)[i] = other[i];
			return *this;
		};
		ColumnView& operator =(VectorBase<S>&& other) {
			if (size != other.size) throw std::out_of_range("Dimensions not compatible");
			for (size_t i = 0; i < size; ++i) (*this)[i] = other[i];
			return *this;
		} //move: a = b + c


		S& operator[](size_t i) override {
			return parent[i,col];
		}
		const S& operator[](size_t i) const override {
			return parent[i,col];
		}
	};

	template <typename S>
	Vector<S> operator+(const VectorBase<S>& a, const VectorBase<S>& b) {
		Vector<S> out(a);
		out += b;
		return out;
	}

	template <typename S>
	Vector<S> operator-(const VectorBase<S>& a, const VectorBase<S>& b) {
		Vector<S> out(a);
		out -= b;
		return out;
	}

	template <typename S>
	Vector<S> operator*(const S& c, const VectorBase<S>& a) {
		Vector<S> out(a);
		out *= c;
		return out;
	}

	template <typename S>
	Vector<S> operator*(const VectorBase<S>& a, const S& c) {
		Vector<S> out(a);
		out *= c;
		return out;
	}

	template <typename S>
	Vector<S> operator/(const VectorBase<S>& a, const S& c) {
		Vector<S> out(a);
		out /= c;
		return out;
	}

	template <typename S>
	S dot(const VectorBase<S>& a, const VectorBase<S>& b) {
		if (a.size != b.size) throw std::out_of_range("Dimensions not compatible");
		S out(0.);
		for (size_t i = 0; i < a.size; ++i) {
			out += a[i] * b[i];
		}
		return out;
	}

	template <typename S>
	Vector<S> mat_mult(const MatrixBase<S>& a, const VectorBase<S>& b) {
		if (a.n_cols != b.size) throw std::out_of_range("Dimensions not compatible");
		Vector<S> out(a.n_rows);
		for (size_t i = 0; i < a.n_rows; ++i) {
			for (size_t j = 0; j < a.n_cols; ++j) {
				out[i] += a[i,j] * b[j];
			}
		}
		return out;
	}

	template <typename S>
	inline Vector<S> operator*(const MatrixBase<S>& a, const VectorBase<S>& b) {
		return mat_mult(a,b);
	}

	template <typename S>
	std::ostream& operator<<(std::ostream& s, const VectorBase<S>& v) {
		for (size_t i = 0; i < v.size; ++i) {
			s << v[i];
			if (i < v.size - 1) s << "\n";
		}
		return s;
	}

	template <typename S>
	S norm(const VectorBase<S>& a) {
		return std::sqrt(dot(a,a));
	}

	template <typename S>
	using VectorC = Vector<std::complex<S>>;

	template <typename S>
	using MatrixC = Matrix<std::complex<S>>;

	class RandomMatrixGenerator {
	public:
		std::uniform_real_distribution<double> random;
		std::default_random_engine re;

		RandomMatrixGenerator(double lower, double upper) : random(lower, upper) {}

		void fill(MatrixBase<double>& mat) {
			for (size_t j = 0; j < mat.n_cols; ++j) {
				for (size_t i = 0; i < mat.n_rows; ++i) mat[i,j] = random(re);
			}
		}

		void fill_symmetric(MatrixBase<double>& mat) {
			if (!mat.is_square()) throw std::invalid_argument("Non-square matrix cannot be made symmetric");
			for (size_t j = 0; j < mat.n_rows; ++j) {
				for (size_t i = 0; i < j; ++i) {
					double a = random(re);
					mat[i,j] = a;
					mat[j,i] = a;
				}
				mat[j,j] = random(re);
			}
		}
		
		void fill(VectorBase<double>& v) {
			for (size_t i = 0; i < v.size; ++i) {
				v[i] = random(re);
			}
		}

		Vector<double> create_vec(size_t len) {
			Vector<double> out(len);
			fill(out);
			return out;
		}

		Matrix<double> create(size_t rows, size_t cols) {
			Matrix<double> out(rows, cols);
			fill(out);
			return out;
		}

		Matrix<double> create_symmetric(size_t n) {
			Matrix<double> out(n,n);
			fill_symmetric(out);
			return out;
		}
	};

	template <typename S>
	bool approx(const S& a, const S& b, double acc, double eps) {
		S diff = std::abs(a - b);
		return diff <= acc || diff <= eps * std::max(std::abs(a), std::abs(b));
	}

	template <typename S>
	bool mat_approx(const MatrixBase<S>& a, const MatrixBase<S>& b, double acc, double eps) {
		if (a.n_rows != b.n_rows || a.n_cols != b.n_cols) throw std::out_of_range("Dimensions not compatible");
		bool result = true;
		for (size_t j = 0; (j < a.n_cols) && result; ++j) {
			for (size_t i = 0; (i < a.n_rows) && result; ++i) 
				result &= approx(a[i,j], b[i,j], acc, eps);
		}
		return result;
	}

	template <typename S>
	bool mat_approx(const VectorBase<S>& a, const VectorBase<S>& b, double acc, double eps) {
		if (a.size != b.size) throw std::out_of_range("Dimensions not compatible");
		bool result = true;
		for (size_t i = 0; i < a.size; ++i) {
			result &= approx(a[i], b[i], acc, eps);
		}
		return result;
	}
}
