// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_SYSTEM_MACROS_H_
#define MOJO_PUBLIC_SYSTEM_MACROS_H_

#include <stddef.h>

#if defined(__GNUC__)
#define MOJO_ALLOW_UNUSED __attribute__((unused))
#else
#define MOJO_ALLOW_UNUSED
#endif

#if defined(__GNUC__)
#define MOJO_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
#define MOJO_WARN_UNUSED_RESULT
#endif

#ifdef __cplusplus

#if defined(_MSC_VER) || defined(__clang__)
#define MOJO_OVERRIDE override
#else
#define MOJO_OVERRIDE
#endif

#define MOJO_DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&); \
    void operator=(const TypeName&)

namespace mojo { template <bool> struct CompileAssert {}; }
#define MOJO_COMPILE_ASSERT(expr, msg) \
    typedef ::mojo::CompileAssert<(bool(expr))> msg[bool(expr) ? 1 : -1]

namespace mojo {
template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];
#if !defined(_MSC_VER)
template <typename T, size_t N>
char (&ArraySizeHelper(const T (&array)[N]))[N];
#endif
}  
#define MOJO_ARRAYSIZE(array) (sizeof(::mojo::ArraySizeHelper(array)))

#define MOJO_MOVE_ONLY_TYPE_FOR_CPP_03(type, rvalue_type) \
 private: \
  struct rvalue_type { \
    explicit rvalue_type(type* object) : object(object) {} \
    type* object; \
  }; \
  type(type&); \
  void operator=(type&); \
 public: \
  operator rvalue_type() { return rvalue_type(this); } \
  type Pass() { return type(rvalue_type(this)); } \
  typedef void MoveOnlyTypeForCPP03; \
 private:

#endif  

#endif  
