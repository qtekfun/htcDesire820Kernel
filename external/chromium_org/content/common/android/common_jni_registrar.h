// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_ANDROID_COMMON_JNI_REGISTRAR_H_
#define CONTENT_COMMON_ANDROID_COMMON_JNI_REGISTRAR_H_

#include <jni.h>

#include "content/common/content_export.h"

namespace content {
namespace android {

CONTENT_EXPORT bool RegisterCommonJni(JNIEnv* env);

}  
}  

#endif  
