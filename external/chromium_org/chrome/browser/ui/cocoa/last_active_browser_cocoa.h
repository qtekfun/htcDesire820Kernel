// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_LAST_ACTIVE_BROWSER_COCOA_H_
#define CHROME_BROWSER_UI_COCOA_LAST_ACTIVE_BROWSER_COCOA_H_

#include "build/build_config.h"

#if !defined(OS_MACOSX)
#error This file is intended for use in the Cocoa frontend only.
#endif

class Browser;

namespace chrome {

Browser* GetLastActiveBrowser();

}  

#endif  
