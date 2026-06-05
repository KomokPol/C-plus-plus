#ifndef SHARED_PTR_H
#define SHARED_PTR_H
#define WEAK_PTR_IMPLEMENTED

#include <stddef.h>
#include <utility>
#include <stdexcept>
using BadWeakPtr = std::bad_weak_ptr;

template <typename T>
class WeakPtr;
template <typename T>
class SharedPtr;
template <typename T>
struct Counter;

template <typename T>
struct Counter {
  T* ptr;
  size_t strong_count;
  size_t weak_count;
  Counter() = default;
  explicit Counter(T* pointer) : ptr(pointer), strong_count(1), weak_count(0) {}
  ~Counter() {
    delete ptr;
  }
};

template <typename T>
class WeakPtr {
  Counter<T>* counter_;
  friend class SharedPtr<T>;

  public:
  WeakPtr() : counter_(nullptr) {}
  WeakPtr(const SharedPtr<T>& other) : counter_(other.counter_) { // NOLINT
    if (counter_) {
      ++(counter_->weak_count);
    }
  }
  WeakPtr(const WeakPtr<T>& other) : counter_(other.counter_) {
    if (counter_) {
      ++counter_->weak_count;
    }
  }
  WeakPtr(WeakPtr<T>&& other) noexcept : counter_(other.counter_) {
    other.counter_ = nullptr;
  }
  WeakPtr<T>& operator=(const WeakPtr<T>& other) {
    if (this != &other) {
      this->~WeakPtr();
      counter_ = other.counter_;
      if (counter_) {
        ++counter_->weak_count;
      }
    }
    return *this;
  }
  WeakPtr<T>& operator=(WeakPtr<T>&& other) noexcept {
    if (this != &other) {
      this->~WeakPtr();
      counter_ = other.counter_;
      other.counter_ = nullptr;
    }
    return *this;
  }
  WeakPtr<T>& operator=(const SharedPtr<T>& other) {
    this->~WeakPtr();
    counter_ = other.counter_;
    if (counter_) {
      ++counter_->weak_count;
    }
    return *this;
  }
  ~WeakPtr() {
    if (counter_) {
      --(counter_->weak_count);
      if (counter_->weak_count == 0 && counter_->strong_count == 0) {
        delete counter_;
      }
    }
  }
  void Swap(WeakPtr<T>& other) {
    std::swap(counter_, other.counter_);
  }
  void Reset() {
    this->~WeakPtr();
    counter_ = nullptr;
  }
  size_t UseCount() const {
    return counter_ ? counter_->strong_count : 0;
  }
  bool Expired() const {
    return counter_ == nullptr || counter_->strong_count == 0;
  }
  SharedPtr<T> Lock() const {
    return Expired() ? SharedPtr<T>(nullptr) : SharedPtr<T>(*this);
  }
};

template <typename T>
class SharedPtr {
  T* ptr_;
  Counter<T>* counter_;
  friend class WeakPtr<T>;
  
  public:
  SharedPtr() : ptr_(nullptr), counter_(nullptr) {}
  explicit SharedPtr(T* ptr) : ptr_(ptr), counter_(ptr == nullptr ? nullptr : new Counter<T>(ptr)) {}
  explicit SharedPtr(const WeakPtr<T>& obj) {
    if (obj.Expired()) {
      throw BadWeakPtr();
    }
    counter_ = obj.counter_;
    ptr_ = counter_->ptr;
    if (counter_) {
      ++(counter_->strong_count);
    }
  }
  SharedPtr(const SharedPtr<T>& other) : ptr_(other.ptr_), counter_(other.counter_) {
    if (counter_) {
      ++(counter_->strong_count);
    }
  }
  SharedPtr(SharedPtr<T>&& other) noexcept : ptr_(other.ptr_), counter_(other.counter_) {
    other.ptr_ = nullptr;
    other.counter_ = nullptr;
  }
  SharedPtr<T>& operator=(const SharedPtr<T>& other) {
    if (this != &other) {
      this->~SharedPtr();
      ptr_ = other.ptr_;
      counter_ = other.counter_;
      if (counter_) {
        ++(counter_->strong_count);
      }
    }
    return *this;
  }
  SharedPtr<T>& operator=(SharedPtr<T>&& other) noexcept {
    if (this != &other) {
      this->~SharedPtr();
      ptr_ = other.ptr_;
      counter_ = other.counter_;
      other.ptr_ = nullptr;
      other.counter_ = nullptr;
    }
    return *this;
  }
  T& operator*() const {
    return *ptr_;
  }
  T* operator->() const {
    return ptr_;
  }
  explicit operator bool() const {
    return ptr_ != nullptr;
  }
  ~SharedPtr() {
    if (counter_) {
      --(counter_->strong_count);
      if (counter_->strong_count == 0) {
        delete ptr_;
        counter_->ptr = nullptr;
        if (counter_->weak_count == 0) {
          delete counter_;
        }
      }
    }
  }
  void Reset(T* ptr = nullptr) {
    this->~SharedPtr();
    ptr_ = ptr;
    counter_ = ptr_ ? new Counter<T>(ptr_) : nullptr;
  }
  void Swap(SharedPtr<T>& other) {
    std::swap(ptr_, other.ptr_);
    std::swap(counter_, other.counter_);
  }
  T* Get() const {
    return ptr_;
  }
  size_t UseCount() const {
    return counter_ ? counter_->strong_count : 0;
  }
};

template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args) {
  return SharedPtr<T>(new T(std::forward<Args>(args)...));
}
#endif