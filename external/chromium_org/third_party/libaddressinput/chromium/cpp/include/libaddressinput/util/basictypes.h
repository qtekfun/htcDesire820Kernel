// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef I18N_ADDRESSINPUT_UTIL_BASICTYPES_H_
#define I18N_ADDRESSINPUT_UTIL_BASICTYPES_H_

#include <limits.h>         
#include <stddef.h>         
#include <string.h>         

#if !defined(_WIN32)
#include <stdint.h>         
#endif

#ifdef INT64_MAX

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

const uint8  kuint8max  = UINT8_MAX;
const uint16 kuint16max = UINT16_MAX;
const uint32 kuint32max = UINT32_MAX;
const uint64 kuint64max = UINT64_MAX;
const  int8  kint8min   = INT8_MIN;
const  int8  kint8max   = INT8_MAX;
const  int16 kint16min  = INT16_MIN;
const  int16 kint16max  = INT16_MAX;
const  int32 kint32min  = INT32_MIN;
const  int32 kint32max  = INT32_MAX;
const  int64 kint64min  = INT64_MIN;
const  int64 kint64max  = INT64_MAX;

#else 

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

#endif 

typedef signed char         schar;

typedef signed int         char32;

#if !defined(DISALLOW_COPY_AND_ASSIGN)
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)
#endif


template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];

#ifndef _MSC_VER
template <typename T, size_t N>
char (&ArraySizeHelper(const T (&array)[N]))[N];
#endif

#if !defined(arraysize)
#define arraysize(array) (sizeof(ArraySizeHelper(array)))
#endif


#if !defined(ARRAYSIZE_UNSAFE)
#define ARRAYSIZE_UNSAFE(a) \
  ((sizeof(a) / sizeof(*(a))) / \
   static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))
#endif


template <bool>
struct CompileAssert {
};

#if !defined(COMPILE_ASSERT)
#define COMPILE_ASSERT(expr, msg) \
  typedef CompileAssert<(bool(expr))> msg[bool(expr) ? 1 : -1]
#endif

#endif  
