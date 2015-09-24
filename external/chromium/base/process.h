// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_PROCESS_H_
#define BASE_PROCESS_H_
#pragma once

#include "base/base_api.h"
#include "base/basictypes.h"
#include "build/build_config.h"

#include <sys/types.h>
#if defined(OS_WIN)
#include <windows.h>
#endif

namespace base {

#if defined(OS_WIN)
typedef HANDLE ProcessHandle;
typedef DWORD ProcessId;
typedef HANDLE UserTokenHandle;
const ProcessHandle kNullProcessHandle = NULL;
const ProcessId kNullProcessId = 0;
#elif defined(OS_POSIX)
typedef pid_t ProcessHandle;
typedef pid_t ProcessId;
const ProcessHandle kNullProcessHandle = 0;
const ProcessId kNullProcessId = 0;
#endif  

#if defined(OS_POSIX) && !defined(OS_MACOSX)
const int kUnsetProcessPriority = 256;
#endif

class BASE_API Process {
 public:
  Process() : process_(kNullProcessHandle) {
#if defined(OS_POSIX) && !defined(OS_MACOSX)
    saved_priority_ = kUnsetProcessPriority;
#endif
  }

  explicit Process(ProcessHandle handle) : process_(handle) {
#if defined(OS_POSIX) && !defined(OS_MACOSX)
    saved_priority_ = kUnsetProcessPriority;
#endif
  }

  
  static Process Current();

  
  
  ProcessHandle handle() const { return process_; }
  void set_handle(ProcessHandle handle) {
    process_ = handle;
#if defined(OS_POSIX) && !defined(OS_MACOSX)
    saved_priority_ = kUnsetProcessPriority;
#endif
  }

  
  ProcessId pid() const;

  
  bool is_current() const;

  
  void Close();

  
  
  
  void Terminate(int result_code);

  
  
  bool IsProcessBackgrounded() const;

  
  
  
  
  
  bool SetProcessBackgrounded(bool value);

  
  
  int GetPriority() const;

 private:
  ProcessHandle process_;
#if defined(OS_POSIX) && !defined(OS_MACOSX)
  
  
  int saved_priority_;
#endif
};

}  

#endif  
