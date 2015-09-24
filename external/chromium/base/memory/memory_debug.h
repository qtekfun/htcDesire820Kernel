// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_MEMORY_MEMORY_DEBUG_H_
#define BASE_MEMORY_MEMORY_DEBUG_H_
#pragma once

#include "base/base_api.h"
#include "base/basictypes.h"

namespace base {

class BASE_API MemoryDebug {
 public:
  
  
  static void SetMemoryInUseEnabled(bool enabled);

  
  static void DumpAllMemoryInUse();
  
  
  static void DumpNewMemoryInUse();

  
  static void DumpAllLeaks();
  
  
  static void DumpNewLeaks();

  
  
  static void MarkAsInitialized(void* addr, size_t size);

 private:
  static bool memory_in_use_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(MemoryDebug);
};

}  

#endif  
