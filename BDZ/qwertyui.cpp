#include <stdexcept>
#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <cstddef>
#include "big_integer.h"
// Захотелось пописать комменты, а то код у меня запутанный даже для меня

// Метод считает количество разрядов в основании системы счисления
// Под количеством разрядов я понимаю (количество цифр - 1). То есть у числа 1000: 3 разряда
size_t BigInteger::CountDigitsInBase() const {
  size_t digits = 0;
  DigitType type = kBase; // Скопировали базовую систему счисления в программе

  while (type > 1) {
    type /= 10;
    ++digits;
  }
  return digits;
}

// Метод конвертирует массив в строковое представление числа
// ВАЖНО, что здесь запись всегда без знака минус, даже если он предполагается
std::string BigInteger::ArrToString() const {
  std::string str;
  size_t digits = CountDigitsInBase(); // Считаем количество разрядов
  for (size_t i = 0; i < arr_.size(); ++i) {
    if (i != arr_.size() - 1) {
      std::string substr = std::to_string(arr_[i]); // Превращаем число в строку
      if (substr.length() == digits) { // Если длина строки совпадает с числом разрядов
        str.insert(0, substr); // Вставляем прям так
      } else { // Иначе приписываем слева нули
        str.insert(0, std::string(digits - substr.length(), '0') + substr);
      }
    } else {
      str.insert(0, std::to_string(arr_[i]));
    }
  }
  return str;
}

// Метод убирает все нулевые разряды в конце массива, нормализуя его
// Также, если в массиве остается единственный 0, значит число неотрицательное
void BigInteger::Normalize() {
  while (arr_.size() > 1 && arr_.back() == 0) {
    arr_.pop_back();
  }

  if (arr_.size() == 1 && arr_[0] == 0) {
    is_negative_ = false;
  }
}

// Проверка на переполнение (не больше 10^30000)
void BigInteger::CheckOverflow() {
  size_t digits = CountDigitsInBase(); // Количество разрядов в основании СС
  DigitType bigd = arr_.back(); // Последний элемент массива, дающий понимание, сколько всего цифр в числе

  size_t biggest_digits = bigd == 0 ? 1 : 0; // Если последний элемент == 0, значит он единственный и цифр в числе 1
  if (biggest_digits == 0) {
    DigitType elem = bigd; // Скопировали последний элемент
    while (elem > 0) {
      elem /= 10;
      ++biggest_digits; // Считаем количество разрядов у последнего элемента
    }
  }

  size_t total_digits = digits * (arr_.size() - 1) + biggest_digits;
  if (total_digits > kMaxDecimalDigits) { // Если переполнение, то исключение
    throw BigIntegerOverflow{};
  }
}
// Метод проверки на отрицательное число
bool BigInteger::IsNegative() const {
  return is_negative_;
}

// Конструктор на основе си-строки
BigInteger::BigInteger(const char* str) : is_negative_(str != nullptr && str[0] == '-') {
  if (str == nullptr) {
    arr_.push_back(0);
    return;
  }

  size_t len = strlen(str); // Количество символов в строке
  size_t pos = 0; // Позиция, с которой начнем обрабатывать строку
  if (is_negative_ || str[0] == '+') {
    pos = 1; // Пропускаем минус или плюс
  }

  if (len <= pos) {
    arr_.push_back(0);
    is_negative_ = false;
    return;
  }
  size_t digits = CountDigitsInBase(); // Количество разрядов в основании СС

  // Все элементы, кроме первого, должны быть фикс длины digits
  // Поэтому обрабатываем первый элемент отдельно
  size_t first_chunk_len = (len - pos) % digits;
  if (first_chunk_len == 0) { // Если остаток == 0, значит все элементы массива одной длины digits
    first_chunk_len = digits;
  }

  std::string chunk(str + pos, first_chunk_len); // Создаем строку с первым элементом
  arr_.insert(arr_.begin(), std::stoi(chunk)); // Вставляем в начало массива преобразованную в число строку

  for (size_t i = first_chunk_len; i < len - pos; i += digits) {
    std::string chunk(str + i + pos, digits); // Строка длины digits
    arr_.insert(arr_.begin(), std::stoi(chunk)); // Вставка в начало массива
  }
  Normalize();
  CheckOverflow(); // Проверка на переполнение
}
// Конструктор копирования
BigInteger::BigInteger(const BigInteger& other) = default;
// Конструктор перемещения
BigInteger::BigInteger(BigInteger&& other) noexcept : arr_(std::move(other.arr_)), is_negative_(other.is_negative_) {}
// Копирующее присваивание
BigInteger& BigInteger::operator=(const BigInteger& other) {
  if (this != &other) {
    is_negative_ = other.is_negative_;
    arr_ = other.arr_;
  }
  return *this;
}
// Перемещающее присваивание
BigInteger& BigInteger::operator=(BigInteger&& other) noexcept {
  if (this != &other) {
    is_negative_ = other.is_negative_;
    arr_ = std::move(other.arr_);
  }
  return *this;
}

