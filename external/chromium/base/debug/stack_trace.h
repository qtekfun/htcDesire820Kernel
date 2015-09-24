// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_DEBUG_STACK_TRACE_H_
#define BASE_DEBUG_STACK_TRACE_H_
#pragma once

#include <iosfwd>

#include "base/base_api.h"
#include "build/build_config.h"

#if defined(OS_WIN)
struct _EXCEPTION_POINTERS;
#endif

namespace base {
namespace debug {

class BASE_API StackTrace {
 public:
  
  StackTrace();

#if defined(OS_WIN)
  
  
  
  StackTrace(_EXCEPTION_POINTERS* exception_pointers);
#endif

  

  ~StackTrace();

  
  
  const void* const* Addresses(size_t* count) const;

  
  void PrintBacktrace() const;

  
  void OutputToStream(std::ostream* os) const;

 private:
  
  
  
  
  static const int kMaxTraces = 62;

  void* trace_[kMaxTraces];
  int count_;
};

}  
}  

#endif  
