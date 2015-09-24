// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_CHROME_VERSION_INFO_H_
#define CHROME_COMMON_CHROME_VERSION_INFO_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

class FileVersionInfo;

namespace chrome {

class VersionInfo {
 public:
  VersionInfo();
  ~VersionInfo();

  
  
  
  
  bool is_valid() const;

  
  std::string Name() const;

  
  std::string Version() const;

  
  std::string LastChange() const;

  
  
  
  bool IsOfficialBuild() const;

 private:
#if defined(OS_WIN) || defined(OS_MACOSX)
  scoped_ptr<FileVersionInfo> version_info_;
#endif

  DISALLOW_COPY_AND_ASSIGN(VersionInfo);
};

}  

#endif  
