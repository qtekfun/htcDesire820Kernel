// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SHELL_COMMON_WEBKIT_TEST_HELPERS_H_
#define CONTENT_SHELL_COMMON_WEBKIT_TEST_HELPERS_H_

struct WebPreferences;

namespace WebTestRunner {
struct WebPreferences;
}

namespace base {
class FilePath;
}

struct WebPreferences;

namespace content {

void ExportLayoutTestSpecificPreferences(
    const WebTestRunner::WebPreferences& from, WebPreferences* to);

void ApplyLayoutTestDefaultPreferences(WebPreferences* prefs);

base::FilePath GetWebKitRootDirFilePath();

}  

#endif  
