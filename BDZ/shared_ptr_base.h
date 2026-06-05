template <typename T>
class SharedPtr {
  T* ptr_;
  size_t* strong_counter_;
  
  public:
  SharedPtr() : ptr_(nullptr), strong_counter_(nullptr) {}
  explicit SharedPtr(T* ptr) : ptr_(ptr), strong_counter_(ptr == nullptr ? nullptr : new size_t(1)) {}
  SharedPtr(const SharedPtr& other) : ptr_(other.ptr_), strong_counter_(other.strong_counter_) {
    if (strong_counter_) {
      ++(*strong_counter_);
    }
  }
  SharedPtr(SharedPtr&& other) noexcept : ptr_(other.ptr_), strong_counter_(other.strong_counter_) {
    other.ptr_ = nullptr;
    other.strong_counter_ = nullptr;
  }
  SharedPtr& operator=(const SharedPtr& other) {
    if (this != &other) {
      this->~SharedPtr();
      ptr_ = other.ptr_;
      strong_counter_ = other.strong_counter_;
      if (strong_counter_) {
        ++(*strong_counter_);
      }
    }
    return *this;
  }
  SharedPtr& operator=(SharedPtr&& other) noexcept {
    if (this != &other) {
      this->~SharedPtr();
      ptr_ = other.ptr_;
      strong_counter_ = other.strong_counter_;
      other.ptr_ = nullptr;
      other.strong_counter_ = nullptr;
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
    if (strong_counter_) {
      --(*strong_counter_);
      if (*strong_counter_ == 0) {
        delete strong_counter_;
        delete ptr_;
      }
    }
  }
  void Reset(T* ptr = nullptr) {
    this->~SharedPtr();
    ptr_ = ptr;
    strong_counter_ = ptr_ ? new size_t(1) : nullptr;
  }
  void Swap(SharedPtr& other) {
    std::swap(ptr_, other.ptr_);
    std::swap(strong_counter_, other.strong_counter_);
  }
  T* Get() const {
    return ptr_;
  }
  size_t UseCount() const {
    return strong_counter_ ? *strong_counter_ : 0;
  }
};