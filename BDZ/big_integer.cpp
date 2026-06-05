#include <stdexcept>
#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits>
#include "big_integer.h"

size_t BigInteger::CountDigitsInBase() {
    size_t digits = 0;
    DigitType t = kBase;
    while (t > 1) {
        t /= 10;
        ++digits;
    }
    return digits;
}

std::string BigInteger::ArrToString() const {
    std::string s;
    size_t per_chunk = CountDigitsInBase();
    for (size_t i = 0; i < arr_.size(); ++i) {
        if (i + 1 < arr_.size()) {
            std::string chunk = std::to_string(arr_[i]);
            if (chunk.length() < per_chunk) {
                s.insert(0, std::string(per_chunk - chunk.length(), '0') + chunk);
            } else {
                s.insert(0, chunk);
            }
        } else {
            s.insert(0, std::to_string(arr_[i]));
        }
    }
    return s;
}

void BigInteger::Normalize() {
    while (arr_.size() > 1 && arr_.back() == 0) {
        arr_.pop_back();
    }
    if (arr_.size() == 1 && arr_[0] == 0) {
        is_negative_ = false;
    }
}

void BigInteger::CheckOverflow() {
    if (arr_.empty()) {
        arr_.push_back(0);
        is_negative_ = false;
        return;
    }
    size_t biggest_digits = 1;
    if (arr_.back() != 0) {
        DigitType x = arr_.back();
        biggest_digits = 0;
        while (x > 0) {
            x /= 10;
            ++biggest_digits;
        }
    }
    size_t per_chunk = CountDigitsInBase();
    if (arr_.size() > 1) {
        size_t total_digits = per_chunk * (arr_.size() - 1) + biggest_digits;
        if (total_digits > kMaxDecimalDigits) {
            throw BigIntegerOverflow();
        }
    } else {
        if (biggest_digits > kMaxDecimalDigits) {
            throw BigIntegerOverflow();
        }
    }
}

bool BigInteger::IsNegative() const {
    return is_negative_;
}

BigInteger::BigInteger() {
    arr_.push_back(0);
    is_negative_ = false;
}

BigInteger::BigInteger(const char* str) {
    if (str == nullptr || *str == '\0') {
        arr_.push_back(0);
        is_negative_ = false;
        return;
    }
    is_negative_ = (str[0] == '-');
    size_t idx = (str[0] == '-' || str[0] == '+') ? 1 : 0;
    while (str[idx] == '0' && str[idx + 1] != '\0') {
        ++idx;
    }
    size_t len = std::strlen(str);
    if (idx >= len) {
        arr_.push_back(0);
        is_negative_ = false;
        return;
    }
    size_t per_chunk = CountDigitsInBase();
    size_t first_chunk_len = (len - idx) % per_chunk;
    if (first_chunk_len == 0) {
      first_chunk_len = per_chunk;
    }
    try {
        std::string chunk(str + idx, first_chunk_len);
        arr_.push_back(static_cast<DigitType>(std::stoul(chunk)));
        for (size_t i = idx + first_chunk_len; i < len; i += per_chunk) {
            chunk = std::string(str + i, per_chunk);
            arr_.insert(arr_.begin(), static_cast<DigitType>(std::stoul(chunk)));
        }
    } catch(...) {
        arr_.clear();
        arr_.push_back(0);
        is_negative_ = false;
        return;
    }
    Normalize();
    CheckOverflow();
}

BigInteger::BigInteger(const BigInteger& other) = default;

BigInteger::BigInteger(BigInteger&& other) noexcept
    : arr_(std::move(other.arr_)),
      is_negative_(other.is_negative_)
{}

BigInteger& BigInteger::operator=(const BigInteger& other) {
    if (this != &other) {
        arr_ = other.arr_;
        is_negative_ = other.is_negative_;
    }
    return *this;
}

BigInteger& BigInteger::operator=(BigInteger&& other) noexcept {
    if (this != &other) {
        arr_ = std::move(other.arr_);
        is_negative_ = other.is_negative_;
    }
    return *this;
}

