// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_SYS_INFO_H_
#define BASE_SYS_INFO_H_

#include <map>
#include <string>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/time/time.h"
#include "build/build_config.h"

namespace base {

class BASE_EXPORT SysInfo {
 public:
  
  static int NumberOfProcessors();

  
  static int64 AmountOfPhysicalMemory();

  
  
  static int64 AmountOfAvailablePhysicalMemory();

  
  static int AmountOfPhysicalMemoryMB() {
    return static_cast<int>(AmountOfPhysicalMemory() / 1024 / 1024);
  }

  
  
  static int64 AmountOfFreeDiskSpace(const FilePath& path);

  
  static int64 Uptime();

  
  static std::string OperatingSystemName();

  
  static std::string OperatingSystemVersion();

  
  
  
  
  
  
  
  
  static void OperatingSystemVersionNumbers(int32* major_version,
                                            int32* minor_version,
                                            int32* bugfix_version);

  
  
  
  
  static std::string OperatingSystemArchitecture();

  
  
  
  
  static std::string CPUModelName();

  
  
  static size_t VMAllocationGranularity();

#if defined(OS_POSIX) && !defined(OS_MACOSX)
  
  
  static size_t MaxSharedMemorySize();
#endif  

#if defined(OS_CHROMEOS)
  typedef std::map<std::string, std::string> LsbReleaseMap;

  
  static const LsbReleaseMap& GetLsbReleaseMap();

  
  static bool GetLsbReleaseValue(const std::string& key, std::string* value);

  
  
  static std::string GetLsbReleaseBoard();

  
  
  static Time GetLsbReleaseTime();

  
  static bool IsRunningOnChromeOS();

  
  static void SetChromeOSVersionInfoForTest(const std::string& lsb_release,
                                            const Time& lsb_release_time);
#endif  

#if defined(OS_ANDROID)
  
  static std::string GetAndroidBuildCodename();

  
  static std::string GetAndroidBuildID();

  
  static std::string GetDeviceName();

  static int DalvikHeapSizeMB();
  static int DalvikHeapGrowthLimitMB();
#endif  
};

}  

#endif  
