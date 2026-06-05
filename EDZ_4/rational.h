#pragma once
#include <stdexcept>
#include <iostream>
#include <cstdint>
#include <numeric>

class RationalDivisionByZero : public std::runtime_error {
  public:
  RationalDivisionByZero() : std::runtime_error("RationalDivisionByZero") {}
};
class Rational {
  int32_t numerator_;
  int32_t denominator_;
  void Reduction();
  public:
  Rational() : numerator_(0), denominator_(1) {}
  Rational(int32_t num) : numerator_(num), denominator_(1) {} // NOLINT
  Rational(int32_t n, int32_t d);
  int32_t GetNumerator() const {
    return numerator_;
  }
  int32_t GetDenominator() const {
    return denominator_;
  }
  void SetNumerator(int n);
  void SetDenominator(int d);
  Rational operator+() const;
  Rational operator-() const;
  Rational& operator++();
  Rational& operator--();
  Rational operator++(int);
  Rational operator--(int);
  Rational operator+(const Rational& other) const;
  Rational operator-(const Rational& other) const;
  Rational operator*(const Rational& other) const;
  Rational operator/(const Rational& other) const;
  Rational& operator+=(const Rational& other);
  Rational& operator-=(const Rational& other);
  Rational& operator*=(const Rational& other);
  Rational& operator/=(const Rational& other);
  bool operator==(const Rational& other) const;
  bool operator!=(const Rational& other) const;
  bool operator<(const Rational& other) const;
  bool operator<=(const Rational& other) const;
  bool operator>(const Rational& other) const;
  bool operator>=(const Rational& other) const;
  friend bool operator==(int num, const Rational& other);
  friend bool operator<(int num, const Rational& other);
  friend bool operator>(int num, const Rational& other);
  friend bool operator>=(int num, const Rational& other);
  friend bool operator<=(int num, const Rational& other);
  friend bool operator!=(int num, const Rational& other);
  friend std::istream& operator>>(std::istream& is, Rational& num);
  friend std::ostream& operator<<(std::ostream& os, const Rational& num);
};
