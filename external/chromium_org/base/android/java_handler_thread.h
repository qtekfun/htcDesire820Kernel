// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_THREADING_JAVA_THREAD_H_
#define BASE_THREADING_JAVA_THREAD_H_

#include <jni.h>

#include "base/android/scoped_java_ref.h"
#include "base/memory/scoped_ptr.h"

namespace base {

class MessageLoop;
class WaitableEvent;

namespace android {

class BASE_EXPORT JavaHandlerThread {
 public:
  JavaHandlerThread(const char* name);
  virtual ~JavaHandlerThread();

  base::MessageLoop* message_loop() const { return message_loop_.get(); }
  void Start();
  void Stop();

  
  
  void InitializeThread(JNIEnv* env, jobject obj, jint event);

  static bool RegisterBindings(JNIEnv* env);

 private:
  scoped_ptr<base::MessageLoop> message_loop_;
  ScopedJavaGlobalRef<jobject> java_thread_;
};

}  
}  

#endif  
