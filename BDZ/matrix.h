#pragma once
#include <stdexcept>
#include <iostream>
#include <cmath>

class MatrixIsDegenerateError : public std::runtime_error {
 public:
  MatrixIsDegenerateError() : std::runtime_error("MatrixIsDegenerateError") {
  }
};

class MatrixOutOfRange : public std::out_of_range {
 public:
  MatrixOutOfRange() : std::out_of_range("MatrixOutOfRange") {
  }
};

template <class T, size_t N, size_t M>
class Matrix {
  public:
  T matrix[N][M];
  // Функции для получения количества строк и столбцов.
  size_t RowsNumber() const {
    return N;
  }
  size_t ColumnsNumber() const {
    return M;
  }
  // Две перегрузки оператора (). Первый - для записи, второй - для чтения.
  T& operator()(size_t n, size_t m) {
    return matrix[n][m];
  }
  const T& operator()(size_t n, size_t m) const {
    return matrix[n][m];
  }
  // Аналог доступа к элементам и их запись, но обрабатывающий исключение.
  T& At(size_t n, size_t m) {
    if (n >= N || m >= M) {
      throw MatrixOutOfRange{};
    }
    return matrix[n][m];
  }
  const T& At(size_t n, size_t m) const {
    if (n >= N || m >= M) {
      throw MatrixOutOfRange{};
    }
    return matrix[n][m];
  }
  // Перегрузки сложения, вычитания, умножения матриц и их присваивающие версии.
  Matrix operator+(const Matrix& other) const {
    Matrix<T, N, M> new_mat;
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        new_mat(i, j) = other(i, j) + matrix[i][j];
      }
    }
    return new_mat;
  }
  Matrix operator-(const Matrix& other) const {
    Matrix<T, N, M> new_mat;
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        new_mat(i, j) = matrix[i][j] - other(i, j);
      }
    }
    return new_mat;
  }
  template <size_t MOther>
  Matrix<T, N, MOther> operator*(const Matrix<T, M, MOther>& other) const {
    Matrix<T, N, MOther> new_mat;
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < MOther; ++j) {
        new_mat(i, j) = 0;
        for (size_t k = 0; k < M; ++k) {
          new_mat(i, j) += matrix[i][k] * other(k, j);
        }
      }
    }
    return new_mat;
  }
  Matrix& operator+=(const Matrix& other) {
    *this = *this + other;
    return *this;
  }
  Matrix& operator-=(const Matrix& other) {
    *this = *this - other;
    return *this;
  }
  Matrix<T, N, M>& operator*=(const Matrix<T, M, M>& other) {
    *this = *this * other;
    return *this;
  }
  // Перегрузки умножения/деления матрицы и числа слева/справа.
  Matrix operator*(const T num) const {
    Matrix<T, N, M> new_mat;
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        new_mat(i, j) = matrix[i][j] * num;
      }
    }
    return new_mat;
  }
  friend Matrix operator*(const T num, const Matrix& mat) {
    return mat * num;
  }
  Matrix operator/(const T num) const {
    Matrix<T, N, M> new_mat;
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        new_mat(i, j) = matrix[i][j] / num;
      }
    }
    return new_mat;
  }
  Matrix& operator*=(const T num) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        matrix[i][j] *= num;
      }
    }
    return *this;
  }
  Matrix& operator/=(const T num) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        matrix[i][j] /= num;
      }
    }
    return *this;
  }
  // Перегрузка сравнение на равенство/неравенство матриц одного размера.
  bool operator==(const Matrix& other) const {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        if (matrix[i][j] != other(i, j)) {
          return false;
        }
      }
    }
    return true;
  }
  bool operator!=(const Matrix& other) const {
    return !(other == *this);
  }
  // Ввод и вывод.
  friend std::istream& operator>>(std::istream& is, Matrix& mat) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        is >> mat(i, j);
      }
  }
  return is;
  }
  friend std::ostream& operator<<(std::ostream& os, const Matrix& mat) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
          os << mat(i, j);
          if (j != M - 1) {
            os << " ";
          }
      }
      os << "\n";
    }
    return os;
  }
};

// Функция для получения транспонизованной матрицы.
template <class T, size_t N, size_t M>
Matrix<T, M, N> GetTransposed(Matrix<T, N, M> mat) {
  Matrix<T, M, N> mat_copy;
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = 0; j < N; ++j) {
      mat_copy(i, j) = mat(j, i);
    }
  }
  return mat_copy;
}
#define MATRIX_SQUARE_MATRIX_IMPLEMENTED 
// Функция для транспонизованния квадратной матрицы.
template <class T, size_t N>
void Transpose(Matrix<T, N, N>& mat) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = i + 1; j < N; ++j) {
      T tmp = mat(i, j);
      mat(i, j) = mat(j, i);
      mat(j, i) = tmp;
    }
  }
}
// Функция для нахождения следа матрицы.
template <class T, size_t N>
T Trace(Matrix<T, N, N>& mat) {
  T trace = 0;
  for (size_t i = 0; i < N; ++i) {
    trace += mat(i, i);
  }
  return trace;
}
// Функция для нахождения определителя матрицы.
template <class T, size_t N>
T Determinant(Matrix<T, N, N>& mat) {
  T det = 0;
  if (N == 2) {
    det = mat(0, 0) * mat(1, 1) - mat(0, 1) * mat(1, 0);
  } else if (N == 3) {
    det = mat(0, 0) * mat(1, 1) * mat(2, 2) + mat(0, 1) * mat(1, 2) * mat(2, 0) + mat(1, 0) * mat(2, 1) * mat(0, 2) - (mat(0, 2) * mat(1, 1) * mat(2, 0) + mat(0, 1) * mat(1, 0) * mat(2, 2) + mat(1, 2) * mat(2, 1) * mat(0, 0));
  } else {
    det = mat(0, 0);
  }
  return det;
}
// Функция для получения обратной матрицы.
template <class T, size_t N>
Matrix<T, N, N> GetInversed(Matrix<T, N, N> mat) {
  T det = Determinant(mat);
  if (det == 0) {
    throw MatrixIsDegenerateError{};
  }
  Matrix<T, N, N> new_mat;
  if constexpr (N == 1) {
    new_mat(0, 0) = T(1);
  } else {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < N; ++j) {
      Matrix<T, N - 1, N - 1> mat2;
      size_t mi = 0;
      for (size_t n = 0; n < N; ++n) {
        if (n == i) {
          continue;
        }
        size_t mj = 0;
        for (size_t m = 0; m < N; ++m) {
          if (m == j) {
            continue;
          }
          mat2(mi, mj++) = mat(n, m);
        }
        mi++;
      }
      new_mat(i, j) = std::pow(-1, i + j) * Determinant(mat2);
      }
    }
  }
  return (T(1) / det) * GetTransposed(new_mat);
}
// Функция для получения обратной матрицы, изменяя данную.
template <class T, size_t N>
void Inverse(Matrix<T, N, N>& mat) {
  mat = GetInversed(mat);
}