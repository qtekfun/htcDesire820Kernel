// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TEST_PYTHON_UTILS_H_
#define NET_TEST_PYTHON_UTILS_H_
#pragma once

#include "base/compiler_specific.h"

class FilePath;

extern const char kPythonPathEnv[];

void AppendToPythonPath(const FilePath& dir);

bool GetPyProtoPath(FilePath* dir);

bool GetPythonRunTime(FilePath* path) WARN_UNUSED_RESULT;

#endif  
