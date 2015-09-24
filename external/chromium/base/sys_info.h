// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_SYS_INFO_H_
#define BASE_SYS_INFO_H_
#pragma once

#include "base/base_api.h"
#include "base/basictypes.h"

#include <string>

class FilePath;

namespace base {

class BASE_API SysInfo {
 public:
  
  static int NumberOfProcessors();

  
  static int64 AmountOfPhysicalMemory();

  
  static int AmountOfPhysicalMemoryMB() {
    return static_cast<int>(AmountOfPhysicalMemory() / 1024 / 1024);
  }

  
  
  static int64 AmountOfFreeDiskSpace(const FilePath& path);

  
  static std::string OperatingSystemName();

  
  static std::string OperatingSystemVersion();

  
  
  
  static void OperatingSystemVersionNumbers(int32* major_version,
                                            int32* minor_version,
                                            int32* bugfix_version);

  
  
  static std::string CPUArchitecture();

  
  
  static void GetPrimaryDisplayDimensions(int* width, int* height);

  
  static int DisplayCount();

  
  
  static size_t VMAllocationGranularity();

#if defined(OS_POSIX) && !defined(OS_MACOSX)
  
  static size_t MaxSharedMemorySize();
#endif

#if defined(OS_CHROMEOS)
  
  
  static std::string GetLinuxStandardBaseVersionKey();

  
  
  static void ParseLsbRelease(const std::string& lsb_release,
                              int32* major_version,
                              int32* minor_version,
                              int32* bugfix_version);
#endif
};

}  

#endif  
