// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SHELL_APP_PATHS_MAC_H_
#define CONTENT_SHELL_APP_PATHS_MAC_H_

namespace base {
class FilePath;
}

void OverrideFrameworkBundlePath();

void OverrideChildProcessPath();

base::FilePath GetResourcesPakFilePath();

base::FilePath GetInfoPlistPath();

#endif  
