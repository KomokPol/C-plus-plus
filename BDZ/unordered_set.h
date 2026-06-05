#ifndef UNORDERED_SET_H
#define UNORDERED_SET_H
#include <vector>
#include <list>
#include <algorithm>

template <typename KeyT>
class UnorderedSet {
  size_t n_buckets_;
  size_t n_elements_;
  std::vector<std::list<KeyT>> arr_;
  std::hash<KeyT> hasher_;

 public:
  UnorderedSet() : n_buckets_(0), n_elements_(0), arr_(0), hasher_() {
  }
  explicit UnorderedSet(size_t count) : n_buckets_(count), n_elements_(0), arr_(count), hasher_() {
  }
  template <typename Forward>
  UnorderedSet(Forward start, Forward end) : n_buckets_(0), n_elements_(0), hasher_() {
    Forward copy = start;
    while (copy != end) {
      ++n_elements_;
      ++n_buckets_;
      ++copy;
    }
    arr_.resize(n_buckets_);
    while (start != end) {
      size_t hash = hasher_(*start);
      size_t index = hash % n_buckets_;
      arr_[index].push_back(*start);
      ++start;
    }
  }
  UnorderedSet(const UnorderedSet<KeyT>& other)
      : n_buckets_(other.n_buckets_), n_elements_(other.n_elements_), arr_(other.arr_), hasher_(other.hasher_) {
  }
  UnorderedSet(UnorderedSet<KeyT>&& other) noexcept
      : n_buckets_(other.n_buckets_)
      , n_elements_(other.n_elements_)
      , arr_(std::move(other.arr_))
      , hasher_(std::move(other.hasher_)) {
    other.n_buckets_ = 0;
    other.n_elements_ = 0;
  }
  UnorderedSet<KeyT>& operator=(const UnorderedSet<KeyT>& other) {
    if (this != &other) {
      n_buckets_ = other.n_buckets_;
      n_elements_ = other.n_elements_;
      arr_ = other.arr_;
      hasher_ = other.hasher_;
    }
    return *this;
  }
  UnorderedSet<KeyT>& operator=(UnorderedSet<KeyT>&& other) noexcept {
    if (this != &other) {
      n_buckets_ = other.n_buckets_;
      n_elements_ = other.n_elements_;
      arr_ = std::move(other.arr_);
      hasher_ = std::move(other.hasher_);
      other.n_buckets_ = 0;
      other.n_elements_ = 0;
    }
    return *this;
  }
  ~UnorderedSet() = default;
  size_t Size() const {
    return n_elements_;
  }
  bool Empty() const {
    return n_elements_ == 0;
  }
  void Clear() {
    for (size_t i = 0; i < n_buckets_; ++i) {
      arr_[i].clear();
    }
    n_elements_ = 0;
  }
  void Insert(const KeyT& key) {
    if (n_buckets_ == 0) {
      n_buckets_ = 1;
      arr_.resize(n_buckets_);
    }
    size_t hash = hasher_(key);
    size_t index = hash % n_buckets_;
    if (std::find(arr_[index].begin(), arr_[index].end(), key) != arr_[index].end()) {
      return;
    }
    arr_[index].push_back(key);
    ++n_elements_;
    if (n_elements_ > n_buckets_) {
      Rehash(n_buckets_ * 2);
    }
  }
  void Insert(const KeyT&& key) {
    if (n_buckets_ == 0) {
      n_buckets_ = 1;
      arr_.resize(n_buckets_);
    }
    size_t hash = hasher_(key);
    size_t index = hash % n_buckets_;
    if (std::find(arr_[index].begin(), arr_[index].end(), key) != arr_[index].end()) {
      return;
    }
    arr_[index].push_back(std::move(key));
    ++n_elements_;
    if (n_elements_ > n_buckets_) {
      Rehash(n_buckets_ * 2);
    }
  }
  void Erase(const KeyT& key) {
    if (n_buckets_ == 0) {
      return;
    }
    size_t hash = hasher_(key);
    size_t index = hash % n_buckets_;
    auto pointer = std::find(arr_[index].begin(), arr_[index].end(), key);
    if (pointer != arr_[index].end()) {
      arr_[index].erase(pointer);
      --n_elements_;
    }
  }
  bool Find(const KeyT& key) const {
    if (n_buckets_ == 0) {
      return false;
    }
    size_t hash = hasher_(key);
    size_t index = hash % n_buckets_;
    auto pointer = std::find(arr_[index].begin(), arr_[index].end(), key);
    return (pointer != arr_[index].end());
  }
  void Rehash(size_t new_bucket_count) {
    if (new_bucket_count == n_buckets_ || new_bucket_count < n_elements_) {
      return;
    }
    std::vector<std::list<KeyT>> arr__copy(new_bucket_count);
    for (size_t i = 0; i < n_buckets_; ++i) {
      std::list<KeyT>& bucket = arr_[i];
      auto start = bucket.begin();
      auto end = bucket.end();
      while (start != end) {
        size_t hash = hasher_(*start);
        size_t index = hash % new_bucket_count;
        auto current = start;
        ++start;
        arr__copy[index].splice(arr__copy[index].end(), bucket, current);
      }
    }
    arr_ = std::move(arr__copy);
    n_buckets_ = new_bucket_count;
  }
  void Reserve(size_t new_bucket_count) {
    if (new_bucket_count > n_buckets_) {
      Rehash(new_bucket_count);
    }
  }
  size_t BucketCount() const {
    return n_buckets_;
  }
  size_t BucketSize(size_t id) const {
    if (id >= n_buckets_) {
      return 0;
    }
    return arr_[id].size();
  }
  size_t Bucket(const KeyT& key) const {
    if (n_buckets_ == 0) {
      return 0;
    }
    size_t hash = hasher_(key);
    return hash % n_buckets_;
  }
  double LoadFactor() const {
    if (n_buckets_ == 0) {
      return 0;
    }
    return static_cast<double>(n_elements_) / n_buckets_;
  }
};
#endif