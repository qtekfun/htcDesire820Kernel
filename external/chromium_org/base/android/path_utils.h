// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_ANDROID_PATH_UTILS_H_
#define BASE_ANDROID_PATH_UTILS_H_

#include <jni.h>

#include "base/base_export.h"

namespace base {

class FilePath;

namespace android {

BASE_EXPORT bool GetDataDirectory(FilePath* result);

BASE_EXPORT bool GetDatabaseDirectory(FilePath* result);

BASE_EXPORT bool GetCacheDirectory(FilePath* result);

BASE_EXPORT bool GetDownloadsDirectory(FilePath* result);

BASE_EXPORT bool GetNativeLibraryDirectory(FilePath* result);

BASE_EXPORT bool GetExternalStorageDirectory(FilePath* result);

bool RegisterPathUtils(JNIEnv* env);

}  
}  

#endif  
