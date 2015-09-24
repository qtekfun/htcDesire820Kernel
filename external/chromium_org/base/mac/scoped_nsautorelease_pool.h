// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MAC_SCOPED_NSAUTORELEASE_POOL_H_
#define BASE_MAC_SCOPED_NSAUTORELEASE_POOL_H_

#include "base/base_export.h"
#include "base/basictypes.h"

#if defined(__OBJC__)
@class NSAutoreleasePool;
#else  
class NSAutoreleasePool;
#endif  

namespace base {
namespace mac {


class BASE_EXPORT ScopedNSAutoreleasePool {
 public:
  ScopedNSAutoreleasePool();
  ~ScopedNSAutoreleasePool();

  
  
  
  
  void Recycle();
 private:
  NSAutoreleasePool* autorelease_pool_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ScopedNSAutoreleasePool);
};

}  
}  

#endif  
