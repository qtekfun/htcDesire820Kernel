// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_CHROME_PATHS_INTERNAL_H_
#define CHROME_COMMON_CHROME_PATHS_INTERNAL_H_
#pragma once

#include "build/build_config.h"

class FilePath;

namespace chrome {

bool GetDefaultUserDataDirectory(FilePath* result);

bool GetChromeFrameUserDataDirectory(FilePath* result);

void GetUserCacheDirectory(const FilePath& profile_dir, FilePath* result);

bool GetUserDocumentsDirectory(FilePath* result);

#if defined (OS_WIN)
bool GetUserDownloadsDirectorySafe(FilePath* result);
#endif

bool GetUserDownloadsDirectory(FilePath* result);

bool GetUserDesktop(FilePath* result);

#if defined(OS_MACOSX)
FilePath GetVersionedDirectory();

void SetOverrideVersionedDirectory(const FilePath* path);

FilePath GetFrameworkBundlePath();

bool GetLocalLibraryDirectory(FilePath* result);

#endif  

}  

#endif  