BigInteger::operator bool() const {
    for (auto d : arr_) {
        if (d != 0) {
          return true;
        }
    }
    return false;
}

bool BigInteger::operator==(const BigInteger& other) const {
    return is_negative_ == other.is_negative_
        && arr_.size() == other.arr_.size()
        && std::equal(arr_.begin(), arr_.end(), other.arr_.begin());
}

bool BigInteger::operator!=(const BigInteger& other) const {
    return !(*this == other);
}

bool BigInteger::operator<(const BigInteger& other) const {
    if (is_negative_ != other.is_negative_) {
        return is_negative_;
    }
    if (arr_.size() == 1 && arr_[0] == 0
     && other.arr_.size() == 1 && other.arr_[0] == 0) {
        return false;
    }
    if (arr_.size() != other.arr_.size()) {
        return (arr_.size() < other.arr_.size()) ^ is_negative_;
    }
    for (size_t i = arr_.size(); i > 0; --i) {
        if (arr_[i-1] != other.arr_[i-1]) {
            return (arr_[i-1] < other.arr_[i-1]) ^ is_negative_;
        }
    }
    return false;
}

bool BigInteger::operator>(const BigInteger& other)  const { return other < *this; }
bool BigInteger::operator<=(const BigInteger& other) const { return !(*this > other); }
bool BigInteger::operator>=(const BigInteger& other) const { return !(*this < other); }

bool BigInteger::AbsLess(const BigInteger& other) const {
    if (arr_.size() != other.arr_.size()) {
        return arr_.size() < other.arr_.size();
    }
    for (size_t i = arr_.size(); i > 0; --i) {
        if (arr_[i-1] != other.arr_[i-1]) {
            return arr_[i-1] < other.arr_[i-1];
        }
    }
    return false;
}

BigInteger BigInteger::operator+() const {
    return *this;
}

BigInteger BigInteger::operator-() const {
    BigInteger tmp = *this;
    if (arr_.size() != 1 || arr_[0] != 0) {
        tmp.is_negative_ = !tmp.is_negative_;
    }
    return tmp;
}

BigInteger& BigInteger::operator++() {
    *this = *this + 1;
    return *this;
}

BigInteger BigInteger::operator++(int) {
    BigInteger old = *this;
    ++(*this);
    return old;
}

BigInteger& BigInteger::operator--() {
    *this = *this - 1;
    return *this;
}

BigInteger BigInteger::operator--(int) {
    BigInteger old = *this;
    --(*this);
    return old;
}

static std::vector<BigInteger::DigitType> VectorsPlus(
    const std::vector<BigInteger::DigitType>& a,
    const std::vector<BigInteger::DigitType>& b)
{
    std::vector<BigInteger::DigitType> result;
    size_t n1 = a.size();
    size_t n2 = b.size();
    BigInteger::DigitType carry = 0;
    size_t per_chunk = BigInteger::CountDigitsInBase();
    for (size_t i = 0; i < n1 || i < n2 || carry; ++i) {
        uint32_t sum = carry;
        if (i < n1) {
          sum += a[i];
        }
        if (i < n2) {
          sum += b[i];
        }
        if (sum >= BigInteger::kBase) {
            result.push_back(static_cast<BigInteger::DigitType>(sum % BigInteger::kBase));
            carry = static_cast<BigInteger::DigitType>(sum / BigInteger::kBase);
        } else {
            result.push_back(static_cast<BigInteger::DigitType>(sum));
            carry = 0;
        }
        if (result.size() * per_chunk > BigInteger::kMaxDecimalDigits) {
            throw BigIntegerOverflow();
        }
    }
    return result;
}

