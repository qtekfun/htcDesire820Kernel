// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_PLATFORM_MIME_UTIL_H_
#define NET_BASE_PLATFORM_MIME_UTIL_H_
#pragma once

#include <string>

#include "base/file_path.h"

namespace net {

class PlatformMimeUtil {
 public:
  
  bool GetPreferredExtensionForMimeType(const std::string& mime_type,
                                        FilePath::StringType* extension) const;
 protected:

  
  
  bool GetPlatformMimeTypeFromExtension(const FilePath::StringType& ext,
                                        std::string* mime_type) const;
};

}  

#endif  
