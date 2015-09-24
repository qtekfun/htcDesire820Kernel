// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_FILE_VERSION_INFO_H__
#define BASE_FILE_VERSION_INFO_H__
#pragma once

#include <string>

#include "base/base_api.h"
#include "base/string16.h"
#include "build/build_config.h"

class FilePath;


class BASE_API FileVersionInfo {
 public:
  virtual ~FileVersionInfo() {}
#if defined(OS_WIN) || defined(OS_MACOSX)
  
  
  
  static FileVersionInfo* CreateFileVersionInfo(const FilePath& file_path);
#endif  

  
  
  static FileVersionInfo* CreateFileVersionInfoForCurrentModule();

  
  
  virtual string16 company_name() = 0;
  virtual string16 company_short_name() = 0;
  virtual string16 product_name() = 0;
  virtual string16 product_short_name() = 0;
  virtual string16 internal_name() = 0;
  virtual string16 product_version() = 0;
  virtual string16 private_build() = 0;
  virtual string16 special_build() = 0;
  virtual string16 comments() = 0;
  virtual string16 original_filename() = 0;
  virtual string16 file_description() = 0;
  virtual string16 file_version() = 0;
  virtual string16 legal_copyright() = 0;
  virtual string16 legal_trademarks() = 0;
  virtual string16 last_change() = 0;
  virtual bool is_official_build() = 0;
};

#endif  
