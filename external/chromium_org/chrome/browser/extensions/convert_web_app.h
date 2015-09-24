// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_CONVERT_WEB_APP_H_
#define CHROME_BROWSER_EXTENSIONS_CONVERT_WEB_APP_H_

#include <string>

#include "base/memory/ref_counted.h"

namespace base {
class FilePath;
class Time;
}

struct WebApplicationInfo;

namespace extensions {
class Extension;

std::string ConvertTimeToExtensionVersion(const base::Time& time);

scoped_refptr<Extension> ConvertWebAppToExtension(
    const WebApplicationInfo& web_app_info,
    const base::Time& create_time,
    const base::FilePath& extensions_dir);

}  

#endif  
