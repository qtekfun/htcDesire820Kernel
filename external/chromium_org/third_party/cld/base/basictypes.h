// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_BASICTYPES_H_
#define BASE_BASICTYPES_H_

#include <limits.h>         
#include <stddef.h>         
#include <string.h>         

#include "base/port.h"    

#ifndef COMPILER_MSVC
#include <stdint.h>         
#endif

typedef signed char         schar;
typedef signed char         int8;
typedef short               int16;
#ifndef _INT32
#define _INT32
typedef int                 int32;
#endif

#if __LP64__
typedef long                int64;
#else
typedef long long           int64;
#endif


typedef unsigned char      uint8;
typedef unsigned short     uint16;
#ifndef _UINT32
#define _UINT32
typedef unsigned int       uint32;
#endif

#if __LP64__
typedef unsigned long uint64;
#else
typedef unsigned long long uint64;
#endif

typedef signed int         char32;

const uint8  kuint8max  = (( uint8) 0xFF);
const uint16 kuint16max = ((uint16) 0xFFFF);
const uint32 kuint32max = ((uint32) 0xFFFFFFFF);
const uint64 kuint64max = ((uint64) GG_LONGLONG(0xFFFFFFFFFFFFFFFF));
const  int8  kint8min   = ((  int8) 0x80);
const  int8  kint8max   = ((  int8) 0x7F);
const  int16 kint16min  = (( int16) 0x8000);
const  int16 kint16max  = (( int16) 0x7FFF);
const  int32 kint32min  = (( int32) 0x80000000);
const  int32 kint32max  = (( int32) 0x7FFFFFFF);
const  int64 kint64min  = (( int64) GG_LONGLONG(0x8000000000000000));
const  int64 kint64max  = (( int64) GG_LONGLONG(0x7FFFFFFFFFFFFFFF));

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#define DISALLOW_EVIL_CONSTRUCTORS(TypeName) DISALLOW_COPY_AND_ASSIGN(TypeName)

#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName();                                    \
  DISALLOW_COPY_AND_ASSIGN(TypeName)


template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];

#ifndef _MSC_VER
template <typename T, size_t N>
char (&ArraySizeHelper(const T (&array)[N]))[N];
#endif

#define arraysize(array) (sizeof(ArraySizeHelper(array)))


#define ARRAYSIZE_UNSAFE(a) \
  ((sizeof(a) / sizeof(*(a))) / \
   static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))


template<typename To, typename From>
inline To implicit_cast(From const &f) {
  return f;
}


template <bool>
struct CompileAssert {
};

#undef COMPILE_ASSERT
#define COMPILE_ASSERT(expr, msg) \
  typedef CompileAssert<(bool(expr))> msg[bool(expr) ? 1 : -1]

//   to work around a bug in gcc 3.4.4 and 4.0.1.  If we had written


typedef uint32 MetatagId;

enum Ownership {
  DO_NOT_TAKE_OWNERSHIP,
  TAKE_OWNERSHIP
};


template <class Dest, class Source>
inline Dest bit_cast(const Source& source) {
  
  
  typedef char VerifySizesAreEqual [sizeof(Dest) == sizeof(Source) ? 1 : -1];

  Dest dest;
  memcpy(&dest, &source, sizeof(dest));
  return dest;
}

namespace base {
enum LinkerInitialized { LINKER_INITIALIZED };
}  

#if defined(ARCH_CPU_X86_FAMILY)
inline uint32 UnalignedLoad32(const void* p) {
  return *reinterpret_cast<const uint32*>(p);
}
#else
#define NEED_ALIGNED_LOADS
inline uint32 UnalignedLoad32(const void* p) {
  uint32 t;
  memcpy(&t, reinterpret_cast<const uint8*>(p), sizeof(t));
  return t;
}

#endif
#endif  
