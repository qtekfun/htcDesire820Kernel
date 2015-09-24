// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_GOOGLE_UPDATE_UTIL_H_
#define CHROME_INSTALLER_UTIL_GOOGLE_UPDATE_UTIL_H_

#include <string>

namespace google_update {

bool EnsureUserLevelGoogleUpdatePresent();

bool UninstallGoogleUpdate(bool system_install);

std::string GetUntrustedDataValue(const std::string& key);

}  

#endif  