static std::vector<BigInteger::DigitType> VectorsMinus(
    const std::vector<BigInteger::DigitType>& a,
    const std::vector<BigInteger::DigitType>& b)
{
    std::vector<BigInteger::DigitType> result;
    size_t n1 = a.size();
    size_t n2 = b.size();
    BigInteger::DigitType borrow = 0;
    size_t min_len = std::min(n1, n2);
    for (size_t i = 0; i < min_len; ++i) {
        BigInteger::DigitType t = (a[i] >= borrow ? a[i] - borrow
                                                   : a[i] + BigInteger::kBase - borrow);
        if (t >= b[i]) {
            result.push_back(t - b[i]);
            borrow = 0;
        } else {
            result.push_back(static_cast<BigInteger::DigitType>(t + BigInteger::kBase - b[i]));
            borrow = 1;
        }
    }
    for (size_t i = min_len; i < n1; ++i) {
        if (a[i] >= borrow) {
            result.push_back(a[i] - borrow);
            borrow = 0;
        } else {
            result.push_back(static_cast<BigInteger::DigitType>(a[i] + BigInteger::kBase - borrow));
            borrow = 1;
        }
    }
    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }
    return result;
}

static std::vector<BigInteger::DigitType> VectorsMulti(
    const std::vector<BigInteger::DigitType>& a,
    const std::vector<BigInteger::DigitType>& b)
{
    size_t n1 = a.size();
    size_t n2 = b.size();
    std::vector<BigInteger::DigitType> result(n1 + n2, 0);
    uint64_t base = BigInteger::kBase;
    for (size_t i = 0; i < n1; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < n2; ++j) {
            uint64_t prod = static_cast<uint64_t>(a[i]) * b[j]
                            + result[i + j]
                            + carry;
            result[i + j] = static_cast<BigInteger::DigitType>(prod % base);
            carry = prod / base;
        }
        size_t k = i + n2;
        while (carry > 0 && k < result.size()) {
            uint64_t sum = static_cast<uint64_t>(result[k]) + carry;
            result[k] = static_cast<BigInteger::DigitType>(sum % base);
            carry = sum / base;
            ++k;
        }
        if (carry > 0) {
            throw BigIntegerOverflow();
        }
    }
    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }
    return result;
}

BigInteger BigInteger::operator+(const BigInteger& other) const {
    BigInteger result;
    if (is_negative_ == other.is_negative_) {
        result.arr_ = VectorsPlus(arr_, other.arr_);
        result.is_negative_ = is_negative_;
    } else {
        if (this->AbsLess(other)) {
            result.arr_ = VectorsMinus(other.arr_, this->arr_);
            result.is_negative_ = other.is_negative_;
        } else {
            result.arr_ = VectorsMinus(this->arr_, other.arr_);
            result.is_negative_ = this->is_negative_;
        }
    }
    result.Normalize();
    result.CheckOverflow();
    return result;
}

BigInteger BigInteger::operator-(const BigInteger& other) const {
    if (is_negative_ != other.is_negative_) {
        return *this + (-other);
    }
    if (this->AbsLess(other)) {
        BigInteger result;
        result.arr_ = VectorsMinus(other.arr_, this->arr_);
        result.is_negative_ = !this->is_negative_;
        result.Normalize();
        return result;
    }
    BigInteger result;
    result.arr_ = VectorsMinus(this->arr_, other.arr_);
    result.is_negative_ = this->is_negative_;
    result.Normalize();
    return result;
}

BigInteger BigInteger::operator*(const BigInteger& other) const {
    BigInteger result;
    result.arr_ = VectorsMulti(arr_, other.arr_);
    result.is_negative_ = (is_negative_ != other.is_negative_);
    result.Normalize();
    result.CheckOverflow();
    return result;
}

BigInteger& BigInteger::operator+=(const BigInteger& other) {
    *this = *this + other;
    return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& other) {
    *this = *this - other;
    return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger& other) {
    *this = *this * other;
    return *this;
}

BigInteger BigInteger::operator+(int value) const {
    return *this + BigInteger(value);
}

BigInteger BigInteger::operator-(int value) const {
    return *this - BigInteger(value);
}

BigInteger BigInteger::operator*(int value) const {
    return *this * BigInteger(value);
}

BigInteger& BigInteger::operator+=(int value) {
    *this = *this + value;
    return *this;
}

