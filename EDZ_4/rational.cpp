#include <stdexcept>
#include <iostream>
#include <cstdint>
#include <numeric>
#include "rational.h"

void Rational::Reduction() {
  if (denominator_ == 0) {
    throw RationalDivisionByZero{};
  }
  if (denominator_ < 0) {
    numerator_ = -numerator_;
    denominator_ = -denominator_;
  }
  int32_t gcd = std::gcd(numerator_, denominator_);
  while (gcd > 1) {
    numerator_ /= gcd;
    denominator_ /= gcd;
    gcd = std::gcd(numerator_, denominator_);
  }
}
Rational::Rational(int32_t n, int32_t d) : numerator_(n), denominator_(d) {
  Reduction();
}
void Rational::SetNumerator(int n) {
  numerator_ = n;
  Reduction();
}
void Rational::SetDenominator(int d) {
  denominator_ = d;
  Reduction();
}
Rational Rational::operator+() const {
  return {numerator_, denominator_};
}
Rational Rational::operator-() const {
  return {-numerator_, denominator_};
}
Rational& Rational::operator++() {
  numerator_ += denominator_;
  return *this;
}
Rational& Rational::operator--() {
  numerator_ -= denominator_;
  return *this;
}
Rational Rational::operator++(int) {
  Rational copy = *this;
  numerator_ += denominator_;
  return copy;
}
Rational Rational::operator--(int) {
  Rational copy = *this;
  numerator_ -= denominator_;
  return copy;
}
Rational Rational::operator+(const Rational& other) const {
  int32_t new_n = numerator_ * other.denominator_ + denominator_ * other.numerator_;
  int32_t new_d = denominator_ * other.denominator_;
  return {new_n, new_d};
}
Rational Rational::operator-(const Rational& other) const {
  int32_t new_n = numerator_ * other.denominator_ - denominator_ * other.numerator_;
  int32_t new_d = denominator_ * other.denominator_;
  return {new_n, new_d};
}
Rational Rational::operator*(const Rational& other) const {
  return {numerator_ * other.numerator_, denominator_ * other.denominator_};
}
Rational Rational::operator/(const Rational& other) const {
  return {numerator_ * other.denominator_, denominator_ * other.numerator_};
}
Rational& Rational::operator+=(const Rational& other) {
  numerator_ = numerator_ * other.denominator_ + denominator_ * other.numerator_;
  denominator_ = denominator_ * other.denominator_;
  Reduction();
  return *this;
}
Rational& Rational::operator-=(const Rational& other) {
  numerator_ = numerator_ * other.denominator_ - denominator_ * other.numerator_;
  denominator_ = denominator_ * other.denominator_;
  Reduction();
  return *this;
}
Rational& Rational::operator*=(const Rational& other) {
  numerator_ *= other.numerator_;
  denominator_ *= other.denominator_;
  Reduction();
  return *this;
}
Rational& Rational::operator/=(const Rational& other) {
  numerator_ *= other.denominator_;
  denominator_ *= other.numerator_;
  Reduction();
  return *this;
}
bool Rational::operator==(const Rational& other) const {
  return numerator_ == other.numerator_ && denominator_ == other.denominator_;
}
bool Rational::operator<(const Rational& other) const {
  int32_t n1 = numerator_ * other.denominator_;
  int32_t n2 = other.numerator_ * denominator_;
  return n1 < n2;
}
bool Rational::operator>(const Rational& other) const {
  return other < *this;
}
bool Rational::operator>=(const Rational& other) const {
  return !(*this < other);
}
bool Rational::operator<=(const Rational& other) const {
  return !(other < *this);
}
bool Rational::operator!=(const Rational& other) const {
  return !(other == *this);
}
bool operator==(int num, const Rational& other) {
  return Rational(num) == other;
}
bool operator!=(int num, const Rational& other) {
  return Rational(num) != other;
}
bool operator<(int num, const Rational& other) {
  return Rational(num) < other;
}
bool operator<=(int num, const Rational& other) {
  return Rational(num) <= other;
}
bool operator>(int num, const Rational& other) {
  return Rational(num) > other;
}
bool operator>=(int num, const Rational& other) {
  return Rational(num) >= other;
}
std::istream& operator>>(std::istream& is, Rational& num) {
  int32_t n = 1;
  int32_t d = 1;
  char slesh = 0;
  is >> n;
  if (is.peek() == '/') {
    is >> slesh >> d;
  }
  num = Rational(n, d);
  return is;
}
std::ostream& operator<<(std::ostream& os, const Rational& num) {
  if (num.denominator_ == 1) {
    os << num.numerator_;
  } else {
    os << num.numerator_ << "/" << num.denominator_;
  }
  return os;
}
