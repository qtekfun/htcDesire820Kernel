// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_CASTS_H_
#define BASE_CASTS_H_

#include <assert.h>         
#include <string.h>         

#include "base/macros.h"


template<typename To, typename From>
inline To implicit_cast(From const &f) {
  return f;
}



template<typename To, typename From>     
inline To down_cast(From* f) {                   
  
  
  
  
  if (false) {
    implicit_cast<From*, To>(0);
  }

  assert(f == NULL || dynamic_cast<To>(f) != NULL);  
  return static_cast<To>(f);
}

template<typename To, typename From>
inline To down_cast(From& f) {
  COMPILE_ASSERT(base::is_reference<To>::value, target_type_not_a_reference);
  typedef typename base::remove_reference<To>::type* ToAsPointer;
  if (false) {
    
    implicit_cast<From*, ToAsPointer>(0);
  }

  assert(dynamic_cast<ToAsPointer>(&f) != NULL);  
  return static_cast<To>(f);
}


template <class Dest, class Source>
inline Dest bit_cast(const Source& source) {
  
  
  typedef char VerifySizesAreEqual [sizeof(Dest) == sizeof(Source) ? 1 : -1];

  Dest dest;
  memcpy(&dest, &source, sizeof(dest));
  return dest;
}

#endif  
