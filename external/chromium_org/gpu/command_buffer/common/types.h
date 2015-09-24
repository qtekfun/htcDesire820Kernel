// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef GPU_COMMAND_BUFFER_COMMON_TYPES_H_
#define GPU_COMMAND_BUFFER_COMMON_TYPES_H_

#if !defined(_MSC_VER)
#include <stdint.h>
#endif
#include <cstddef>
#include <string>

typedef signed char         schar;
typedef signed char         int8;
#ifndef _INT16
#define _INT16
typedef short               int16;
#endif
#ifndef _INT32
#define _INT32
typedef int                 int32;
#endif

#if defined(__LP64__) && !defined(__APPLE__) && !defined(__OpenBSD__)
typedef long                int64;
#else
typedef long long           int64;
#endif


typedef unsigned char      uint8;
#ifndef _UINT16
#define _UINT16
typedef unsigned short     uint16;
#endif
#ifndef _UINT32
#define _UINT32
typedef unsigned int       uint32;
#endif

#if defined(__LP64__) && !defined(__APPLE__) && !defined(__OpenBSD__)
typedef unsigned long uint64;
#else
typedef unsigned long long uint64;
#endif

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName();                                    \
  DISALLOW_COPY_AND_ASSIGN(TypeName)


template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];

#if !defined(_MSC_VER)
template <typename T, size_t N>
char (&ArraySizeHelper(const T (&array)[N]))[N];
#endif

#define arraysize(array) (sizeof(ArraySizeHelper(array)))


template <bool>
struct GpuCompileAssert {
};

#undef COMPILE_ASSERT
#define COMPILE_ASSERT(expr, msg) \
  typedef GpuCompileAssert<(bool(expr))> msg[bool(expr) ? 1 : -1]

//   to work around a bug in gcc 3.4.4 and 4.0.1.  If we had written

namespace gpu {
#if defined(_MSC_VER)
typedef short Int16;
typedef unsigned short Uint16;
typedef int Int32;
typedef unsigned int Uint32;
#else
typedef int16_t Int16;
typedef uint16_t Uint16;
typedef int32_t Int32;
typedef uint32_t Uint32;
#endif

typedef std::string String;

}  

#endif  
