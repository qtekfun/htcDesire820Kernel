// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_METRO_UTILS_METRO_CHROME_WIN_H_
#define CHROME_BROWSER_METRO_UTILS_METRO_CHROME_WIN_H_

#include "chrome/browser/ui/host_desktop.h"

namespace chrome {

bool ActivateMetroChrome();

enum Win8Environment {
  WIN_8_ENVIRONMENT_METRO,
  WIN_8_ENVIRONMENT_DESKTOP,
  WIN_8_ENVIRONMENT_METRO_AURA,
  WIN_8_ENVIRONMENT_DESKTOP_AURA,
  WIN_8_ENVIRONMENT_MAX
};

Win8Environment GetWin8Environment(HostDesktopType desktop);

}  

#endif  
