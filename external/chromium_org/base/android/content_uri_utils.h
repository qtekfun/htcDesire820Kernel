// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_ANDROID_CONTENT_URI_UTILS_H_
#define BASE_ANDROID_CONTENT_URI_UTILS_H_

#include <jni.h>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/files/file_path.h"

namespace base {

bool RegisterContentUriUtils(JNIEnv* env);

BASE_EXPORT int OpenContentUriForRead(const FilePath& content_uri);

BASE_EXPORT bool ContentUriExists(const FilePath& content_uri);

}  

#endif  
