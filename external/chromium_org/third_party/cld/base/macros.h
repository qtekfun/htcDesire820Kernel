// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MACROS_H_
#define BASE_MACROS_H_

#include <stddef.h>         

#include "base/type_traits.h"



#define COMPILE_ASSERT(expr, msg) \
  typedef CompileAssert<(bool(expr))> msg[bool(expr) ? 1 : -1]

//   to work around a bug in gcc 3.4.4 and 4.0.1.  If we had written


#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#define DISALLOW_EVIL_CONSTRUCTORS(TypeName) DISALLOW_COPY_AND_ASSIGN(TypeName)

#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName();                                    \
  DISALLOW_COPY_AND_ASSIGN(TypeName)


template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];

#ifndef COMPILER_MSVC
template <typename T, size_t N>
char (&ArraySizeHelper(const T (&array)[N]))[N];
#endif

#define arraysize(array) (sizeof(ArraySizeHelper(array)))

#if !defined(COMPILER_MSVC) || (defined(_MSC_VER) && _MSC_VER < 1400)
#define ARRAYSIZE(a) \
  ((sizeof(a) / sizeof(*(a))) / \
   static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))
#endif

#define AS_STRING(x)   AS_STRING_INTERNAL(x)
#define AS_STRING_INTERNAL(x)   #x



#define DECLARE_POD(TypeName)                       \
namespace base {                                    \
template<> struct is_pod<TypeName> : true_type { }; \
}                                                   \
typedef int Dummy_Type_For_DECLARE_POD              \

#define DECLARE_NESTED_POD(TypeName) DECLARE_POD(TypeName)

#define PROPAGATE_POD_FROM_TEMPLATE_ARGUMENT(TemplateName)             \
namespace base {                                                       \
template <typename T> struct is_pod<TemplateName<T> > : is_pod<T> { }; \
}                                                                      \
typedef int Dummy_Type_For_PROPAGATE_POD_FROM_TEMPLATE_ARGUMENT


template <typename Boolean> struct ERROR_TYPE_MUST_BE_POD;
template <> struct ERROR_TYPE_MUST_BE_POD<base::true_type> { };
#define ENFORCE_POD(TypeName)                                             \
  enum { dummy_##TypeName                                                 \
           = sizeof(ERROR_TYPE_MUST_BE_POD<                               \
                      typename base::is_pod<TypeName>::type>) }

#endif  
