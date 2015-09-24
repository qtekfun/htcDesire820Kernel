// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_BASE_ENUM_SET_H_
#define SYNC_INTERNAL_API_PUBLIC_BASE_ENUM_SET_H_

#include <bitset>
#include <cstddef>
#include <string>

#include "base/basictypes.h"
#include "base/logging.h"

namespace syncer {

template <typename E, E MinEnumValue, E MaxEnumValue>
class EnumSet;

template <typename E, E Min, E Max>
EnumSet<E, Min, Max> Union(EnumSet<E, Min, Max> set1,
                           EnumSet<E, Min, Max> set2);

template <typename E, E Min, E Max>
EnumSet<E, Min, Max> Intersection(EnumSet<E, Min, Max> set1,
                                  EnumSet<E, Min, Max> set2);

template <typename E, E Min, E Max>
EnumSet<E, Min, Max> Difference(EnumSet<E, Min, Max> set1,
                                EnumSet<E, Min, Max> set2);


template <typename E, E MinEnumValue, E MaxEnumValue>
class EnumSet {
 public:
  typedef E EnumType;
  static const E kMinValue = MinEnumValue;
  static const E kMaxValue = MaxEnumValue;
  static const size_t kValueCount = kMaxValue - kMinValue + 1;
  COMPILE_ASSERT(kMinValue < kMaxValue,
                 min_value_must_be_less_than_max_value);

 private:
  
  typedef std::bitset<kValueCount> EnumBitSet;

 public:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  class Iterator {
   public:
    
    
    
    Iterator() : enums_(NULL), i_(kValueCount) {}
    ~Iterator() {}

    

    
    
    bool Good() const {
      return enums_ && i_ < kValueCount && enums_->test(i_);
    }

    
    
    E Get() const {
      CHECK(Good());
      return FromIndex(i_);
    }

    
    
    void Inc() {
      CHECK(Good());
      i_ = FindNext(i_ + 1);
    }

   private:
    friend Iterator EnumSet::First() const;

    explicit Iterator(const EnumBitSet& enums)
        : enums_(&enums), i_(FindNext(0)) {}

    size_t FindNext(size_t i) {
      while ((i < kValueCount) && !enums_->test(i)) {
        ++i;
      }
      return i;
    }

    const EnumBitSet* enums_;
    size_t i_;
  };

  

  EnumSet() {}

  explicit EnumSet(E value) {
    Put(value);
  }

  EnumSet(E value1, E value2) {
    Put(value1);
    Put(value2);
  }

  EnumSet(E value1, E value2, E value3) {
    Put(value1);
    Put(value2);
    Put(value3);
  }

  
  static EnumSet All() {
    EnumBitSet enums;
    enums.set();
    return EnumSet(enums);
  }

  ~EnumSet() {}

  

  
  
  

  
  void Put(E value) {
    enums_.set(ToIndex(value));
  }

  
  void PutAll(EnumSet other) {
    enums_ |= other.enums_;
  }

  

  
  void RetainAll(EnumSet other) {
    enums_ &= other.enums_;
  }

  
  void Remove(E value) {
    if (InRange(value)) {
      enums_.reset(ToIndex(value));
    }
  }

  
  void RemoveAll(EnumSet other) {
    enums_ &= ~other.enums_;
  }

  
  void Clear() {
    enums_.reset();
  }

  
  
  bool Has(E value) const {
    return InRange(value) && enums_.test(ToIndex(value));
  }

  
  bool HasAll(EnumSet other) const {
    return (enums_ & other.enums_) == other.enums_;
  }

  
  
  bool Equals(const EnumSet& other) const {
    return enums_ == other.enums_;
  }

  
  bool Empty() const {
    return !enums_.any();
  }

  
  size_t Size() const {
    return enums_.count();
  }

  
  Iterator First() const {
    return Iterator(enums_);
  }

 private:
  friend EnumSet Union<E, MinEnumValue, MaxEnumValue>(
      EnumSet set1, EnumSet set2);
  friend EnumSet Intersection<E, MinEnumValue, MaxEnumValue>(
      EnumSet set1, EnumSet set2);
  friend EnumSet Difference<E, MinEnumValue, MaxEnumValue>(
      EnumSet set1, EnumSet set2);

  explicit EnumSet(EnumBitSet enums) : enums_(enums) {}

  static bool InRange(E value) {
    return (value >= MinEnumValue) && (value <= MaxEnumValue);
  }

  

  static size_t ToIndex(E value) {
    DCHECK_GE(value, MinEnumValue);
    DCHECK_LE(value, MaxEnumValue);
    return value - MinEnumValue;
  }

  static E FromIndex(size_t i) {
    DCHECK_LT(i, kValueCount);
    return static_cast<E>(MinEnumValue + i);
  }

  EnumBitSet enums_;
};

template <typename E, E MinEnumValue, E MaxEnumValue>
const E EnumSet<E, MinEnumValue, MaxEnumValue>::kMinValue;

template <typename E, E MinEnumValue, E MaxEnumValue>
const E EnumSet<E, MinEnumValue, MaxEnumValue>::kMaxValue;

template <typename E, E MinEnumValue, E MaxEnumValue>
const size_t EnumSet<E, MinEnumValue, MaxEnumValue>::kValueCount;


template <typename E, E Min, E Max>
EnumSet<E, Min, Max> Union(EnumSet<E, Min, Max> set1,
                           EnumSet<E, Min, Max> set2) {
  return EnumSet<E, Min, Max>(set1.enums_ | set2.enums_);
}

template <typename E, E Min, E Max>
EnumSet<E, Min, Max> Intersection(EnumSet<E, Min, Max> set1,
                                  EnumSet<E, Min, Max> set2) {
  return EnumSet<E, Min, Max>(set1.enums_ & set2.enums_);
}

template <typename E, E Min, E Max>
EnumSet<E, Min, Max> Difference(EnumSet<E, Min, Max> set1,
                                EnumSet<E, Min, Max> set2) {
  return EnumSet<E, Min, Max>(set1.enums_ & ~set2.enums_);
}

}  

#endif  
