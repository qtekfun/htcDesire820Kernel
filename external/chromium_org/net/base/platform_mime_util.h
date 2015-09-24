// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_PLATFORM_MIME_UTIL_H_
#define NET_BASE_PLATFORM_MIME_UTIL_H_

#include <string>

#include "base/containers/hash_tables.h"
#include "base/files/file_path.h"

namespace net {

class PlatformMimeUtil {
 public:
  
  bool GetPreferredExtensionForMimeType(
      const std::string& mime_type,
      base::FilePath::StringType* extension) const;

  
  
  
  void GetPlatformExtensionsForMimeType(
      const std::string& mime_type,
      base::hash_set<base::FilePath::StringType>* extensions) const;

 protected:
  
  
  bool GetPlatformMimeTypeFromExtension(const base::FilePath::StringType& ext,
                                        std::string* mime_type) const;
};

}  

#endif  