// Перегрузка була
BigInteger::operator bool() const {
  for (size_t i = 0; i < arr_.size(); ++i) {
    if (arr_[i] != 0) {
      return true;
    }
  }
  return false;
}
// Все перегрузки сравнения
bool BigInteger::operator==(const BigInteger& other) const {
  if (is_negative_ != other.is_negative_ || arr_.size() != other.arr_.size()) {
    return false;
  }
  for (size_t i = 0; i < arr_.size(); ++i) {
    if (arr_[i] != other.arr_[i]) {
      return false;
    }
  }
  return true;
}
bool BigInteger::operator!=(const BigInteger& other) const {
  return !(other == *this);
}
bool BigInteger::operator<(const BigInteger& other) const {
  if (is_negative_ != other.is_negative_) {
    return is_negative_;
  }
  if (arr_.size() > other.arr_.size()) {
    return is_negative_;
  }
  if (arr_.size() < other.arr_.size()) {
    return !is_negative_;
  }
  for (size_t i = arr_.size(); i > 0; --i) {
    if (arr_[i - 1] != other.arr_[i - 1]) {
      if (!is_negative_) {
        return arr_[i - 1] < other.arr_[i - 1];
      }
      return arr_[i - 1] > other.arr_[i - 1];
    }
  }
  return false;
}
bool BigInteger::operator>(const BigInteger& other) const {
  return other < *this;
}
bool BigInteger::operator<=(const BigInteger& other) const {
  return !(other < *this);
}
bool BigInteger::operator>=(const BigInteger& other) const {
  return !(*this < other);
}

bool BigInteger::AbsLess(const BigInteger& other) const {
  if (arr_.size() != other.arr_.size()) {
    return arr_.size() < other.arr_.size();
  }
  for (size_t i = arr_.size(); i > 0; --i) {
    if (arr_[i - 1] != other.arr_[i - 1]) {
      return arr_[i - 1] < other.arr_[i - 1];
    }
  }
  return false;
}

// Унарный плюс
BigInteger BigInteger::operator+() const {
  return *this;
}
// Унарный минус
BigInteger BigInteger::operator-() const {
  BigInteger copy = *this;
  copy.is_negative_ = !copy.is_negative_;
  return copy;
}

// Методы, которые независимо от класса просто реализуют сложение, вычитание, умножение две векторов
std::vector<BigInteger::DigitType> VectorsPlus(const std::vector<BigInteger::DigitType>& arr1, const std::vector<BigInteger::DigitType>& arr2) {
  std::vector<BigInteger::DigitType> result;
  auto minmax_pair = std::minmax(arr2.size(), arr1.size());
  size_t min_len = minmax_pair.first;
  size_t max_len = minmax_pair.second;
  BigInteger::DigitType difference = 0;

  for (size_t i = 0; i < min_len; ++i) {
    int summ = arr2[i] + arr1[i] + difference;
    result.push_back(summ % BigInteger::kBase);
    difference = summ / BigInteger::kBase;
  }
  for (size_t i = min_len; i < max_len; ++i) {
    int summa = (max_len == arr1.size()) ? arr1[i] + difference : arr2[i] + difference;
    result.push_back(summa % BigInteger::kBase);
    difference = summa / BigInteger::kBase;
  }
  if (difference > 0) {
    result.push_back(difference);
  }
  return result;
}
std::vector<BigInteger::DigitType> VectorsMinus(const std::vector<BigInteger::DigitType>& arr1, const std::vector<BigInteger::DigitType>& arr2) {
  std::vector<BigInteger::DigitType> result;

  auto minmax_pair = std::minmax(arr2.size(), arr1.size());
  size_t min_len = minmax_pair.first;
  size_t max_len = minmax_pair.second;

  BigInteger::DigitType difference = 0;

  for (size_t i = 0; i < min_len; ++i) {
    int current = static_cast<int>(arr1[i]) - static_cast<int>(arr2[i]) - difference;
    bool need_borrow = (current < 0);

    result.push_back(need_borrow ? static_cast<BigInteger::DigitType>(current + BigInteger::kBase) : static_cast<BigInteger::DigitType>(current));
    difference = need_borrow ? 1 : 0;
  }
  for (size_t i = min_len; i < max_len; ++i) {
    int current = (max_len == arr1.size()) ? static_cast<int>(arr1[i]) - difference : -(static_cast<int>(arr2[i]) - difference);
    bool need_borrow = (current < 0);

    result.push_back(need_borrow ? static_cast<BigInteger::DigitType>(current + BigInteger::kBase) : static_cast<BigInteger::DigitType>(current));
    difference = need_borrow ? 1 : 0;
  }
  return result;
}
std::vector<BigInteger::DigitType> VectorsMulti(const std::vector<BigInteger::DigitType>& arr1, const std::vector<BigInteger::DigitType>& arr2) {
  std::vector<BigInteger::DigitType> result(arr1.size() + arr2.size(), 0);
  
  for (size_t i = 0; i < arr1.size(); ++i) {
    BigInteger::DoubleDigitType difference = 0;

    for (size_t j = 0; j < arr2.size(); ++j) {
      BigInteger::DoubleDigitType prod = static_cast<BigInteger::DoubleDigitType>(arr1[i]) * arr2[j] + result[i + j] + difference;
      result[i + j] = static_cast<BigInteger::DigitType>(prod % BigInteger::kBase);
      difference = prod / BigInteger::kBase;
    }
    result[i + arr2.size()] = static_cast<BigInteger::DigitType>(difference);
  }
  while (result.size() > 1 && result.back() == 0) {
    result.pop_back();
  }
  return result;
}

