// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TEST_PYTHON_UTILS_H_
#define NET_TEST_PYTHON_UTILS_H_

#include "base/compiler_specific.h"

class CommandLine;

namespace base {
class FilePath;
}

extern const char kPythonPathEnv[];

void AppendToPythonPath(const base::FilePath& dir);

bool GetPyProtoPath(base::FilePath* dir);

bool GetPythonCommand(CommandLine* python_cmd) WARN_UNUSED_RESULT;

#endif  
