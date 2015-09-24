// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_FILE_SYSTEM_OPTIONS_H_
#define WEBKIT_BROWSER_FILEAPI_FILE_SYSTEM_OPTIONS_H_

#include <string>
#include <vector>

#include "webkit/browser/webkit_storage_browser_export.h"

namespace fileapi {

class WEBKIT_STORAGE_BROWSER_EXPORT FileSystemOptions {
 public:
  enum ProfileMode {
    PROFILE_MODE_NORMAL = 0,
    PROFILE_MODE_INCOGNITO
  };

  
  
  
  
  
  FileSystemOptions(
      ProfileMode profile_mode,
      const std::vector<std::string>& additional_allowed_schemes);

  ~FileSystemOptions();

  
  bool is_incognito() const { return profile_mode_ == PROFILE_MODE_INCOGNITO; }

  
  
  
  
  const std::vector<std::string>& additional_allowed_schemes() const {
    return additional_allowed_schemes_;
  }

 private:
  const ProfileMode profile_mode_;
  const std::vector<std::string> additional_allowed_schemes_;
};

}  

#endif  
