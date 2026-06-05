#pragma once
#include <stdexcept>

class ArrayOutOfRange : public std::out_of_range {
 public:
  ArrayOutOfRange() : std::out_of_range("ArrayOutOfRange") {
  }
};

template <class T, size_t N>
class Array {
  public:
  T matrix[N];
  // Две перегрузки оператора []. Первый - для чтения, второй - для записи.
  const T& operator[](size_t i) const {
    return matrix[i];
  }
  T& operator[](size_t i) {
    return matrix[i];
  }
  // Аналог доступа к элементам и их запись, но обрабатывающий исключение.
  T& At(size_t idx) {
    if (idx >= N) {
      throw ArrayOutOfRange{};
    }
    return matrix[idx];
  }
  const T& At(size_t idx) const {
    if (idx >= N) {
      throw ArrayOutOfRange{};
    }
    return matrix[idx];
  }
  // Функции доступа к первому элементу и его запись.
  T& Front() {
    return matrix[0];
  }
  const T& Front() const {
    return matrix[0];
  }
  // Функции доступа к последнему элементу и его запись.
  T& Back() {
    return matrix[N - 1];
  }
  const T& Back() const {
    return matrix[N - 1];
  }
  // Функции, возвращающие указатель на начало массива.
  T* Data() {
    if (N == 0) {
      return nullptr;
    }
    return &matrix[0];
  }
  const T* Data() const {
    if (N == 0) {
      return nullptr;
    }
    return &matrix[0];
  }
  // Функция, возвращающая размер массива.
  size_t Size() const {
    return N;
  }
  // Функция на проверку пустоты массива.
  bool Empty() const {
    return N == 0;
  }
  // Функция, заполняющая массив значениями.
  void Fill(const T& value) {
    for (size_t i = 0; i < N; ++i) {
      matrix[i] = value;
    }
  }
  // Метод, обменивающий содержимое массивов одинакового размера.
  void Swap(Array<T, N>& other) {
    for (size_t i = 0; i < N; ++i) {
      T tmp = matrix[i];
      matrix[i] = other.matrix[i];
      other.matrix[i] = tmp;
    }
  }
};

#define ARRAY_TRAITS_IMPLEMENTED
// Функция, возвращающая размерность массива.
template <class T>
size_t GetSize(const T&) { return 0; }
template <class T, size_t N>
size_t GetSize(const T (&)[N]) { return N; }
template <class T, size_t N, size_t M>
size_t GetSize(const T (&)[N][M]) { return N; }
template <class T, size_t N, size_t M, size_t K>
size_t GetSize(const T (&)[N][M][K]) { return N; }
// Функция, возвращающая число координат массива.
template <class T>
size_t GetRank(const T&) { return 0; }
template <class T, size_t N>
size_t GetRank(const T (&)[N]) { return 1; }
template <class T, size_t N, size_t M>
size_t GetRank(const T (&)[N][M]) { return 2; }
template <class T, size_t N, size_t M, size_t K>
size_t GetRank(const T (&)[N][M][K]) { return 3; }
// Функция, возвращающая количество элементов в массиве.
template <class T>
size_t GetNumElements(const T&) { return 1; }
template <class T, size_t N>
size_t GetNumElements(const T (&)[N]) { return N; }
template <class T, size_t N, size_t M>
size_t GetNumElements(const T (&)[N][M]) { return N * M; }
template <class T, size_t N, size_t M, size_t K>
size_t GetNumElements(const T (&)[N][M][K]) { return N * M * K; }