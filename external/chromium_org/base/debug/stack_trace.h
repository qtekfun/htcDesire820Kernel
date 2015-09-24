// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_DEBUG_STACK_TRACE_H_
#define BASE_DEBUG_STACK_TRACE_H_

#include <iosfwd>
#include <string>

#include "base/base_export.h"
#include "build/build_config.h"

#if defined(OS_POSIX)
#include <unistd.h>
#endif

#if defined(OS_WIN)
struct _EXCEPTION_POINTERS;
#endif

namespace base {
namespace debug {

BASE_EXPORT bool EnableInProcessStackDumping();

class BASE_EXPORT StackTrace {
 public:
  
  StackTrace();

  
  
  
  StackTrace(const void* const* trace, size_t count);

#if defined(OS_WIN)
  
  
  
  StackTrace(_EXCEPTION_POINTERS* exception_pointers);
#endif

  

  ~StackTrace();

  
  
  const void* const* Addresses(size_t* count) const;

  
  void Print() const;

  
  void OutputToStream(std::ostream* os) const;

  
  std::string ToString() const;

 private:
  
  
  
  
  static const int kMaxTraces = 62;

  void* trace_[kMaxTraces];

  
  size_t count_;
};

namespace internal {

#if defined(OS_POSIX) && !defined(OS_ANDROID)
BASE_EXPORT char *itoa_r(intptr_t i,
                         char *buf,
                         size_t sz,
                         int base,
                         size_t padding);
#endif  

}  

}  
}  

#endif  
