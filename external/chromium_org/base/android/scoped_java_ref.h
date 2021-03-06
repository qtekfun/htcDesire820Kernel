// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_ANDROID_SCOPED_JAVA_REF_H_
#define BASE_ANDROID_SCOPED_JAVA_REF_H_

#include <jni.h>
#include <stddef.h>

#include "base/base_export.h"
#include "base/basictypes.h"

namespace base {
namespace android {

template<typename T> class JavaRef;

template<>
class BASE_EXPORT JavaRef<jobject> {
 public:
  jobject obj() const { return obj_; }

  bool is_null() const { return obj_ == NULL; }

 protected:
  
  JavaRef();

  
  
  JavaRef(JNIEnv* env, jobject obj);

  ~JavaRef();

  
  
  JNIEnv* SetNewLocalRef(JNIEnv* env, jobject obj);
  void SetNewGlobalRef(JNIEnv* env, jobject obj);
  void ResetLocalRef(JNIEnv* env);
  void ResetGlobalRef();
  jobject ReleaseInternal();

 private:
  jobject obj_;

  DISALLOW_COPY_AND_ASSIGN(JavaRef);
};

template<typename T>
class JavaRef : public JavaRef<jobject> {
 public:
  T obj() const { return static_cast<T>(JavaRef<jobject>::obj()); }

 protected:
  JavaRef() {}
  ~JavaRef() {}

  JavaRef(JNIEnv* env, T obj) : JavaRef<jobject>(env, obj) {}

 private:
  DISALLOW_COPY_AND_ASSIGN(JavaRef);
};

template<typename T>
class ScopedJavaLocalRef : public JavaRef<T> {
 public:
  ScopedJavaLocalRef() : env_(NULL) {}

  
  
  ScopedJavaLocalRef(const ScopedJavaLocalRef<T>& other)
      : env_(other.env_) {
    this->SetNewLocalRef(env_, other.obj());
  }

  template<typename U>
  explicit ScopedJavaLocalRef(const U& other)
      : env_(NULL) {
    this->Reset(other);
  }

  
  
  ScopedJavaLocalRef(JNIEnv* env, T obj) : JavaRef<T>(env, obj), env_(env) {}

  ~ScopedJavaLocalRef() {
    this->Reset();
  }

  
  
  void operator=(const ScopedJavaLocalRef<T>& other) {
    this->Reset(other);
  }

  void Reset() {
    this->ResetLocalRef(env_);
  }

  template<typename U>
  void Reset(const ScopedJavaLocalRef<U>& other) {
    
    
    
    this->Reset(other.env_, other.obj());
  }

  template<typename U>
  void Reset(const U& other) {
    
    
    this->Reset(env_, other.obj());
  }

  template<typename U>
  void Reset(JNIEnv* env, U obj) {
    implicit_cast<T>(obj);  
    env_ = this->SetNewLocalRef(env, obj);
  }

  
  
  T Release() {
    return static_cast<T>(this->ReleaseInternal());
  }

 private:
  
  
  JNIEnv* env_;
};

template<typename T>
class ScopedJavaGlobalRef : public JavaRef<T> {
 public:
  ScopedJavaGlobalRef() {}

  explicit ScopedJavaGlobalRef(const ScopedJavaGlobalRef<T>& other) {
    this->Reset(other);
  }

  template<typename U>
  explicit ScopedJavaGlobalRef(const U& other) {
    this->Reset(other);
  }

  ~ScopedJavaGlobalRef() {
    this->Reset();
  }

  void Reset() {
    this->ResetGlobalRef();
  }

  template<typename U>
  void Reset(const U& other) {
    this->Reset(NULL, other.obj());
  }

  template<typename U>
  void Reset(JNIEnv* env, U obj) {
    implicit_cast<T>(obj);  
    this->SetNewGlobalRef(env, obj);
  }

  
  
  T Release() {
    return static_cast<T>(this->ReleaseInternal());
  }
};

}  
}  

#endif  
