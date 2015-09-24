// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_NAVIGATION_INTERCEPTION_COMPONENT_JNI_REGISTRAR_H_
#define COMPONENTS_NAVIGATION_INTERCEPTION_COMPONENT_JNI_REGISTRAR_H_

#include <jni.h>

namespace navigation_interception {

bool RegisterNavigationInterceptionJni(JNIEnv* env);

}  

#endif  
