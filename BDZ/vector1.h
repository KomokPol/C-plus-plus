// Вектор базовая задача

#pragma once
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <initializer_list>
#include <memory>

template <class T>
class Vector {
  T* buffer_;
  size_t size_;
  size_t capacity_;
  public:
  using ValueType = T;
  using Pointer = T*;
  using ConstPointer = const T*;
  using Reference = T&;
  using ConstReference = const T&;
  using SizeType = size_t;
  using Iterator = T*;
  using ConstIterator = const T*;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

  Iterator begin() { // NOLINT
    return buffer_;
  }
  Iterator end() { // NOLINT
    return buffer_ + size_;
  }
  ConstIterator begin() const { // NOLINT
    return buffer_;
  }
  ConstIterator end() const { // NOLINT
    return buffer_ + size_;
  }
  ConstIterator cbegin() const noexcept { // NOLINT
    return buffer_;
  }
  ConstIterator cend() const noexcept { // NOLINT
    return buffer_ + size_;
  }
  ReverseIterator rbegin() { // NOLINT
    return ReverseIterator(end());
  }
  ReverseIterator rend() { // NOLINT
    return ReverseIterator(begin());
  }
  ConstReverseIterator rbegin() const { // NOLINT
    return ConstReverseIterator(end());
  }
  ConstReverseIterator rend() const { // NOLINT
    return ConstReverseIterator(begin());
  }
  ConstReverseIterator crbegin() const noexcept { // NOLINT
    return ConstReverseIterator(end());
  }
  ConstReverseIterator crend() const noexcept { // NOLINT
    return ConstReverseIterator(begin());
  }

