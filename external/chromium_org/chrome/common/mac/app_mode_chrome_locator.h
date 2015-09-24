// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_MAC_APP_MODE_CHROME_LOCATOR_H_
#define CHROME_COMMON_MAC_APP_MODE_CHROME_LOCATOR_H_

#include <CoreFoundation/CoreFoundation.h>

#include "base/strings/string16.h"

@class NSString;

namespace base {
class FilePath;
}

namespace app_mode {

bool FindBundleById(NSString* bundle_id, base::FilePath* out_bundle);

bool GetChromeBundleInfo(const base::FilePath& chrome_bundle,
                         base::string16* raw_version_str,
                         base::FilePath* version_path,
                         base::FilePath* framework_shlib_path);

}  

#endif  
