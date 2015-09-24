// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_BROWSERTEST_UTIL_H_
#define CHROME_BROWSER_EXTENSIONS_BROWSERTEST_UTIL_H_

#include <string>

class Profile;

namespace extensions {
namespace browsertest_util {

std::string ExecuteScriptInBackgroundPage(Profile* profile,
                                          const std::string& extension_id,
                                          const std::string& script);

}  
}  

#endif  
