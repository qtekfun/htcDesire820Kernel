// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_CHROME_PATHS_INTERNAL_H_
#define CHROME_COMMON_CHROME_PATHS_INTERNAL_H_

#include <string>

#include "build/build_config.h"

#if defined(OS_MACOSX)
#if defined(__OBJC__)
@class NSBundle;
#else
class NSBundle;
#endif
#endif

namespace base {
class FilePath;
}

namespace chrome {

bool GetDefaultUserDataDirectory(base::FilePath* result);

bool GetChromeFrameUserDataDirectory(base::FilePath* result);

void GetUserCacheDirectory(const base::FilePath& profile_dir, base::FilePath* result);

bool GetUserDocumentsDirectory(base::FilePath* result);

#if defined(OS_WIN) || defined(OS_LINUX)
bool GetUserDownloadsDirectorySafe(base::FilePath* result);
#endif

bool GetUserDownloadsDirectory(base::FilePath* result);

bool GetUserMusicDirectory(base::FilePath* result);

bool GetUserPicturesDirectory(base::FilePath* result);

bool GetUserVideosDirectory(base::FilePath* result);

#if defined(OS_MACOSX) && !defined(OS_IOS)
base::FilePath GetVersionedDirectory();

void SetOverrideVersionedDirectory(const base::FilePath* path);

base::FilePath GetFrameworkBundlePath();

bool GetLocalLibraryDirectory(base::FilePath* result);

bool GetGlobalApplicationSupportDirectory(base::FilePath* result);

NSBundle* OuterAppBundle();

bool GetUserDataDirectoryForBrowserBundle(NSBundle* bundle,
                                          base::FilePath* result);

#endif  

bool ProcessNeedsProfileDir(const std::string& process_type);

}  

#endif  
