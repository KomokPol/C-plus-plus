#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#define BIG_INTEGER_DIVISION_IMPLEMENTED

#include <stdexcept>
#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <limits>

class BigIntegerOverflow : public std::runtime_error {
 public:
  BigIntegerOverflow() : std::runtime_error("BigIntegerOverflow") {
  }
};

class BigIntegerDivisionByZero : public std::runtime_error {
 public:
  BigIntegerDivisionByZero() : std::runtime_error("BigIntegerDivisionByZero") {
  }
};

class BigInteger {
  public:
  using DigitType = uint16_t;
  using DoubleDigitType = uint32_t;
  static constexpr DigitType kBase = 10000;
  static constexpr size_t kMaxDecimalDigits = 30000;

  private:
  std::vector<DigitType> arr_;
  bool is_negative_ = false;
  std::string ArrToString() const;
  void Normalize();
  void CheckOverflow();
  bool AbsLess(const BigInteger& other) const;

  public:
  bool IsNegative() const;
  static size_t CountDigitsInBase();

  BigInteger();
  ~BigInteger() = default;

  explicit BigInteger(const char* str);
  
  template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
  BigInteger(T n) { // NOLINT
    if (std::is_signed<T>::value && n == std::numeric_limits<T>::min()) {
      std::string min_str = std::to_string(n);
      *this = BigInteger(min_str.c_str());
      return;
    }

    is_negative_ = (n < 0);
    auto abs_n = static_cast<typename std::make_unsigned<T>::type>(is_negative_ ? -n : n);

    if (abs_n == 0) {
      arr_.push_back(0);
    } else {
      while (abs_n > 0) {
        arr_.push_back(static_cast<DigitType>(abs_n % kBase));
        abs_n /= kBase;
      }
    }
    Normalize();
  }
  
  BigInteger(const BigInteger& other);
  BigInteger(BigInteger&& other) noexcept;
  BigInteger& operator=(const BigInteger& other);
  BigInteger& operator=(BigInteger&& other) noexcept;
  
  BigInteger operator+() const;
  BigInteger operator-() const;
  
  BigInteger operator+(const BigInteger& other) const;
  BigInteger operator-(const BigInteger& other) const;
  BigInteger operator*(const BigInteger& other) const;
  BigInteger& operator+=(const BigInteger& other);
  BigInteger& operator-=(const BigInteger& other);
  BigInteger& operator*=(const BigInteger& other);

  BigInteger operator+(int value) const;
  BigInteger operator-(int value) const;
  BigInteger operator*(int value) const;
  BigInteger& operator+=(int value);
  BigInteger& operator-=(int value);
  BigInteger& operator*=(int value);

  BigInteger& operator++();
  BigInteger& operator--();
  BigInteger operator++(int);
  BigInteger operator--(int);

  BigInteger operator/(const BigInteger& other) const;
  BigInteger& operator/=(const BigInteger& other);

  BigInteger operator%(const BigInteger& other) const;
  BigInteger& operator%=(const BigInteger& other);

  explicit operator bool() const;
  bool operator==(const BigInteger& other) const;
  bool operator!=(const BigInteger& other) const;
  bool operator<(const BigInteger& other) const;
  bool operator<=(const BigInteger& other) const;
  bool operator>(const BigInteger& other) const;
  bool operator>=(const BigInteger& other) const;
  
  friend std::istream& operator>>(std::istream& is, BigInteger& num);
  friend std::ostream& operator<<(std::ostream& os, const BigInteger& num);
};

#endif