// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SHELL_BROWSER_SHELL_LAYOUT_TESTS_ANDROID_H_
#define CONTENT_SHELL_BROWSER_SHELL_LAYOUT_TESTS_ANDROID_H_

#include <string>

class GURL;

namespace content {

bool GetTestUrlForAndroid(std::string& path_or_url, GURL* url);

void EnsureInitializeForAndroidLayoutTests();

}  

#endif  