// Сложение двух бигинтов
BigInteger BigInteger::operator+(const BigInteger& other) const {
  BigInteger result;
  if (is_negative_ == other.is_negative_) {
    result.arr_ = VectorsPlus(arr_, other.arr_);
    result.is_negative_ = is_negative_;
  } else {
    if (AbsLess(other)) {
      result.arr_ = VectorsMinus(other.arr_, arr_);
      result.is_negative_ = other.is_negative_;
    } else {
      result.arr_ = VectorsMinus(arr_, other.arr_);
      result.is_negative_ = is_negative_;
    }
  }
  result.Normalize();
  result.CheckOverflow();
  return result;
}
// Вычитание двух бигинтов
BigInteger BigInteger::operator-(const BigInteger& other) const {
  return *this + (-other);
}
// Умножение двух бигинтов
BigInteger BigInteger::operator*(const BigInteger& other) const {
  BigInteger result;

  result.arr_ = VectorsMulti(arr_, other.arr_);
  result.is_negative_ = is_negative_ != other.is_negative_;
  
  result.Normalize();
  result.CheckOverflow();
  return result;
}
// Сложение двух бигинтов с присваиванием
BigInteger& BigInteger::operator+=(const BigInteger& other) {
  *this = *this + other;
  return *this;
}
// Вычитание двух бигинтов с присваиванием
BigInteger& BigInteger::operator-=(const BigInteger& other) {
  *this = *this - other;
  return *this;
}
// Вычитание двух бигинтов с присваиванием
BigInteger& BigInteger::operator*=(const BigInteger& other) {
  *this = *this * other;
  return *this;
}
// Сложение бигинта и числа
BigInteger BigInteger::operator+(int value) const {
  return *this + BigInteger(value);
}
// Вычитание бигинта и числа
BigInteger BigInteger::operator-(int value) const {
  return *this - BigInteger(value);
}
// Умножение бигинта и числа
BigInteger BigInteger::operator*(int value) const {
  return *this * BigInteger(value);
}
// Сложение бигинта и числа с присваиванием
BigInteger& BigInteger::operator+=(int value) {
  *this = *this + value;
  return *this;
}
// Вычитание бигинта и числа с присваиванием
BigInteger& BigInteger::operator-=(int value) {
  *this = *this - value;
  return *this;
}
// Вычитание бигинта и числа с присваиванием
BigInteger& BigInteger::operator*=(int value) {
  *this = *this * value;
  return *this;
}
// Префиксный инкремент
BigInteger& BigInteger::operator++() {
  *this = *this + 1;
  return *this;
}
// Префиксный декремент
BigInteger& BigInteger::operator--() {
  *this = *this - 1;
  return *this;
}
// Постфиксный инкремент
BigInteger BigInteger::operator++(int) {
  BigInteger copy = *this;
  ++(*this);
  return copy;
}
// Постфиксный декремент
BigInteger BigInteger::operator--(int) {
  BigInteger copy = *this;
  --(*this);
  return copy;
}
// Побитовый сдвиг
std::istream& operator>>(std::istream& is, BigInteger& num) {
  std::string input;
  
  is >> input;
  num = BigInteger(input.c_str());
  return is;
}
std::ostream& operator<<(std::ostream& os, const BigInteger& num) {
  std::string output = num.ArrToString();

  if (num.is_negative_) {
    os << '-' << output;
  } else {
    os << output;
  }
  return os;
}






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
  size_t static CountDigitsInBase();

  BigInteger() : arr_({0}), is_negative_(false) {}
  ~BigInteger() = default;

  explicit BigInteger(const char* str);
  
  template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
  BigInteger(T n) { // NOLINT
    if (std::is_signed<T>::value && n == std::numeric_limits<T>::min()) {
      // Handle minimum value of signed types specially to avoid overflow
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