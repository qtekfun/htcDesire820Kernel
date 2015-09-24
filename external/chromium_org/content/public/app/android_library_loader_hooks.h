// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_APP_ANDROID_LIBRARY_LOADER_HOOKS_H_
#define CONTENT_PUBLIC_APP_ANDROID_LIBRARY_LOADER_HOOKS_H_

#include <jni.h>

#include "base/basictypes.h"
#include "content/common/content_export.h"

namespace content {

CONTENT_EXPORT bool RegisterLibraryLoaderEntryHook(JNIEnv* env);

CONTENT_EXPORT bool EnsureJniRegistered(JNIEnv* env);

CONTENT_EXPORT void SetVersionNumber(const char* version_number);

CONTENT_EXPORT void LibraryLoaderExitHook();

}  

#endif  
