// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SHELL_ANDROID_SHELL_MANAGER_H_
#define CONTENT_SHELL_ANDROID_SHELL_MANAGER_H_

#include <jni.h>

#include "base/android/jni_android.h"
#include "base/android/scoped_java_ref.h"

class Shell;

namespace blink {
class WebLayer;
}

namespace content {

jobject CreateShellView(Shell* shell);

void CloseShellView(jobject shell_view);

bool RegisterShellManager(JNIEnv* env);

void ShellAttachLayer(blink::WebLayer* layer);
void ShellRemoveLayer(blink::WebLayer* layer);
}  

#endif  