BigInteger& BigInteger::operator-=(int value) {
    *this = *this - value;
    return *this;
}

BigInteger& BigInteger::operator*=(int value) {
    *this = *this * value;
    return *this;
}

static BigInteger AbsCopy(const BigInteger& x) {
    BigInteger tmp = x;
    if (tmp.IsNegative()) {
        tmp = -tmp;
    }
    return tmp;
}

BigInteger BigInteger::operator/(const BigInteger& other) const {
    if (other == BigInteger(0)) {
        throw BigIntegerDivisionByZero{};
    }
    bool result_negative = (this->is_negative_ != other.is_negative_);
    BigInteger dividend = AbsCopy(*this);
    BigInteger divisor  = AbsCopy(other);
    if (dividend.AbsLess(divisor)) {
        return {0};
    }
    size_t n = dividend.arr_.size();
    BigInteger current(0);
    std::vector<DigitType> quotient_chunks(n, 0);
    for (size_t i = n; i-- > 0; ) {
        if (current.arr_.size() != 1 || current.arr_[0] != 0) {
            current.arr_.insert(current.arr_.begin(), dividend.arr_[i]);
        } else {
            current.arr_[0] = dividend.arr_[i];
        }
        current.Normalize();
        DigitType low = 0;
        auto high = static_cast<DigitType>(kBase);
        while (low < high) {
            auto mid = static_cast<DigitType>((static_cast<uint32_t>(low) + high) / 2);
            BigInteger prod = divisor * mid;
            if (prod <= current) {
                low = mid + 1;
            } else {
                high = mid;
            }
        }
        DigitType x = low - 1;
        quotient_chunks[i] = x;
        BigInteger to_sub = divisor * x;
        current = current - to_sub;
    }
    BigInteger result;
    result.is_negative_ = result_negative;
    result.arr_ = std::move(quotient_chunks);
    result.Normalize();
    return result;
}

BigInteger& BigInteger::operator/=(const BigInteger& other) {
    *this = *this / other;
    return *this;
}

BigInteger BigInteger::operator%(const BigInteger& other) const {
    if (other == BigInteger(0)) {
        throw BigIntegerDivisionByZero{};
    }
    BigInteger dividend = AbsCopy(*this);
    BigInteger divisor  = AbsCopy(other);
    if (dividend.AbsLess(divisor)) {
        BigInteger r = *this;
        if (r.arr_.size() == 1 && r.arr_[0] == 0) {
            r.is_negative_ = false;
        }
        return r;
    }
    size_t n = dividend.arr_.size();
    BigInteger current(0);
    for (size_t i = n; i-- > 0; ) {
        if (current.arr_.size() != 1 || current.arr_[0] != 0) {
            current.arr_.insert(current.arr_.begin(), dividend.arr_[i]);
        } else {
            current.arr_[0] = dividend.arr_[i];
        }
        current.Normalize();
        DigitType low = 0;
        auto high = static_cast<DigitType>(kBase);
        while (low < high) {
            auto mid = static_cast<DigitType>((static_cast<uint32_t>(low) + high) / 2);
            BigInteger prod = divisor * mid;
            if (prod <= current) {
                low = mid + 1;
            } else {
                high = mid;
            }
        }
        DigitType x = low - 1;
        BigInteger to_sub = divisor * x;
        current = current - to_sub;
    }
    if (this->is_negative_) {
        if (current.arr_.size() != 1 || current.arr_[0] != 0) {
            current.is_negative_ = true;
        }
    } else {
        current.is_negative_ = false;
    }
    return current;
}

BigInteger& BigInteger::operator%=(const BigInteger& other) {
    *this = *this % other;
    return *this;
}

std::istream& operator>>(std::istream& is, BigInteger& num) {
    std::string tmp;
    is >> tmp;
    num = BigInteger(tmp.c_str());
    return is;
}

std::ostream& operator<<(std::ostream& os, const BigInteger& num) {
    if (num.is_negative_) {
        os << '-';
    }
    os << num.ArrToString();
    return os;
}
