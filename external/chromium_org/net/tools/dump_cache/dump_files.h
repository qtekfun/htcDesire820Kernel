// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_DUMP_CACHE_DUMP_FILES_H_
#define NET_TOOLS_DUMP_CACHE_DUMP_FILES_H_


#include "base/files/file_path.h"

int GetMajorVersion(const base::FilePath& input_path);

int DumpContents(const base::FilePath& input_path);

int DumpHeaders(const base::FilePath& input_path);

#endif  
