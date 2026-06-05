// Итоговый Вектор+
#ifndef VECTOR_H
#define VECTOR_H
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <initializer_list>
#include <memory>

template <typename T>
class BufferHelp {
  T* ptr_;
  size_t constructed_;

  public:
  explicit BufferHelp(T* ptr) : ptr_(ptr), constructed_(0) {}
  ~BufferHelp() {
    if (ptr_) {
      for (size_t i = 0; i < constructed_; ++i) {
        ptr_[i].~T();
      }
      operator delete(ptr_);
    }
  }
  // Добавить удаленные конструкторы копирования
  void IncreaseCon() {
    ++constructed_;
  }
  size_t GetCon() const {
    return constructed_;
  }
  T* Release() {
    T* temp = ptr_;
    ptr_ = nullptr;
    constructed_ = 0;
    return temp;
  }
};

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
    if (size_ > 0) {
      T* new_buffer = static_cast<T*>(operator new(sizeof(T) * size_));
      BufferHelp<T> helper(new_buffer);

      for (size_t i = 0; i < size_; ++i) {
        new (new_buffer + i) T();
        helper.IncreaseCon();
      }

      buffer_ = helper.Release();
    } else {
      buffer_ = nullptr;
    }
  }
  Vector(size_t size, T value) : size_(size), capacity_(size) {
    if (size_ > 0) {
      T* new_buffer = static_cast<T*>(operator new(sizeof(T) * size_));
      BufferHelp<T> helper(new_buffer);

      for (size_t i = 0; i < size_; ++i) {
        new (new_buffer + i) T(value);
        helper.IncreaseCon();
      }

      buffer_ = helper.Release();
    } else {
      buffer_ = nullptr;
    }
  }
  template <class Iterator, class = std::enable_if_t<std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>>>
  Vector(Iterator first, Iterator last) {
    int count = 0;
    Iterator first_copy = first;

    while (first_copy != last) {
      ++count;
      ++first_copy;
    }
    size_ = count;
    capacity_ = count;

    if (count > 0) {
      T* new_buffer = static_cast<T*>(operator new(sizeof(T) * count));
      BufferHelp<T> helper(new_buffer);
      int index = 0;
      Iterator first_copy = first;

      while (first_copy != last) {
        new (new_buffer + index) T(*first_copy);
        helper.IncreaseCon();
        ++index;
        ++first_copy;
      }

      buffer_ = helper.Release();
    } else {
      buffer_ = nullptr;
    }
  }
  Vector(std::initializer_list<T> arr) : size_(arr.size()), capacity_(arr.size()) {
    if (size_ > 0) {
      T* new_buffer = static_cast<T*>(operator new(sizeof(T) * size_));
      BufferHelp<T> helper(new_buffer);
      int index = 0;
      auto first = arr.begin();
      auto last = arr.end();

      while (first != last) {
        new (new_buffer + index) T(*first);
        helper.IncreaseCon();
        ++first;
        ++index;
      }

      buffer_ = helper.Release();
    } else {
      buffer_ = nullptr;
    }
  }
  Vector(const Vector<T>& other) : size_(other.size_), capacity_(other.capacity_) {
    if (capacity_ > 0) {
      T* new_buffer = static_cast<T*>(operator new(sizeof(T) * capacity_));
      BufferHelp<T> helper(new_buffer);

      for (size_t i = 0; i < size_; ++i) {
        new (new_buffer + i) T(other.buffer_[i]);
        helper.IncreaseCon();
      }

      buffer_ = helper.Release();
    } else {
      buffer_ = nullptr;
    }
  }
  Vector(Vector<T>&& other) noexcept : buffer_(other.buffer_), size_(other.size_), capacity_(other.capacity_) {
    other.size_ = 0;
    other.capacity_ = 0;
    other.buffer_ = nullptr;
  }
  Vector<T>& operator=(const Vector<T>& other) {
    if (this != &other) {
      if (other.capacity_ > 0) {
        T* new_buffer = static_cast<T*>(operator new(sizeof(T) * other.capacity_));
        BufferHelp<T> helper(new_buffer);

        for (size_t i = 0; i < other.size_; ++i) {
          new (new_buffer + i) T(other.buffer_[i]);
          helper.IncreaseCon();
        }

        new_buffer = helper.Release();

        std::destroy(buffer_, buffer_ + size_);
        operator delete(buffer_);

        buffer_ = new_buffer;
        size_ = other.size_;
        capacity_ = other.capacity_;
      } else {
        std::destroy(buffer_, buffer_ + size_);
        operator delete(buffer_);

        buffer_ = nullptr;
        size_ = 0;
        capacity_ = 0;
      }
    }
    return *this;
  }  
  Vector<T>& operator=(Vector<T>&& other) noexcept {
    if (this != &other) {
      std::destroy(buffer_, buffer_ + size_);
      operator delete(buffer_);

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
    std::destroy(buffer_, buffer_ + size_);
    operator delete(buffer_);
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
      T* new_buffer = static_cast<T*>(operator new(sizeof(T) * new_size));
      BufferHelp<T> helper(new_buffer);

      for (size_t i = 0; i < size_; ++i) {
        new (new_buffer + i) T(std::move(buffer_[i]));
        helper.IncreaseCon();
      }

      for (size_t i = size_; i < new_size; ++i) {
        new (new_buffer + i) T();
        helper.IncreaseCon();
      }

      new_buffer = helper.Release();

      std::destroy(buffer_, buffer_ + size_);
      operator delete(buffer_);

      buffer_ = new_buffer;
      size_ = new_size;
      capacity_ = new_size;
    } else if (new_size > size_) {
      size_t constructed = 0;
      try {
        for (size_t i = size_; i < new_size; ++i) {
          new (buffer_ + i) T();
          ++constructed;
        }
      } catch (...) {
        std::destroy(buffer_ + size_, buffer_ + size_ + constructed);
        throw;
      }
      size_ = new_size;
    } else {
      std::destroy(buffer_ + new_size, buffer_ + size_);
      size_ = new_size;
    }
  }
  void Resize(size_t new_size, T value) {
    if (new_size > capacity_) {
      T* new_buffer = static_cast<T*>(operator new(sizeof(T) * new_size));
      BufferHelp<T> helper(new_buffer);

      for (size_t i = 0; i < size_; ++i) {
        new (new_buffer + i) T(std::move(buffer_[i]));
        helper.IncreaseCon();
      }
      
      for (size_t i = size_; i < new_size; ++i) {
        new (new_buffer + i) T(value);
        helper.IncreaseCon();
      }

      new_buffer = helper.Release();

      std::destroy(buffer_, buffer_ + size_);
      operator delete(buffer_);

      buffer_ = new_buffer;
      size_ = new_size;
      capacity_ = new_size;
    } else if (new_size > size_) {
      size_t constructed = 0;
      try {
        for (size_t i = size_; i < new_size; ++i) {
          new (buffer_ + i) T(value);
          ++constructed;
        }
      } catch (...) {
        std::destroy(buffer_ + size_, buffer_ + size_ + constructed);
        throw;
      }
      size_ = new_size;
    } else {
      std::destroy(buffer_ + new_size, buffer_ + size_);
      size_ = new_size;
    }
  }
  void Reserve(size_t new_cap) {
    if (new_cap > capacity_) {
      T* new_buffer = static_cast<T*>(operator new(sizeof(T) * new_cap));
      BufferHelp<T> helper(new_buffer);

      for (size_t i = 0; i < size_; ++i) {
        new (new_buffer + i) T(std::move(buffer_[i]));
        helper.IncreaseCon();
      }

      new_buffer = helper.Release();

      std::destroy(buffer_, buffer_ + size_);
      operator delete(buffer_);

      buffer_ = new_buffer;
      capacity_ = new_cap;
    }
  }
  void ShrinkToFit() {
    if (size_ == 0) {
      std::destroy(buffer_, buffer_ + size_);
      operator delete(buffer_);

      buffer_ = nullptr;
      capacity_ = 0;
    } else if (size_ < capacity_) {
      T* new_buffer = static_cast<T*>(operator new(sizeof(T) * size_));
      BufferHelp<T> helper(new_buffer);

      for (size_t i = 0; i < size_; ++i) {
        new (new_buffer + i) T(std::move(buffer_[i]));
        helper.IncreaseCon();
      }

      new_buffer = helper.Release();

      std::destroy(buffer_, buffer_ + size_);
      operator delete(buffer_);

      buffer_ = new_buffer;
      capacity_ = size_;
    }
  }
  void Clear() {
    std::destroy(buffer_, buffer_ + size_);
    size_ = 0;
  }
  void PushBack(const T& other) {
    if (size_ == capacity_) {
      size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
      T* new_buffer = static_cast<T*>(operator new(sizeof(T) * new_cap));
      BufferHelp<T> helper(new_buffer);

      for (size_t i = 0; i < size_; ++i) {
        new (new_buffer + i) T(std::move(buffer_[i]));
        helper.IncreaseCon();
      }

      new (new_buffer + size_) T(other);
      helper.IncreaseCon();

      std::destroy(buffer_, buffer_ + size_);
      operator delete(buffer_);

      buffer_ = helper.Release();
      capacity_ = new_cap;
      ++size_;
    } else {
      new (buffer_ + size_) T(other);
      ++size_;
    }
  }
  void PushBack(T&& other) {
    if (size_ == capacity_) {
      size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
      T* new_buffer = static_cast<T*>(operator new(sizeof(T) * new_cap));
      BufferHelp<T> helper(new_buffer);

      for (size_t i = 0; i < size_; ++i) {
        new (new_buffer + i) T(std::move(buffer_[i]));
        helper.IncreaseCon();
      }

      new (new_buffer + size_) T(std::move(other));
      helper.IncreaseCon();

      std::destroy(buffer_, buffer_ + size_);
      operator delete(buffer_);

      buffer_ = helper.Release();
      capacity_ = new_cap;
      ++size_;
    } else {
      new (buffer_ + size_) T(std::move(other));
      ++size_;
    }
  }
  template <typename... Args>
  void EmplaceBack(Args&&... args) {
    if (size_ == capacity_) {
      size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
      T* new_buffer = static_cast<T*>(operator new(sizeof(T) * new_cap));
      BufferHelp<T> helper(new_buffer);

      for (size_t i = 0; i < size_; ++i) {
        new (new_buffer + i) T(std::move(buffer_[i]));
        helper.IncreaseCon();
      }

      new (new_buffer + size_) T(std::forward<Args>(args)...);
      helper.IncreaseCon();

      std::destroy(buffer_, buffer_ + size_);
      operator delete(buffer_);

      buffer_ = helper.Release();
      capacity_ = new_cap;
    }
    else {
      new (buffer_ + size_) T(std::forward<Args>(args)...);
    }
    ++size_;
  }
  void PopBack() {
    std::destroy_at(buffer_ + --size_);
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
    size_t mini = (size_ < other.size_) ? size_ : other.size_;
    for (size_t i = 0; i < mini; ++i) {
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
#endif