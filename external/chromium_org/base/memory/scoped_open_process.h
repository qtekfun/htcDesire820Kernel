// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MEMORY_SCOPED_OPEN_PROCESS_H_
#define BASE_MEMORY_SCOPED_OPEN_PROCESS_H_

#include "base/process/process_handle.h"

namespace base {

class ScopedOpenProcess {
 public:
  ScopedOpenProcess() : handle_(kNullProcessHandle) {
  }

  
  ~ScopedOpenProcess() {
    Close();
  }

  
  
  bool Open(ProcessId pid) {
    Close();
    return OpenProcessHandle(pid, &handle_);
  }

  
  void Close() {
    if (handle_ == kNullProcessHandle)
      return;

    CloseProcessHandle(handle_);
    handle_ = kNullProcessHandle;
  }

  ProcessHandle handle() const { return handle_; }

 private:
  ProcessHandle handle_;
  DISALLOW_COPY_AND_ASSIGN(ScopedOpenProcess);
};
}  

#endif  
