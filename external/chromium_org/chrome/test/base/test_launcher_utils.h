// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_BASE_TEST_LAUNCHER_UTILS_H_
#define CHROME_TEST_BASE_TEST_LAUNCHER_UTILS_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/files/file_path.h"

class CommandLine;

namespace test_launcher_utils {

void PrepareBrowserCommandLineForTests(CommandLine* command_line);

bool OverrideUserDataDir(
    const base::FilePath& user_data_dir) WARN_UNUSED_RESULT;

}  

#endif  
