// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_ANDROID_BUILD_INFO_H_
#define BASE_ANDROID_BUILD_INFO_H_

#include <jni.h>

#include <string>

#include "base/base_export.h"
#include "base/memory/singleton.h"

namespace base {
namespace android {


class BASE_EXPORT BuildInfo {
 public:

  ~BuildInfo() {}

  
  
  
  
  static BuildInfo* GetInstance();

  
  
  
  
  const char* device() const {
    return device_;
  }

  const char* model() const {
    return model_;
  }

  const char* brand() const {
    return brand_;
  }

  const char* android_build_id() const {
    return android_build_id_;
  }

  const char* android_build_fp() const {
    return android_build_fp_;
  }

  const char* package_version_code() const {
    return package_version_code_;
  }

  const char* package_version_name() const {
    return package_version_name_;
  }

  const char* package_label() const {
    return package_label_;
  }

  const char* package_name() const {
    return package_name_;
  }

  int sdk_int() const {
    return sdk_int_;
  }

  const char* java_exception_info() const {
    return java_exception_info_;
  }

  void set_java_exception_info(const std::string& info);

  static bool RegisterBindings(JNIEnv* env);

 private:
  friend struct BuildInfoSingletonTraits;

  explicit BuildInfo(JNIEnv* env);

  
  
  
  
  const char* const device_;
  const char* const model_;
  const char* const brand_;
  const char* const android_build_id_;
  const char* const android_build_fp_;
  const char* const package_version_code_;
  const char* const package_version_name_;
  const char* const package_label_;
  const char* const package_name_;
  const int sdk_int_;
  
  const char* java_exception_info_;

  DISALLOW_COPY_AND_ASSIGN(BuildInfo);
};

}  
}  

#endif  
