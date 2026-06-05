#ifndef RANGE_H
#define RANGE_H
#define REVERSE_RANGE_IMPLEMENTED
#include <stdexcept>

class Iterator {
  int num_;
  int step_;
  bool reverse_;
  public:
  Iterator(int num, int step, bool reverse) : num_(num), step_(step), reverse_(reverse) {}
  int operator*() const {
    return num_;
  }
  Iterator& operator++() {
    if (!reverse_) {
      num_ += step_;
    } else {
      num_ -= step_;
    }
    return *this;
  }
  bool operator!=(const Iterator& other) const {
    if (!reverse_) {
      return (step_ > 0) ? (num_ < other.num_) : (num_ > other.num_);
    }
    return (step_ > 0) ? (num_ > other.num_) : (num_ < other.num_);
  }
};

class RangeObj {
  int start_;
  int end_;
  int step_;
  public:
  explicit RangeObj(int end) : start_(0), end_(end), step_(1) { }
  RangeObj(int start, int end) : start_(start), end_(end), step_(1) { }
  RangeObj(int start, int end, int step) : start_(start), end_(end), step_(step) { }
  Iterator begin() const { // NOLINT
    if (step_ == 0) {
      return {end_, step_, false};
    }
    return {start_, step_, false};
  }
  Iterator end() const { // NOLINT
    return {end_, step_, false};
  }
  Iterator rbegin() const { // NOLINT
    if (step_ == 0) {
      return rend();
    }
    int count = 0;
    if ((step_ <= 0 || start_ < end_) && (step_ >= 0 || start_ > end_)) {
      count = (std::abs(end_ - start_) + std::abs(step_) - 1) / std::abs(step_);
    }
    if (count <= 0) {
      return rend();
    }
    int last = start_ + (count - 1) * step_;
    return {last, step_, true};
  }
  Iterator rend() const { // NOLINT
    if (step_ == 0) {
      return {start_, step_, true};
    }
    return {start_ - step_, step_, true};
  }
};

inline RangeObj Range(int end) {
  return RangeObj(end);
}
inline RangeObj Range(int start, int end, int step = 1) {
  return {start, end, step};
}

#endif