// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_STRINGS_SAFE_SPRINTF_H_
#define BASE_STRINGS_SAFE_SPRINTF_H_

#include "build/build_config.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#if defined(OS_POSIX)
#include <unistd.h>
#endif

#include "base/base_export.h"
#include "base/basictypes.h"

namespace base {
namespace strings {

#if defined(_MSC_VER)
#if defined(_WIN64)
typedef __int64 ssize_t;
#else
typedef long ssize_t;
#endif
#endif

// can be written). The return value can never be larger than SSIZE_MAX-1.

namespace internal {

struct Arg {
  enum Type { INT, UINT, STRING, POINTER };

  
  Arg(signed char c)        : i(c), width(sizeof(char)),      type(INT)  { }
  Arg(unsigned char c)      : i(c), width(sizeof(char)),      type(UINT) { }
  Arg(signed short j)       : i(j), width(sizeof(short)),     type(INT)  { }
  Arg(unsigned short j)     : i(j), width(sizeof(short)),     type(UINT) { }
  Arg(signed int j)         : i(j), width(sizeof(int)),       type(INT)  { }
  Arg(unsigned int j)       : i(j), width(sizeof(int)),       type(UINT) { }
  Arg(signed long j)        : i(j), width(sizeof(long)),      type(INT)  { }
  Arg(unsigned long j)      : i(j), width(sizeof(long)),      type(UINT) { }
  Arg(signed long long j)   : i(j), width(sizeof(long long)), type(INT)  { }
  Arg(unsigned long long j) : i(j), width(sizeof(long long)), type(UINT) { }

  
  Arg(const char* s) : str(s), type(STRING) { }
  Arg(char* s)       : str(s), type(STRING) { }

  
  template<class T> Arg(T* p) : ptr((void*)p), type(POINTER) { }

  union {
    
