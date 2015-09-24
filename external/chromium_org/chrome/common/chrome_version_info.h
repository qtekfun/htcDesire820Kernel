// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_CHROME_VERSION_INFO_H_
#define CHROME_COMMON_CHROME_VERSION_INFO_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

class FileVersionInfo;

namespace chrome {

class VersionInfo {
 public:
  
  enum Channel {
    CHANNEL_UNKNOWN = 0,  
    CHANNEL_CANARY,       
    CHANNEL_DEV,          
    CHANNEL_BETA,         
    CHANNEL_STABLE        
  };

  VersionInfo();
  ~VersionInfo();

  
  
  
  
  bool is_valid() const;

  
  std::string ProductNameAndVersionForUserAgent() const;

  
  std::string Name() const;

  
  std::string Version() const;

  
  std::string LastChange() const;

  
  
  
  bool IsOfficialBuild() const;

  
  std::string OSType() const;

  
  
  
  
  
  
  
  
  
  
  static std::string GetVersionStringModifier();

  
  
  
  
  static Channel GetChannel();

#if defined(OS_CHROMEOS)
  
  static void SetChannel(const std::string& channel);
#endif

  
  std::string CreateVersionString() const;

 private:
#if defined(OS_WIN) || defined(OS_MACOSX)
  scoped_ptr<FileVersionInfo> version_info_;
#endif

  DISALLOW_COPY_AND_ASSIGN(VersionInfo);
};

}  

#endif  
