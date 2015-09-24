// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_FEATURES_FEATURE_CHANNEL_H_
#define CHROME_COMMON_EXTENSIONS_FEATURES_FEATURE_CHANNEL_H_

#include "chrome/common/chrome_version_info.h"

namespace extensions {

chrome::VersionInfo::Channel GetCurrentChannel();

void SetCurrentChannel(chrome::VersionInfo::Channel channel);

chrome::VersionInfo::Channel GetDefaultChannel();

class ScopedCurrentChannel {
 public:
  explicit ScopedCurrentChannel(chrome::VersionInfo::Channel channel)
      : original_channel_(chrome::VersionInfo::CHANNEL_UNKNOWN) {
    original_channel_ = GetCurrentChannel();
    SetCurrentChannel(channel);
  }

  ~ScopedCurrentChannel() {
    SetCurrentChannel(original_channel_);
  }

 private:
  chrome::VersionInfo::Channel original_channel_;
};

}  

#endif  