    struct {
      int64_t       i;
      unsigned char width;
    };

    
    const char* str;

    
    const void* ptr;
  };
  const enum Type type;
};

BASE_EXPORT ssize_t SafeSNPrintf(char* buf, size_t sz, const char* fmt,
                                 const Arg* args, size_t max_args);

#if !defined(NDEBUG)
BASE_EXPORT void SetSafeSPrintfSSizeMaxForTest(size_t max);
BASE_EXPORT size_t GetSafeSPrintfSSizeMaxForTest();
#endif

}  


template<class T0, class T1, class T2, class T3, class T4,
         class T5, class T6, class T7, class T8, class T9>
ssize_t SafeSNPrintf(char* buf, size_t N, const char* fmt,
                     T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                     T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9) {
  
  
  const internal::Arg arg_array[] = {
    arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9
  };
  return internal::SafeSNPrintf(buf, N, fmt, arg_array, arraysize(arg_array));
}

template<size_t N,
         class T0, class T1, class T2, class T3, class T4,
         class T5, class T6, class T7, class T8, class T9>
ssize_t SafeSPrintf(char (&buf)[N], const char* fmt,
                    T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                    T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9) {
  
  
  const internal::Arg arg_array[] = {
    arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9
  };
  return internal::SafeSNPrintf(buf, N, fmt, arg_array, arraysize(arg_array));
}

template<class T0, class T1, class T2, class T3, class T4,
         class T5, class T6, class T7, class T8>
ssize_t SafeSNPrintf(char* buf, size_t N, const char* fmt,
                     T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                     T5 arg5, T6 arg6, T7 arg7, T8 arg8) {
  
  
  const internal::Arg arg_array[] = {
    arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8
  };
  return internal::SafeSNPrintf(buf, N, fmt, arg_array, arraysize(arg_array));
}

template<size_t N,
         class T0, class T1, class T2, class T3, class T4, class T5,
         class T6, class T7, class T8>
ssize_t SafeSPrintf(char (&buf)[N], const char* fmt,
                    T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                    T5 arg5, T6 arg6, T7 arg7, T8 arg8) {
  
  
  const internal::Arg arg_array[] = {
    arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8
  };
  return internal::SafeSNPrintf(buf, N, fmt, arg_array, arraysize(arg_array));
}

template<class T0, class T1, class T2, class T3, class T4, class T5,
         class T6, class T7>
ssize_t SafeSNPrintf(char* buf, size_t N, const char* fmt,
                     T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                     T5 arg5, T6 arg6, T7 arg7) {
  
  
  const internal::Arg arg_array[] = {
    arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7
  };
  return internal::SafeSNPrintf(buf, N, fmt, arg_array, arraysize(arg_array));
}

template<size_t N,
         class T0, class T1, class T2, class T3, class T4, class T5,
         class T6, class T7>
ssize_t SafeSPrintf(char (&buf)[N], const char* fmt,
                    T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                    T5 arg5, T6 arg6, T7 arg7) {
  
  
  const internal::Arg arg_array[] = {
    arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7
  };
  return internal::SafeSNPrintf(buf, N, fmt, arg_array, arraysize(arg_array));
}

template<class T0, class T1, class T2, class T3, class T4, class T5,
         class T6>
ssize_t SafeSNPrintf(char* buf, size_t N, const char* fmt,
                     T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                     T5 arg5, T6 arg6) {
  
  
  const internal::Arg arg_array[] = {
    arg0, arg1, arg2, arg3, arg4, arg5, arg6
  };
  return internal::SafeSNPrintf(buf, N, fmt, arg_array, arraysize(arg_array));
}

template<size_t N,
         class T0, class T1, class T2, class T3, class T4, class T5,
         class T6>
ssize_t SafeSPrintf(char (&buf)[N], const char* fmt,
                    T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5,
                    T6 arg6) {
  
  
  const internal::Arg arg_array[] = {
    arg0, arg1, arg2, arg3, arg4, arg5, arg6
  };
  return internal::SafeSNPrintf(buf, N, fmt, arg_array, arraysize(arg_array));
}

template<class T0, class T1, class T2, class T3, class T4, class T5>
ssize_t SafeSNPrintf(char* buf, size_t N, const char* fmt,
                     T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5) {
  
  
  const internal::Arg arg_array[] = { arg0, arg1, arg2, arg3, arg4, arg5 };
  return internal::SafeSNPrintf(buf, N, fmt, arg_array, arraysize(arg_array));
}

template<size_t N,
         class T0, class T1, class T2, class T3, class T4, class T5>
ssize_t SafeSPrintf(char (&buf)[N], const char* fmt,
                    T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5) {
  
  
  const internal::Arg arg_array[] = { arg0, arg1, arg2, arg3, arg4, arg5 };
  return internal::SafeSNPrintf(buf, N, fmt, arg_array, arraysize(arg_array));
}

template<class T0, class T1, class T2, class T3, class T4>
ssize_t SafeSNPrintf(char* buf, size_t N, const char* fmt,
                     T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
  
  
  const internal::Arg arg_array[] = { arg0, arg1, arg2, arg3, arg4 };
  return internal::SafeSNPrintf(buf, N, fmt, arg_array, arraysize(arg_array));
}

template<size_t N, class T0, class T1, class T2, class T3, class T4>
ssize_t SafeSPrintf(char (&buf)[N], const char* fmt, T0 arg0, T1 arg1,
                    T2 arg2, T3 arg3, T4 arg4) {
  
  
  const internal::Arg arg_array[] = { arg0, arg1, arg2, arg3, arg4 };
  return internal::SafeSNPrintf(buf, N, fmt, arg_array, arraysize(arg_array));
}

template<class T0, class T1, class T2, class T3>
ssize_t SafeSNPrintf(char* buf, size_t N, const char* fmt,
                     T0 arg0, T1 arg1, T2 arg2, T3 arg3) {
  
  
  const internal::Arg arg_array[] = { arg0, arg1, arg2, arg3 };
  return internal::SafeSNPrintf(buf, N, fmt, arg_array, arraysize(arg_array));
}

template<size_t N, class T0, class T1, class T2, class T3>
ssize_t SafeSPrintf(char (&buf)[N], const char* fmt,
                    T0 arg0, T1 arg1, T2 arg2, T3 arg3) {
  
  
  const internal::Arg arg_array[] = { arg0, arg1, arg2, arg3 };
  return internal::SafeSNPrintf(buf, N, fmt, arg_array, arraysize(arg_array));
}

template<class T0, class T1, class T2>
ssize_t SafeSNPrintf(char* buf, size_t N, const char* fmt,
                     T0 arg0, T1 arg1, T2 arg2) {
  
  
  const internal::Arg arg_array[] = { arg0, arg1, arg2 };
  return internal::SafeSNPrintf(buf, N, fmt, arg_array, arraysize(arg_array));
}

template<size_t N, class T0, class T1, class T2>
ssize_t SafeSPrintf(char (&buf)[N], const char* fmt, T0 arg0, T1 arg1,
                    T2 arg2) {
  
  
  const internal::Arg arg_array[] = { arg0, arg1, arg2 };
  return internal::SafeSNPrintf(buf, N, fmt, arg_array, arraysize(arg_array));
}

template<class T0, class T1>
ssize_t SafeSNPrintf(char* buf, size_t N, const char* fmt, T0 arg0, T1 arg1) {
  
  
  const internal::Arg arg_array[] = { arg0, arg1 };
  return internal::SafeSNPrintf(buf, N, fmt, arg_array, arraysize(arg_array));
}

template<size_t N, class T0, class T1>
ssize_t SafeSPrintf(char (&buf)[N], const char* fmt, T0 arg0, T1 arg1) {
  
  
  const internal::Arg arg_array[] = { arg0, arg1 };
  return internal::SafeSNPrintf(buf, N, fmt, arg_array, arraysize(arg_array));
}

template<class T0>
ssize_t SafeSNPrintf(char* buf, size_t N, const char* fmt, T0 arg0) {
  
  
  const internal::Arg arg_array[] = { arg0 };
  return internal::SafeSNPrintf(buf, N, fmt, arg_array, arraysize(arg_array));
}

template<size_t N, class T0>
ssize_t SafeSPrintf(char (&buf)[N], const char* fmt, T0 arg0) {
  
  
  const internal::Arg arg_array[] = { arg0 };
  return internal::SafeSNPrintf(buf, N, fmt, arg_array, arraysize(arg_array));
}

BASE_EXPORT ssize_t SafeSNPrintf(char* buf, size_t N, const char* fmt);
template<size_t N>
inline ssize_t SafeSPrintf(char (&buf)[N], const char* fmt) {
  return SafeSNPrintf(buf, N, fmt);
}

}  
}  

#endif  
