// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_UTILITY_THREADING_SIMPLE_THREAD_H_
#define PPAPI_UTILITY_THREADING_SIMPLE_THREAD_H_

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "ppapi/cpp/instance_handle.h"
#include "ppapi/cpp/message_loop.h"

namespace pp {

class SimpleThread {
 public:
#ifdef WIN32
  typedef HANDLE ThreadHandle;
#else
  typedef pthread_t ThreadHandle;
#endif

  typedef void (*ThreadFunc)(MessageLoop&, void* user_data);

  explicit SimpleThread(const InstanceHandle& instance);
  explicit SimpleThread(const InstanceHandle& instance, size_t stacksize);
  ~SimpleThread();

  
  
  
  bool Start();

  
  
  
  bool Join();

  
  
  
  bool StartWithFunction(ThreadFunc func, void* user_data);

  MessageLoop& message_loop() { return message_loop_; }
  ThreadHandle thread() const { return thread_; }

 private:
  InstanceHandle instance_;
  MessageLoop message_loop_;
  const size_t stacksize_;
  ThreadHandle thread_;

  
  SimpleThread(const SimpleThread&);
  SimpleThread(const SimpleThread&, size_t stacksize);
  SimpleThread& operator=(const SimpleThread&);
};

}  

#endif  

