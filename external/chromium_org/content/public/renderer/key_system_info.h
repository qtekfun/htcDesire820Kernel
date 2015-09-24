// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_RENDERER_KEY_SYSTEM_INFO_H_
#define CONTENT_PUBLIC_RENDERER_KEY_SYSTEM_INFO_H_

#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "content/common/content_export.h"


namespace content {

struct CONTENT_EXPORT KeySystemInfo {
  
  
  typedef std::pair<std::string, std::string> ContainerCodecsPair;

  explicit KeySystemInfo(const std::string& key_system);
  ~KeySystemInfo();

  std::string key_system;

  
  
  
  std::vector<ContainerCodecsPair> supported_types;

  
  
  
  std::string parent_key_system;

  
  bool use_aes_decryptor;
#if defined(ENABLE_PEPPER_CDMS)
  std::string pepper_type;
#elif defined(OS_ANDROID)
  std::vector<uint8> uuid;
#endif
};

}  

#endif  
