// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MEMORY_DISCARDABLE_MEMORY_H_
#define BASE_MEMORY_DISCARDABLE_MEMORY_H_

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"

namespace base {

enum LockDiscardableMemoryStatus {
  DISCARDABLE_MEMORY_FAILED = -1,
  DISCARDABLE_MEMORY_PURGED = 0,
  DISCARDABLE_MEMORY_SUCCESS = 1
};

class BASE_EXPORT DiscardableMemory {
 public:
  virtual ~DiscardableMemory() {}

  
  
  static bool SupportedNatively();

  static scoped_ptr<DiscardableMemory> CreateLockedMemory(size_t size);

  
  
  
  
  
  
  virtual LockDiscardableMemoryStatus Lock() WARN_UNUSED_RESULT = 0;

  
  
  virtual void Unlock() = 0;

  
  
  virtual void* Memory() const = 0;

  

  
  
  static bool PurgeForTestingSupported();

  
  
  static void PurgeForTesting();
};

}  

#endif  
