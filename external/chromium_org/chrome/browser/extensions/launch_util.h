// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_LAUNCH_UTIL_H_
#define CHROME_BROWSER_EXTENSIONS_LAUNCH_UTIL_H_

#include <string>

#include "chrome/common/extensions/extension_constants.h"

namespace extensions {

class Extension;
class ExtensionPrefs;

LaunchType GetLaunchType(const ExtensionPrefs* prefs,
                         const Extension* extension);

void SetLaunchType(ExtensionPrefs* prefs,
                   const std::string& extension_id,
                   LaunchType launch_type);

LaunchContainer GetLaunchContainer(const ExtensionPrefs* prefs,
                                   const Extension* extension);

bool HasPreferredLaunchContainer(const ExtensionPrefs* prefs,
                                 const Extension* extension);

}  

#endif  
