// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_MEMORY_MANUAL_CONSTRUCTOR_H_
#define BASE_MEMORY_MANUAL_CONSTRUCTOR_H_

#include <stddef.h>

#include "base/memory/aligned_memory.h"

namespace base {

template <typename Type>
class ManualConstructor {
 public:
  
  
  
  

  
  
  static void* operator new[](size_t size) {
#if defined(COMPILER_MSVC)
    return AlignedAlloc(size, __alignof(Type));
#else
    return AlignedAlloc(size, __alignof__(Type));
#endif
  }
  static void operator delete[](void* mem) {
    AlignedFree(mem);
  }

  inline Type* get() {
    return space_.template data_as<Type>();
  }
  inline const Type* get() const  {
    return space_.template data_as<Type>();
  }

  inline Type* operator->() { return get(); }
  inline const Type* operator->() const { return get(); }

  inline Type& operator*() { return *get(); }
  inline const Type& operator*() const { return *get(); }

  
  inline void Init() {
    new(space_.void_data()) Type;
  }

  template <typename T1>
  inline void Init(const T1& p1) {
    new(space_.void_data()) Type(p1);
  }

  template <typename T1, typename T2>
  inline void Init(const T1& p1, const T2& p2) {
    new(space_.void_data()) Type(p1, p2);
  }

  template <typename T1, typename T2, typename T3>
  inline void Init(const T1& p1, const T2& p2, const T3& p3) {
    new(space_.void_data()) Type(p1, p2, p3);
  }

  template <typename T1, typename T2, typename T3, typename T4>
  inline void Init(const T1& p1, const T2& p2, const T3& p3, const T4& p4) {
    new(space_.void_data()) Type(p1, p2, p3, p4);
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5>
  inline void Init(const T1& p1, const T2& p2, const T3& p3, const T4& p4,
                   const T5& p5) {
    new(space_.void_data()) Type(p1, p2, p3, p4, p5);
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5,
            typename T6>
  inline void Init(const T1& p1, const T2& p2, const T3& p3, const T4& p4,
                   const T5& p5, const T6& p6) {
    new(space_.void_data()) Type(p1, p2, p3, p4, p5, p6);
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5,
            typename T6, typename T7>
  inline void Init(const T1& p1, const T2& p2, const T3& p3, const T4& p4,
                   const T5& p5, const T6& p6, const T7& p7) {
    new(space_.void_data()) Type(p1, p2, p3, p4, p5, p6, p7);
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5,
            typename T6, typename T7, typename T8>
  inline void Init(const T1& p1, const T2& p2, const T3& p3, const T4& p4,
                   const T5& p5, const T6& p6, const T7& p7, const T8& p8) {
    new(space_.void_data()) Type(p1, p2, p3, p4, p5, p6, p7, p8);
  }

  inline void Destroy() {
    get()->~Type();
  }

 private:
#if defined(COMPILER_MSVC)
  AlignedMemory<sizeof(Type), __alignof(Type)> space_;
#else
  AlignedMemory<sizeof(Type), __alignof__(Type)> space_;
#endif
};

}  

#endif  