  Vector() : buffer_(nullptr), size_(0), capacity_(0) {}
  explicit Vector(size_t size) : size_(size), capacity_(size) {
    std::unique_ptr<T[]> tmp(size_ > 0 ? new T[size_]() : nullptr);
    buffer_ = tmp.release();
  }
  Vector(size_t size, T value) : size_(size), capacity_(size) {
    std::unique_ptr<T[]> tmp(size_ > 0 ? new T[size_]() : nullptr);
    std::fill(tmp.get(), tmp.get() + size_, value);
    buffer_ = tmp.release();
  }
  template <class Iterator, class = std::enable_if_t<std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>>>
  Vector(Iterator first, Iterator last) {
    int count = 0;
    Iterator first_copy = first;
    while (first_copy != last) {
      ++count;
      ++first_copy;
    }
    std::unique_ptr<T[]> tmp(count > 0 ? new T[count]() : nullptr);
    std::copy(first, last, tmp.get());
    buffer_ = tmp.release();
    size_ = count;
    capacity_ = count;
  }
  Vector(std::initializer_list<T> arr) : size_(arr.size()), capacity_(arr.size()) {
    std::unique_ptr<T[]> tmp(new T[size_]());
    std::copy(arr.begin(), arr.end(), tmp.get());
    buffer_ = tmp.release();
  }
  Vector(const Vector<T>& other) : size_(other.size_), capacity_(other.capacity_) {
    std::unique_ptr<T[]> tmp(other.capacity_ > 0 ? new T[other.capacity_]() : nullptr);
    for (size_t i = 0; i < size_; ++i) {
      tmp[i] = other.buffer_[i];
    }
    buffer_ = tmp.release();
  }
  Vector(Vector<T>&& other) noexcept : buffer_(other.buffer_), size_(other.size_), capacity_(other.capacity_) {
    other.size_ = 0;
    other.capacity_ = 0;
    other.buffer_ = nullptr;
  }
  Vector<T>& operator=(const Vector<T>& other) {
    if (this != &other) {
      std::unique_ptr<T[]> tmp(other.capacity_ > 0 ? new T[other.capacity_]() : nullptr);
      for (size_t i = 0; i < other.size_; ++i) {
        tmp[i] = other.buffer_[i];
      }
      delete[] buffer_;
      buffer_ = tmp.release();
      size_ = other.size_;
      capacity_ = other.capacity_;
    }
    return *this;
  }  
  Vector<T>& operator=(Vector<T>&& other) noexcept {
    if (this != &other) {
      delete[] buffer_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      buffer_ = other.buffer_;
      other.size_ = 0;
      other.capacity_ = 0;
      other.buffer_ = nullptr;
    }
    return *this;
  }
  ~Vector() {
    delete[] buffer_;
  }
  SizeType Size() const {
    return size_;
  }
  SizeType Capacity() const {
    return capacity_;
  }
  bool Empty() const {
    return size_ == 0;
  }
  Reference operator[](size_t index) {
    return buffer_[index];
  }
  ConstReference operator[](size_t index) const {
    return buffer_[index];
  }
  Reference At(size_t index) {
    if (index >= size_) {
      throw std::out_of_range("Выход за пределы вектора");
    }
    return buffer_[index];
  }
  ConstReference At(size_t index) const {
    if (index >= size_) {
      throw std::out_of_range("Выход за пределы вектора");
    }
    return buffer_[index];
  }
  Reference Front() {
    return buffer_[0];
  }
  ConstReference Front() const {
    return buffer_[0];
  }
  Reference Back() {
    return buffer_[size_ - 1];
  }
  ConstReference Back() const {
    return buffer_[size_ - 1];
  }
  Pointer Data() {
    return buffer_;
  }
  ConstPointer Data() const {
    return buffer_;
  }
  void Swap(Vector& other) {
    T* copy = other.buffer_;
    other.buffer_ = buffer_;
    buffer_ = copy;
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }
  void Resize(size_t new_size) {
    if (new_size > capacity_) {
      std::unique_ptr<T[]> tmp(new T[new_size]());
      for (size_t i = 0; i < size_; ++i) {
        tmp[i] = std::move(buffer_[i]);
      }
      delete[] buffer_;
      buffer_ = tmp.release();
      capacity_ = new_size;
    }
    else if (new_size > size_) {
      for (size_t i = size_; i < new_size; ++i) {
        buffer_[i] = T();
      }
    }
    size_ = new_size;
  }
  void Resize(size_t new_size, T value) {
    if (new_size > capacity_) {
      std::unique_ptr<T[]> tmp(new T[new_size]());
      for (size_t i = 0; i < size_; ++i) {
        tmp[i] = std::move(buffer_[i]);
      }
      for (size_t i = size_; i < new_size; ++i) {
        tmp[i] = value;
      }
      delete[] buffer_;
      buffer_ = tmp.release();
      capacity_ = new_size;
    }
    else if (new_size > size_) {
      for (size_t i = size_; i < new_size; ++i) {
        buffer_[i] = value;
      }
    }
    size_ = new_size;
  }
  void Reserve(size_t new_cap) {
    if (new_cap > capacity_) {
      std::unique_ptr<T[]> tmp(new T[new_cap]());
      for (size_t i = 0; i < size_; ++i) {
        tmp[i] = std::move(buffer_[i]);
      }
      delete[] buffer_;
      buffer_ = tmp.release();
      capacity_ = new_cap;
    }
  }
  void ShrinkToFit() {
    if (size_ == 0) {
      delete[] buffer_;
      buffer_ = nullptr;
      capacity_ = 0;
    } else if (size_ < capacity_) {
      std::unique_ptr<T[]> tmp(new T[size_]());
      for (size_t i = 0; i < size_; ++i) {
        tmp[i] = std::move(buffer_[i]);
      }
      delete[] buffer_;
      buffer_ = tmp.release();
      capacity_ = size_;
    }
  }
  void Clear() {
    size_ = 0;
  }
  void PushBack(const T& other) {
    if (size_ == capacity_) {
      size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
      std::unique_ptr<T[]> new_buffer(new T[new_cap]);
      for (size_t i = 0; i < size_; ++i) {
        new_buffer[i] = std::move(buffer_[i]);
      }
      new_buffer[size_] = other;
      delete[] buffer_;
      buffer_ = new_buffer.release();
      capacity_ = new_cap;
      ++size_;
    } else {
      buffer_[size_] = other;
      ++size_;
    }
  }
  void PushBack(T&& other) {
    if (size_ == capacity_) {
      size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
      std::unique_ptr<T[]> new_buffer(new T[new_cap]);
      for (size_t i = 0; i < size_; ++i) {
        new_buffer[i] = std::move(buffer_[i]);
      }
      new_buffer[size_] = std::move(other);
      delete[] buffer_;
      buffer_ = new_buffer.release();
      capacity_ = new_cap;
      ++size_;
    } else {
      buffer_[size_] = std::move(other);
      ++size_;
    }
  }
  void PopBack() {
    buffer_[--size_] = T();
  }
  bool operator==(const Vector& other) const {
    if (size_ != other.size_) {
      return false;
    }
    for (size_t i = 0; i < size_; ++i) {
      if (buffer_[i] != other.buffer_[i]) {
        return false;
      }
    }
    return true;
  }
  bool operator!=(const Vector& other) const {
    return !(other == *this);
  }
  bool operator<(const Vector& other) const {
    size_t min_size = (size_ < other.size_) ? size_ : other.size_;
    for (size_t i = 0; i < min_size; ++i) {
      if (buffer_[i] < other.buffer_[i]) {
        return true;
      }
      if (buffer_[i] > other.buffer_[i]) {
        return false;
      }
    }
    return size_ < other.size_;
  }
  bool operator>(const Vector& other) const {
    return other < *this;
  }
  bool operator<=(const Vector& other) const {
    return !(*this > other);
  }
  bool operator>=(const Vector& other) const {
    return !(*this < other);
  }
};