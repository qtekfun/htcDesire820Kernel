// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_BASE_ORDINAL_H_
#define SYNC_INTERNAL_API_PUBLIC_BASE_ORDINAL_H_

#include <algorithm>
#include <cstddef>
#include <string>

#include "base/basictypes.h"
#include "base/json/string_escape.h"
#include "base/logging.h"

namespace syncer {

template <typename Traits>
class Ordinal {
 public:
  
  class LessThanFn {
   public:
    LessThanFn();

    bool operator()(const Ordinal<Traits>& lhs,
                    const Ordinal<Traits>& rhs) const;
  };

  class EqualsFn {
   public:
    EqualsFn();

    bool operator()(const Ordinal<Traits>& lhs,
                    const Ordinal<Traits>& rhs) const;
  };

  
  
  explicit Ordinal(const std::string& bytes);

  
  Ordinal();

  
  
  
  static Ordinal CreateInitialOrdinal();

  
  
  bool IsValid() const;

  
  
  bool EqualsOrBothInvalid(const Ordinal& other) const;

  
  
  std::string ToDebugString() const;

  
  

  

  
  bool LessThan(const Ordinal& other) const;

  
  bool GreaterThan(const Ordinal& other) const;

  
  
  bool Equals(const Ordinal& other) const;

  
  
  
  Ordinal CreateBetween(const Ordinal& other) const;

  
  Ordinal CreateBefore() const;

  
  Ordinal CreateAfter() const;

  
  
  
  std::string ToInternalValue() const;

  

  
  static const uint8 kZeroDigit = Traits::kZeroDigit;
  static const uint8 kMaxDigit = Traits::kMaxDigit;
  static const size_t kMinLength = Traits::kMinLength;
  static const uint8 kOneDigit = kZeroDigit + 1;
  static const uint8 kMidDigit = kOneDigit + (kMaxDigit - kOneDigit) / 2;
  static const unsigned int kMidDigitValue = kMidDigit - kZeroDigit;
  static const unsigned int kMaxDigitValue = kMaxDigit - kZeroDigit;
  static const unsigned int kRadix = kMaxDigitValue + 1;

  COMPILE_ASSERT(kOneDigit > kZeroDigit, OrdinalOneDigitGreaterThanMinDigit);
  COMPILE_ASSERT(kMidDigit > kOneDigit, OrdinalMidDigitGreaterThanOneDigit);
  COMPILE_ASSERT(kMaxDigit > kMidDigit, OrdinalMaxDigitGreaterThanMidDigit);
  COMPILE_ASSERT(kMinLength > 0, OrdinalMinLengthIsPositive);
  COMPILE_ASSERT(kMidDigitValue > 1, OrdinalMidDigitValueGreaterThanOne);
  COMPILE_ASSERT(kMaxDigitValue > kMidDigitValue,
                 OrdinalMaxDigitValueGreaterThanMidDigitValue);
  COMPILE_ASSERT(kRadix == kMaxDigitValue + 1,
                 OrdinalRadixIsMaxDigitValuePlusOne);

 private:
  
  
  static bool IsValidOrdinalBytes(const std::string& bytes);

  
  
  static size_t GetLengthWithoutTrailingZeroDigits(
      const std::string& bytes,
      size_t length);

  
  
  static uint8 GetDigit(const std::string& bytes, size_t i);

  
  static int GetDigitValue(const std::string& bytes, size_t i);

  
  
  static int AddDigitValue(std::string* bytes, size_t i, int digit_value);

  
  
  
  
  static size_t GetProperLength(const std::string& lower_bound,
                                const std::string& bytes);

  
  
  static std::string ComputeMidpoint(const std::string& start,
                                     const std::string& end);

  
  
  
  static Ordinal<Traits> CreateOrdinalBetween(const Ordinal<Traits>& start,
                                              const Ordinal<Traits>& end);

  
  
