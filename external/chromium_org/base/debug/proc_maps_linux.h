// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_DEBUG_PROC_MAPS_LINUX_H_
#define BASE_DEBUG_PROC_MAPS_LINUX_H_

#include <string>
#include <vector>

#include "base/base_export.h"
#include "base/basictypes.h"

namespace base {
namespace debug {

struct MappedMemoryRegion {
  enum Permission {
    READ = 1 << 0,
    WRITE = 1 << 1,
    EXECUTE = 1 << 2,
    PRIVATE = 1 << 3,  
  };

  
  uintptr_t start;
  uintptr_t end;

  
  unsigned long long offset;

  
  uint8 permissions;

  
  
  
  
  
  std::string path;
};

BASE_EXPORT bool ReadProcMaps(std::string* proc_maps);

BASE_EXPORT bool ParseProcMaps(const std::string& input,
                               std::vector<MappedMemoryRegion>* regions);

}  
}  

#endif  
