// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_THREADING_THREAD_RESTRICTIONS_H_
#define BASE_THREADING_THREAD_RESTRICTIONS_H_

#include "base/base_api.h"
#include "base/basictypes.h"

namespace base {


class BASE_API ThreadRestrictions {
 public:
  
  
  class BASE_API ScopedAllowIO {
   public:
    ScopedAllowIO() { previous_value_ = SetIOAllowed(true); }
    ~ScopedAllowIO() { SetIOAllowed(previous_value_); }
   private:
    
    bool previous_value_;

    DISALLOW_COPY_AND_ASSIGN(ScopedAllowIO);
  };

  
  
  class BASE_API ScopedAllowSingleton {
   public:
    ScopedAllowSingleton() { previous_value_ = SetSingletonAllowed(true); }
    ~ScopedAllowSingleton() { SetSingletonAllowed(previous_value_); }
   private:
    
    
    bool previous_value_;

    DISALLOW_COPY_AND_ASSIGN(ScopedAllowSingleton);
  };

#ifndef NDEBUG
  
  
  
  static bool SetIOAllowed(bool allowed);

  
  
  
  static void AssertIOAllowed();

  
  
  static bool SetSingletonAllowed(bool allowed);

  
  
  static void AssertSingletonAllowed();
#else
  
  
  static bool SetIOAllowed(bool allowed) { return true; }
  static void AssertIOAllowed() {}
  static bool SetSingletonAllowed(bool allowed) { return true; }
  static void AssertSingletonAllowed() {}
#endif

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(ThreadRestrictions);
};

}  

#endif  
