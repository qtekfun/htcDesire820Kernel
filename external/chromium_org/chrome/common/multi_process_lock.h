// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_MULTI_PROCESS_LOCK_H_
#define CHROME_COMMON_MULTI_PROCESS_LOCK_H_

#include <sys/types.h>
#include <string>

class MultiProcessLock {
 public:

  
  
  
  
  
  static MultiProcessLock* Create(const std::string& name);

  virtual ~MultiProcessLock() { }

  
  virtual bool TryLock() = 0;

  
  virtual void Unlock() = 0;
};

#endif  
