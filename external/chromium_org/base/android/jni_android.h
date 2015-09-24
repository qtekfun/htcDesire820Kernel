// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_ANDROID_JNI_ANDROID_H_
#define BASE_ANDROID_JNI_ANDROID_H_

#include <jni.h>
#include <sys/types.h>

#include "base/android/scoped_java_ref.h"
#include "base/atomicops.h"
#include "base/base_export.h"
#include "base/compiler_specific.h"

namespace base {
namespace android {

#define JNI_EXPORT __attribute__ ((visibility("default")))

struct RegistrationMethod {
  const char* name;
  bool (*func)(JNIEnv* env);
};

BASE_EXPORT JNIEnv* AttachCurrentThread();

BASE_EXPORT void DetachFromVM();

BASE_EXPORT void InitVM(JavaVM* vm);

BASE_EXPORT bool IsVMInitialized();

BASE_EXPORT void InitApplicationContext(JNIEnv* env,
                                        const JavaRef<jobject>& context);

const BASE_EXPORT jobject GetApplicationContext();

BASE_EXPORT ScopedJavaLocalRef<jclass> GetClass(JNIEnv* env,
                                                const char* class_name);

class BASE_EXPORT MethodID {
 public:
  enum Type {
    TYPE_STATIC,
    TYPE_INSTANCE,
  };

  
  
  template<Type type>
  static jmethodID Get(JNIEnv* env,
                       jclass clazz,
                       const char* method_name,
                       const char* jni_signature);

  
  
  
  template<Type type>
  static jmethodID LazyGet(JNIEnv* env,
                           jclass clazz,
                           const char* method_name,
                           const char* jni_signature,
                           base::subtle::AtomicWord* atomic_method_id);
};

BASE_EXPORT bool HasException(JNIEnv* env);

BASE_EXPORT bool ClearException(JNIEnv* env);

BASE_EXPORT void CheckException(JNIEnv* env);

}  
}  

#endif  
