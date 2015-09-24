// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_TABS_WINDOWS_UTIL_H__
#define CHROME_BROWSER_EXTENSIONS_API_TABS_WINDOWS_UTIL_H__

class ChromeAsyncExtensionFunction;

namespace extensions {
class WindowController;
}

namespace windows_util {

bool GetWindowFromWindowID(ChromeAsyncExtensionFunction* function,
                           int window_id,
                           extensions::WindowController** controller);

}  

#endif  
