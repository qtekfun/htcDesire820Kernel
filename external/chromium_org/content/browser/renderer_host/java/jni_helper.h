// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_JAVA_JNI_HELPER_H_
#define CONTENT_BROWSER_RENDERER_HOST_JAVA_JNI_HELPER_H_

#include <jni.h>

#include "content/common/content_export.h"

namespace content {

CONTENT_EXPORT jmethodID GetMethodIDFromClassName(JNIEnv* env,
                                                  const char* class_name,
                                                  const char* method,
                                                  const char* jni_signature);

}  

#endif  
