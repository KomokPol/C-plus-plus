// Вектор гпт

#pragma once
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <initializer_list>
#include <memory>
#define VECTOR_MEMORY_IMPLEMENTED

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
  // explicit Vector(size_t size) : size_(size), capacity_(size) {
  //   buffer_ = size_ > 0 ? static_cast<T*>(operator new(sizeof(T) * size_)) : nullptr;
  //   for (size_t i = 0; i < size_; ++i) {
  //     new (buffer_ + i) T();
  //   }
  // }
  explicit Vector(size_t size) : size_(size), capacity_(size) {
    if (size_ > 0) {
      buffer_ = static_cast<T*>(operator new(sizeof(T) * size_));
      size_t constructed = 0;
      try {
        for (size_t i = 0; i < size_; ++i) {
          new (buffer_ + i) T();
          ++constructed;
        }
      } catch (...) {
        for (size_t i = 0; i < constructed; ++i) {
          buffer_[i].~T();
        }
        operator delete(buffer_);
        buffer_ = nullptr;
        size_ = capacity_ = 0;
        throw;
      }
    } else {
      buffer_ = nullptr;
    }
  }
  // Vector(size_t size, T value) : size_(size), capacity_(size) {
  //   buffer_ = size_ > 0 ? static_cast<T*>(operator new(sizeof(T) * size_)) : nullptr;
  //   for (size_t i = 0; i < size_; ++i) {
  //     new (buffer_ + i) T(value);
  //   }
  // }
  Vector(size_t size, T value) : size_(size), capacity_(size) {
    if (size_ > 0) {
      buffer_ = static_cast<T*>(operator new(sizeof(T) * size_));
      size_t constructed = 0;
      try {
        for (size_t i = 0; i < size_; ++i) {
          new (buffer_ + i) T(value);
          ++constructed;
        }
      } catch (...) {
        for (size_t i = 0; i < constructed; ++i) {
          buffer_[i].~T();
        }
        operator delete(buffer_);
        buffer_ = nullptr;
        size_ = capacity_ = 0;
        throw;
      }
    } else {
      buffer_ = nullptr;
    }
  }
  template <class Iterator, class = std::enable_if_t<std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>>>
  Vector(Iterator first, Iterator last) {
  //   int count = 0;
  // Iterator first_copy = first;
  // while (first_copy != last) {
  //   ++count;
  //   ++first_copy;
  // }
  // buffer_ = count > 0 ? static_cast<T*>(operator new(sizeof(T) * count)) : nullptr;
  // std::uninitialized_copy(first, last, buffer_);
  // size_ = count;
  // capacity_ = count;
    size_t count = 0;
    for (Iterator it = first; it != last; ++it) {
      ++count;
    }
    capacity_ = count;
    size_ = count;
    if (count > 0) {
      buffer_ = static_cast<T*>(operator new(sizeof(T) * count));
      size_t constructed = 0;
      try {
        for (Iterator it = first; it != last; ++it, ++constructed) {
          new (buffer_ + constructed) T(*it);
        }
      } catch (...) {
        for (size_t i = 0; i < constructed; ++i) {
          (buffer_ + i)->~T();
        }
        operator delete(buffer_);
        buffer_ = nullptr;
        size_ = capacity_ = 0;
        throw;
      }
    } else {
      buffer_ = nullptr;
    }
  }
  // Vector(std::initializer_list<T> arr) : size_(arr.size()), capacity_(arr.size()) {
  //   buffer_ = static_cast<T*>(operator new(sizeof(T) * size_));
  //   std::uninitialized_copy(arr.begin(), arr.end(), buffer_);
  // }
  Vector(std::initializer_list<T> arr) : size_(arr.size()), capacity_(arr.size()) {
    if (size_ > 0) {
      buffer_ = static_cast<T*>(operator new(sizeof(T) * size_));
      size_t constructed = 0;
      try {
        for (auto it = arr.begin(); it != arr.end(); ++it, ++constructed) {
          new (buffer_ + constructed) T(*it);
        }
      } catch (...) {
        for (size_t i = 0; i < constructed; ++i) {
          (buffer_ + i)->~T();
        }
        operator delete(buffer_);
        buffer_ = nullptr;
        size_ = capacity_ = 0;
        throw;
      }
    } else {
      buffer_ = nullptr;
    }
  }
  // Vector(const Vector<T>& other) : size_(other.size_), capacity_(other.capacity_) {
  //   buffer_ = other.capacity_ > 0 ? static_cast<T*>(operator new(sizeof(T) * other.capacity_)) : nullptr;
  //   std::uninitialized_copy(other.buffer_, other.buffer_ + size_, buffer_);
  // }
  Vector(const Vector<T>& other) : size_(other.size_), capacity_(other.capacity_) {
    if (capacity_ > 0) {
      buffer_ = static_cast<T*>(operator new(sizeof(T) * capacity_));
      size_t constructed = 0;
      try {
        for (size_t i = 0; i < size_; ++i) {
          new (buffer_ + i) T(other.buffer_[i]);
          ++constructed;
        }
      } catch (...) {
        for (size_t i = 0; i < constructed; ++i) {
          buffer_[i].~T();
        }
        operator delete(buffer_);
        buffer_ = nullptr;
        size_ = capacity_ = 0;
        throw;
      }
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
      // std::destroy(buffer_, buffer_ + size_);
      // operator delete(buffer_);
      // buffer_ = other.capacity_ > 0 ? static_cast<T*>(operator new(sizeof(T) * other.capacity_)) : nullptr;
      // std::uninitialized_copy(other.buffer_, other.buffer_ + other.size_, buffer_);
      // size_ = other.size_;
      // capacity_ = other.capacity_;
      Vector<T> temp(other);
      Swap(temp);
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
      size_t constructed = 0;
      try {
        for (size_t i = 0; i < size_; ++i) {
          new (new_buffer + i) T(std::move(buffer_[i]));
          ++constructed;
        }
        for (size_t i = size_; i < new_size; ++i) {
          new (new_buffer + i) T();
          ++constructed;
        }
      } catch (...) {
        for (size_t i = 0; i < constructed; ++i) {
          (new_buffer + i)->~T();
        }
        operator delete(new_buffer);
        throw;
      }
      std::destroy(buffer_, buffer_ + size_);
      operator delete(buffer_);
      buffer_ = new_buffer;
      capacity_ = new_size;
      size_ = new_size;
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
    // Случай, когда требуется перераспределение памяти
    if (new_size > capacity_) {
        T* new_buffer = nullptr;
        size_t constructed = 0;  // количество успешно сконструированных объектов в new_buffer
        try {
            new_buffer = static_cast<T*>(operator new(sizeof(T) * new_size));
            // Перемещаем уже существующие элементы в новый буфер
            for (size_t i = 0; i < size_; ++i) {
                new (new_buffer + i) T(std::move(buffer_[i]));
                ++constructed;
            }
            // Конструируем новые элементы с помощью конструктора с аргументом value
            for (size_t i = size_; i < new_size; ++i) {
                new (new_buffer + i) T(value);
                ++constructed;
            }
        } catch (...) {
            // Если произошло исключение, уничтожаем все сконструированные объекты и освобождаем память
            for (size_t i = 0; i < constructed; ++i) {
                (new_buffer + i)->~T();
            }
            operator delete(new_buffer);
            throw;
        }
        // Если всё успешно, сначала уничтожаем старые объекты
        std::destroy(buffer_, buffer_ + size_);
        operator delete(buffer_);
        buffer_ = new_buffer;
        capacity_ = new_size;
        size_ = new_size;
    }
    else if (new_size > size_) {
        // Расширяем вектор в уже выделенной памяти: добавляем новые элементы
        size_t constructed = 0;
        try {
            for (size_t i = size_; i < new_size; ++i) {
                new (buffer_ + i) T(value);
                ++constructed;
            }
        } catch (...) {
            // Если при конструировании новых элементов выброшено исключение, уничтожаем только сконструированные объекты
            std::destroy(buffer_ + size_, buffer_ + size_ + constructed);
            throw;
        }
        size_ = new_size;
    }
    else {  
        // Если новый размер меньше текущего, уничтожаем лишние элементы
        std::destroy(buffer_ + new_size, buffer_ + size_);
        size_ = new_size;
    }
}
  void Reserve(size_t new_cap) {
    // if (new_cap > capacity_) {
    //   T* new_buffer = static_cast<T*>(operator new(sizeof(T) * new_cap));
    //   std::uninitialized_move(buffer_, buffer_ + size_, new_buffer);
    //   std::destroy(buffer_, buffer_ + size_);
    //   operator delete(buffer_);
    //   buffer_ = new_buffer;
    //   capacity_ = new_cap;
    // }
    if (new_cap > capacity_) {
      T* new_buffer = static_cast<T*>(operator new(sizeof(T) * new_cap));
      size_t constructed = 0;
      try {
        for (size_t i = 0; i < size_; ++i) {
          new (new_buffer + i) T(std::move(buffer_[i]));
          ++constructed;
        }
      } catch (...) {
        for (size_t i = 0; i < constructed; ++i) {
          (new_buffer + i)->~T();
        }
        operator delete(new_buffer);
        throw;
      }
      std::destroy(buffer_, buffer_ + size_);
      operator delete(buffer_);
      buffer_ = new_buffer;
      capacity_ = new_cap;
    }
  }
  void ShrinkToFit() {
    // if (size_ == 0) {
    //   std::destroy(buffer_, buffer_ + size_);
    //   operator delete(buffer_);
    //   buffer_ = nullptr;
    //   capacity_ = 0;
    // } else if (size_ < capacity_) {
    //   T* new_buffer = static_cast<T*>(operator new(sizeof(T) * size_));
    //   std::uninitialized_move(buffer_, buffer_ + size_, new_buffer);
    //   std::destroy(buffer_, buffer_ + size_);
    //   operator delete(buffer_);
    //   buffer_ = new_buffer;
    //   capacity_ = size_;
    // }
    if (size_ == 0) {
      operator delete(buffer_);
      buffer_ = nullptr;
      capacity_ = 0;
    } else if (size_ < capacity_) {
      T* new_buffer = static_cast<T*>(operator new(sizeof(T) * size_));
      size_t constructed = 0;
      try {
        for (size_t i = 0; i < size_; ++i) {
          new (new_buffer + i) T(std::move(buffer_[i]));
          ++constructed;
        }
      } catch (...) {
        for (size_t i = 0; i < constructed; ++i) {
          (new_buffer + i)->~T();
        }
        operator delete(new_buffer);
        throw;
      }
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
        size_t i = 0;
        try {
            // Перемещаем уже существующие элементы
            for (; i < size_; ++i) {
                new (new_buffer + i) T(std::move(buffer_[i]));
            }
            // Конструируем новый элемент
            new (new_buffer + i) T(other);
        }
        catch (...) {
            // Если возникло исключение, разрушаем сконструированные объекты и освобождаем память
            for (size_t j = 0; j < i; ++j) {
                new_buffer[j].~T();
            }
            operator delete(new_buffer);
            throw;
        }
        // Если все успешно, уничтожаем старые элементы и освобождаем память старого буфера
        std::destroy(buffer_, buffer_ + size_);
        operator delete(buffer_);
        // Обновляем внутреннее состояние вектора (это «коммит» изменений)
        buffer_ = new_buffer;
        capacity_ = new_cap;
        ++size_;
    }
    else {
        // Если есть достаточно места, просто конструируем новый элемент в конце
        new (buffer_ + size_) T(other);
        ++size_;
    }
  }
  void PushBack(T&& other) {
    if (size_ == capacity_) {
        size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
        T* new_buffer = static_cast<T*>(operator new(sizeof(T) * new_cap));
        size_t i = 0;
        try {
            // Перемещаем уже существующие объекты в новый буфер.
            for (; i < size_; ++i) {
                new (new_buffer + i) T(std::move(buffer_[i]));
            }
            // Конструируем новый элемент в конце.
            new (new_buffer + i) T(std::move(other));
        }
        catch (...) {
            // Если при строительстве произошла ошибка, уничтожаем успешно построенные объекты.
            for (size_t j = 0; j < i; ++j) {
                new_buffer[j].~T();
            }
            operator delete(new_buffer);
            throw;
        }
        // Если всё прошло успешно, уничтожаем старые объекты и освобождаем старый буфер.
        std::destroy(buffer_, buffer_ + size_);
        operator delete(buffer_);
        // Выполняем «коммит» — обновляем указатель, вместимость и увеличиваем размер.
        buffer_ = new_buffer;
        capacity_ = new_cap;
        ++size_;
    }
    else {
        new (buffer_ + size_) T(std::move(other));
        ++size_;
    }
  }
  template <typename... Args>
  void EmplaceBack(Args&&... args) {
    // if (size_ == capacity_) {
    //   size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
    //   T* new_buffer = static_cast<T*>(operator new(sizeof(T) * new_cap));
    //   std::uninitialized_move(buffer_, buffer_ + size_, new_buffer);
    //   std::destroy(buffer_, buffer_ + size_);
    //   operator delete(buffer_);
    //   buffer_ = new_buffer;
    //   capacity_ = new_cap;
    // }
    // new (buffer_ + size_) T(std::forward<Args>(args)...);
    // ++size_;
    if (size_ == capacity_) {
      size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
      T* new_buffer = static_cast<T*>(operator new(sizeof(T) * new_cap));
      size_t constructed = 0;
      try {
        for (size_t i = 0; i < size_; ++i) {
          new (new_buffer + i) T(std::move(buffer_[i]));
          ++constructed;
        }
      } catch (...) {
        for (size_t i = 0; i < constructed; ++i) {
          (new_buffer + i)->~T();
        }
        operator delete(new_buffer);
        throw;
      }
      std::destroy(buffer_, buffer_ + size_);
      operator delete(buffer_);
      buffer_ = new_buffer;
      capacity_ = new_cap;
    }
    new (buffer_ + size_) T(std::forward<Args>(args)...);
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
  bool operator>=(const Vector& other) const {
    return !(*this < other);
  }
  bool operator<=(const Vector& other) const {
    return !(*this > other);
  }
};