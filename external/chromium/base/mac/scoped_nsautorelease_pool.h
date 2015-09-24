// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MAC_SCOPED_NSAUTORELEASE_POOL_H_
#define BASE_MAC_SCOPED_NSAUTORELEASE_POOL_H_
#pragma once

#include "base/basictypes.h"

#if defined(OS_MACOSX)
#if defined(__OBJC__)
@class NSAutoreleasePool;
#else  
class NSAutoreleasePool;
#endif  
#endif  

namespace base {
namespace mac {

class ScopedNSAutoreleasePool {
 public:
#if !defined(OS_MACOSX)
  ScopedNSAutoreleasePool() {}
  void Recycle() { }
#else  
  ScopedNSAutoreleasePool();
  ~ScopedNSAutoreleasePool();

  
  
  
  
  void Recycle();
 private:
  NSAutoreleasePool* autorelease_pool_;
#endif  

 private:
  DISALLOW_COPY_AND_ASSIGN(ScopedNSAutoreleasePool);
};

}  
}  

#endif  
