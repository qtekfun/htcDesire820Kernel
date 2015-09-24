
// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <string>

#include "base/base_api.h"
#include "base/string16.h"
#include "nativehelper/jni.h"

namespace android {

namespace jni { 

void BASE_API SetJavaVM(JavaVM* vm);

JNIEnv* GetJNIEnv();

std::string JstringToStdString(JNIEnv* env, jstring jstr);

string16 JstringToString16(JNIEnv* env, jstring jstr);

jstring ConvertUTF8ToJavaString(JNIEnv* env, std::string str);

bool CheckException(JNIEnv*);

void DetachFromVM();

} 

} 

