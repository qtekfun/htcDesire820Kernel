// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_WIN_WINDOWS_VERSION_H_
#define BASE_WIN_WINDOWS_VERSION_H_
#pragma once

#include "base/base_api.h"
#include "base/memory/singleton.h"

typedef void* HANDLE;

namespace base {
namespace win {

enum Version {
  VERSION_PRE_XP = 0,   
  VERSION_XP,
  VERSION_SERVER_2003,  
  VERSION_VISTA,
  VERSION_SERVER_2008,
  VERSION_WIN7,
};

class BASE_API OSInfo {
 public:
  struct VersionNumber {
    int major;
    int minor;
    int build;
  };

  struct ServicePack {
    int major;
    int minor;
  };

  
  
  
  
  
  enum WindowsArchitecture {
    X86_ARCHITECTURE,
    X64_ARCHITECTURE,
    IA64_ARCHITECTURE,
    OTHER_ARCHITECTURE,
  };

  
  
  
  
  
  enum WOW64Status {
    WOW64_DISABLED,
    WOW64_ENABLED,
    WOW64_UNKNOWN,
  };

  static OSInfo* GetInstance();

  Version version() const { return version_; }
  
  VersionNumber version_number() const { return version_number_; }
  ServicePack service_pack() const { return service_pack_; }
  WindowsArchitecture architecture() const { return architecture_; }
  int processors() const { return processors_; }
  size_t allocation_granularity() const { return allocation_granularity_; }
  WOW64Status wow64_status() const { return wow64_status_; }

  
  
  static WOW64Status GetWOW64StatusForProcess(HANDLE process_handle);

 private:
  OSInfo();
  ~OSInfo();

  Version version_;
  VersionNumber version_number_;
  ServicePack service_pack_;
  WindowsArchitecture architecture_;
  int processors_;
  size_t allocation_granularity_;
  WOW64Status wow64_status_;

  friend struct DefaultSingletonTraits<OSInfo>;
  DISALLOW_COPY_AND_ASSIGN(OSInfo);
};

BASE_API Version GetVersion();

}  
}  

#endif  
