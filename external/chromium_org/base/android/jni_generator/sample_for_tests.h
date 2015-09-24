// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <jni.h>
#include <map>
#include <string>

#include "base/basictypes.h"

namespace base {
namespace android {

class CPPClass {
 public:
  CPPClass();
  ~CPPClass();

  class InnerClass {
   public:
    jdouble MethodOtherP0(JNIEnv* env, jobject obj);
  };

  void Destroy(JNIEnv* env, jobject obj);

  jint Method(JNIEnv* env, jobject obj);

  void AddStructB(JNIEnv* env, jobject obj, jobject structb);

  void IterateAndDoSomethingWithStructB(JNIEnv* env, jobject obj);

 private:
  std::map<long, std::string> map_;

  DISALLOW_COPY_AND_ASSIGN(CPPClass);
};

}  
}  
