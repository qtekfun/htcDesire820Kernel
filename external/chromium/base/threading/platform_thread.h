// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_THREADING_PLATFORM_THREAD_H_
#define BASE_THREADING_PLATFORM_THREAD_H_
#pragma once

#include "base/base_api.h"
#include "base/basictypes.h"
#include "build/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#elif defined(OS_POSIX)
#include <pthread.h>
#if defined(OS_MACOSX)
#include <mach/mach.h>
#else  
#include <unistd.h>
#endif
#endif

namespace base {

#if defined(OS_WIN)
typedef DWORD PlatformThreadId;
typedef void* PlatformThreadHandle;  
const PlatformThreadHandle kNullThreadHandle = NULL;
#elif defined(OS_POSIX)
typedef pthread_t PlatformThreadHandle;
const PlatformThreadHandle kNullThreadHandle = 0;
#if defined(OS_MACOSX)
typedef mach_port_t PlatformThreadId;
#else  
typedef pid_t PlatformThreadId;
#endif
#endif

const PlatformThreadId kInvalidThreadId = 0;

class BASE_API PlatformThread {
 public:
  
  
  class BASE_API Delegate {
   public:
    virtual ~Delegate() {}
    virtual void ThreadMain() = 0;
  };

  
  static PlatformThreadId CurrentId();

  
  static void YieldCurrentThread();

  
  static void Sleep(int duration_ms);

  
  static void SetName(const char* name);

  
  
  
  
  
  
  
  
  static bool Create(size_t stack_size, Delegate* delegate,
                     PlatformThreadHandle* thread_handle);

  
  
  
  static bool CreateNonJoinable(size_t stack_size, Delegate* delegate);

  
  
  
  static void Join(PlatformThreadHandle thread_handle);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(PlatformThread);
};

}  

#endif  
