// Copyright (c) 2007, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef TCMALLOC_PACKED_CACHE_INL_H_
#define TCMALLOC_PACKED_CACHE_INL_H_

#include "config.h"
#include <stddef.h>                     
#ifdef HAVE_STDINT_H
#include <stdint.h>                     
#endif
#include "base/basictypes.h"
#include "internal_logging.h"

#define N_ONES_(IntType, N)                                     \
  ( (N) == 0 ? 0 : ((static_cast<IntType>(1) << ((N)-1))-1 +    \
                    (static_cast<IntType>(1) << ((N)-1))) )

template <int kKeybits, typename T>
class PackedCache {
 public:
  typedef uintptr_t K;
  typedef size_t V;
#ifdef TCMALLOC_SMALL_BUT_SLOW
  
  static const int kHashbits = 12;
#else
  
  
  static const int kHashbits = 12;
#endif
  static const int kValuebits = 7;
  static const bool kUseWholeKeys = kKeybits + kValuebits <= 8 * sizeof(T);

  explicit PackedCache(V initial_value) {
    COMPILE_ASSERT(kKeybits <= sizeof(K) * 8, key_size);
    COMPILE_ASSERT(kValuebits <= sizeof(V) * 8, value_size);
    COMPILE_ASSERT(kHashbits <= kKeybits, hash_function);
    COMPILE_ASSERT(kKeybits - kHashbits + kValuebits <= kTbits,
                   entry_size_must_be_big_enough);
    Clear(initial_value);
  }

  void Put(K key, V value) {
    ASSERT(key == (key & kKeyMask));
    ASSERT(value == (value & kValueMask));
    array_[Hash(key)] = KeyToUpper(key) | value;
  }

  bool Has(K key) const {
    ASSERT(key == (key & kKeyMask));
    return KeyMatch(array_[Hash(key)], key);
  }

  V GetOrDefault(K key, V default_value) const {
    
    
    
    
    ASSERT(key == (key & kKeyMask));
    T entry = array_[Hash(key)];
    return KeyMatch(entry, key) ? EntryToValue(entry) : default_value;
  }

  void Clear(V value) {
    ASSERT(value == (value & kValueMask));
    for (int i = 0; i < 1 << kHashbits; i++) {
      ASSERT(kUseWholeKeys || KeyToUpper(i) == 0);
      array_[i] = kUseWholeKeys ? (value | KeyToUpper(i)) : value;
    }
  }

 private:
  
  
  
  
  typedef T UPPER;

  static V EntryToValue(T t) { return t & kValueMask; }

  
  
  
  
  static UPPER KeyToUpper(K k) {
    if (kUseWholeKeys) {
      return static_cast<T>(k) << kValuebits;
    } else {
      const int shift = kHashbits - kValuebits;
      
      return static_cast<T>(k >> shift) & kUpperMask;
    }
  }

  static size_t Hash(K key) {
    return static_cast<size_t>(key) & N_ONES_(size_t, kHashbits);
  }

  
  static bool KeyMatch(T entry, K key) {
    return kUseWholeKeys ?
        (entry >> kValuebits == key) :
        ((KeyToUpper(key) ^ entry) & kUpperMask) == 0;
  }

  static const int kTbits = 8 * sizeof(T);
  static const int kUpperbits = kUseWholeKeys ? kKeybits : kKeybits - kHashbits;

  
  static const K kKeyMask = N_ONES_(K, kKeybits);

  
  static const T kUpperMask = N_ONES_(T, kUpperbits) << kValuebits;

  
  static const V kValueMask = N_ONES_(V, kValuebits);

  
  
  volatile T array_[1 << kHashbits];
};

#undef N_ONES_

#endif  
