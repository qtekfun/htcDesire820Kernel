// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_ANDROID_JNI_ARRAY_H_
#define BASE_ANDROID_JNI_ARRAY_H_

#include <jni.h>
#include <string>
#include <vector>

#include "base/android/scoped_java_ref.h"
#include "base/basictypes.h"
#include "base/strings/string16.h"

namespace base {
namespace android {

BASE_EXPORT ScopedJavaLocalRef<jbyteArray> ToJavaByteArray(
    JNIEnv* env, const uint8* bytes, size_t len);

BASE_EXPORT ScopedJavaLocalRef<jintArray> ToJavaIntArray(
    JNIEnv* env, const int* ints, size_t len);

BASE_EXPORT ScopedJavaLocalRef<jintArray> ToJavaIntArray(
    JNIEnv* env, const std::vector<int>& ints);

BASE_EXPORT ScopedJavaLocalRef<jlongArray> ToJavaLongArray(
    JNIEnv* env, const int64* longs, size_t len);

BASE_EXPORT ScopedJavaLocalRef<jlongArray> ToJavaLongArray(
    JNIEnv* env, const std::vector<int64>& longs);

BASE_EXPORT ScopedJavaLocalRef<jobjectArray> ToJavaArrayOfByteArray(
    JNIEnv* env, const std::vector<std::string>& v);

BASE_EXPORT ScopedJavaLocalRef<jobjectArray> ToJavaArrayOfStrings(
    JNIEnv* env,  const std::vector<std::string>& v);

BASE_EXPORT ScopedJavaLocalRef<jobjectArray> ToJavaArrayOfStrings(
    JNIEnv* env,  const std::vector<string16>& v);

BASE_EXPORT void AppendJavaStringArrayToStringVector(
    JNIEnv* env,
    jobjectArray array,
    std::vector<string16>* out);

BASE_EXPORT void AppendJavaStringArrayToStringVector(
    JNIEnv* env,
    jobjectArray array,
    std::vector<std::string>* out);

BASE_EXPORT void AppendJavaByteArrayToByteVector(
    JNIEnv* env,
    jbyteArray byte_array,
    std::vector<uint8>* out);

BASE_EXPORT void JavaByteArrayToByteVector(
    JNIEnv* env,
    jbyteArray byte_array,
    std::vector<uint8>* out);

BASE_EXPORT void JavaIntArrayToIntVector(
    JNIEnv* env,
    jintArray int_array,
    std::vector<int>* out);

BASE_EXPORT void JavaFloatArrayToFloatVector(
    JNIEnv* env,
    jfloatArray float_array,
    std::vector<float>* out);

BASE_EXPORT void JavaArrayOfByteArrayToStringVector(
    JNIEnv* env,
    jobjectArray array,
    std::vector<std::string>* out);

}  
}  

#endif  
