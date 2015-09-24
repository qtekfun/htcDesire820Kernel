// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_CHROMEDRIVER_UTIL_H_
#define CHROME_TEST_CHROMEDRIVER_UTIL_H_

#include <string>

namespace base {
class FilePath;
class ListValue;
}

class Status;
class WebView;

std::string GenerateId();

Status SendKeysOnWindow(
    WebView* web_view,
    const base::ListValue* key_list,
    bool release_modifiers,
    int* sticky_modifiers);

bool Base64Decode(const std::string& base64, std::string* bytes);

Status UnzipSoleFile(const base::FilePath& unzip_dir,
                     const std::string& bytes,
                     base::FilePath* file);

#endif  