  std::string bytes_;

  
  bool is_valid_;
};

template <typename Traits> const uint8 Ordinal<Traits>::kZeroDigit;
template <typename Traits> const uint8 Ordinal<Traits>::kMaxDigit;
template <typename Traits> const size_t Ordinal<Traits>::kMinLength;
template <typename Traits> const uint8 Ordinal<Traits>::kOneDigit;
template <typename Traits> const uint8 Ordinal<Traits>::kMidDigit;
template <typename Traits> const unsigned int Ordinal<Traits>::kMidDigitValue;
template <typename Traits> const unsigned int Ordinal<Traits>::kMaxDigitValue;
template <typename Traits> const unsigned int Ordinal<Traits>::kRadix;

template <typename Traits>
Ordinal<Traits>::LessThanFn::LessThanFn() {}

template <typename Traits>
bool Ordinal<Traits>::LessThanFn::operator()(const Ordinal<Traits>& lhs,
                                             const Ordinal<Traits>& rhs) const {
  return lhs.LessThan(rhs);
}

template <typename Traits>
Ordinal<Traits>::EqualsFn::EqualsFn() {}

template <typename Traits>
bool Ordinal<Traits>::EqualsFn::operator()(const Ordinal<Traits>& lhs,
                                           const Ordinal<Traits>& rhs) const {
  return lhs.Equals(rhs);
}

template <typename Traits>
Ordinal<Traits>::Ordinal(const std::string& bytes)
    : bytes_(bytes),
      is_valid_(IsValidOrdinalBytes(bytes_)) {}

template <typename Traits>
Ordinal<Traits>::Ordinal() : is_valid_(false) {}

template <typename Traits>
Ordinal<Traits> Ordinal<Traits>::CreateInitialOrdinal() {
  std::string bytes(Traits::kMinLength, kZeroDigit);
  bytes[0] = kMidDigit;
  return Ordinal(bytes);
}

template <typename Traits>
bool Ordinal<Traits>::IsValid() const {
  DCHECK_EQ(IsValidOrdinalBytes(bytes_), is_valid_);
  return is_valid_;
}

template <typename Traits>
bool Ordinal<Traits>::EqualsOrBothInvalid(const Ordinal& other) const {
  if (!IsValid() && !other.IsValid())
    return true;

  if (!IsValid() || !other.IsValid())
    return false;

  return Equals(other);
}

template <typename Traits>
std::string Ordinal<Traits>::ToDebugString() const {
  std::string debug_string =
      base::EscapeBytesAsInvalidJSONString(bytes_, false );
  if (!is_valid_) {
    debug_string = "INVALID[" + debug_string + "]";
  }
  return debug_string;
}

template <typename Traits>
bool Ordinal<Traits>::LessThan(const Ordinal& other) const {
  CHECK(IsValid());
  CHECK(other.IsValid());
  return bytes_ < other.bytes_;
}

template <typename Traits>
bool Ordinal<Traits>::GreaterThan(const Ordinal& other) const {
  CHECK(IsValid());
  CHECK(other.IsValid());
  return bytes_ > other.bytes_;
}

template <typename Traits>
bool Ordinal<Traits>::Equals(const Ordinal& other) const {
  CHECK(IsValid());
  CHECK(other.IsValid());
  return bytes_ == other.bytes_;
}

template <typename Traits>
Ordinal<Traits> Ordinal<Traits>::CreateBetween(const Ordinal& other) const {
  CHECK(IsValid());
  CHECK(other.IsValid());
  CHECK(!Equals(other));

  if (LessThan(other)) {
    return CreateOrdinalBetween(*this, other);
  } else {
    return CreateOrdinalBetween(other, *this);
  }
}

template <typename Traits>
Ordinal<Traits> Ordinal<Traits>::CreateBefore() const {
  CHECK(IsValid());
  
  
  const size_t length = bytes_.length();
  std::string start(length, kZeroDigit);
  start[length - 1] = kOneDigit;
  if (start == bytes_) {
    start[length - 1] = kZeroDigit;
    start += kOneDigit;
  }

  
  
  
  return CreateBetween(Ordinal(start));
}

template <typename Traits>
Ordinal<Traits> Ordinal<Traits>::CreateAfter() const {
  CHECK(IsValid());
  
  
  std::string end(bytes_.length(), kMaxDigit);
  if (end == bytes_)
    end += kMaxDigit;

  
  
  
  return CreateBetween(Ordinal(end));
}

template <typename Traits>
std::string Ordinal<Traits>::ToInternalValue() const {
  CHECK(IsValid());
  return bytes_;
}

template <typename Traits>
bool Ordinal<Traits>::IsValidOrdinalBytes(const std::string& bytes) {
  const size_t length = bytes.length();
  if (length < kMinLength)
    return false;

  bool found_non_zero = false;
  for (size_t i = 0; i < length; ++i) {
    const uint8 byte = bytes[i];
    if (byte < kZeroDigit || byte > kMaxDigit)
      return false;
    if (byte > kZeroDigit)
      found_non_zero = true;
  }
  if (!found_non_zero)
    return false;

  if (length > kMinLength) {
    const uint8 last_byte = bytes[length - 1];
    if (last_byte == kZeroDigit)
      return false;
  }

  return true;
}

template <typename Traits>
size_t Ordinal<Traits>::GetLengthWithoutTrailingZeroDigits(
    const std::string& bytes, size_t length) {
  DCHECK(!bytes.empty());
  DCHECK_GT(length, 0U);

  size_t end_position =
      bytes.find_last_not_of(static_cast<char>(kZeroDigit), length - 1);

  
  
  if (end_position == std::string::npos)
    return 0;

  return end_position + 1;
}

template <typename Traits>
uint8 Ordinal<Traits>::GetDigit(const std::string& bytes, size_t i) {
  return (i < bytes.length()) ? bytes[i] : kZeroDigit;
}

template <typename Traits>
int Ordinal<Traits>::GetDigitValue(const std::string& bytes, size_t i) {
  return GetDigit(bytes, i) - kZeroDigit;
}

template <typename Traits>
int Ordinal<Traits>::AddDigitValue(std::string* bytes,
                                   size_t i, int digit_value) {
  DCHECK_GE(i, 0U);
  DCHECK_LT(i, bytes->length());

  for (int j = static_cast<int>(i); j >= 0 && digit_value > 0; --j) {
    int byte_j_value = GetDigitValue(*bytes, j) + digit_value;
    digit_value = byte_j_value / kRadix;
    DCHECK_LE(digit_value, 1);
    byte_j_value %= kRadix;
    (*bytes)[j] = static_cast<char>(kZeroDigit + byte_j_value);
  }
  return digit_value;
}

template <typename Traits>
size_t Ordinal<Traits>::GetProperLength(const std::string& lower_bound,
                                        const std::string& bytes) {
  CHECK_GT(bytes, lower_bound);

  size_t drop_length =
      GetLengthWithoutTrailingZeroDigits(bytes, bytes.length());
  
  
  if (drop_length > kMinLength) {
    size_t truncated_length =
        GetLengthWithoutTrailingZeroDigits(bytes, drop_length - 1);

    if (truncated_length > 0 &&
        bytes.compare(0, truncated_length, lower_bound) > 0)
      drop_length = truncated_length;
  }
  return std::max(drop_length, kMinLength);
}

template <typename Traits>
std::string Ordinal<Traits>::ComputeMidpoint(
    const std::string& start,
    const std::string& end) {
  size_t max_size = std::max(start.length(), end.length()) + 1;
  std::string midpoint(max_size, kZeroDigit);

  
  
  
  
  
  
  int forward_carry = 0;
  for (size_t i = 0; i < max_size; ++i) {
    const int sum_value = GetDigitValue(start, i) + GetDigitValue(end, i);
    const int digit_value = sum_value / 2 + forward_carry;
    
    
    CHECK_EQ(AddDigitValue(&midpoint, i, digit_value), 0);
    forward_carry = (sum_value % 2 == 1) ? kMidDigitValue : 0;
  }
  DCHECK_EQ(forward_carry, 0);

  return midpoint;
}

template <typename Traits>
Ordinal<Traits> Ordinal<Traits>::CreateOrdinalBetween(
    const Ordinal<Traits>& start,
    const Ordinal<Traits>& end) {
  CHECK(start.IsValid());
  CHECK(end.IsValid());
  CHECK(start.LessThan(end));
  const std::string& start_bytes = start.ToInternalValue();
  const std::string& end_bytes = end.ToInternalValue();
  DCHECK_LT(start_bytes, end_bytes);

  std::string midpoint = ComputeMidpoint(start_bytes, end_bytes);
  const size_t proper_length = GetProperLength(start_bytes, midpoint);
  midpoint.resize(proper_length, kZeroDigit);

  DCHECK_GT(midpoint, start_bytes);
  DCHECK_LT(midpoint, end_bytes);

  Ordinal<Traits> midpoint_ordinal(midpoint);
  DCHECK(midpoint_ordinal.IsValid());
  return midpoint_ordinal;
}

}  

#endif  
