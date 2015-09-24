// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_PROCESS_PROCESS_PROCESS_H_
#define BASE_PROCESS_PROCESS_PROCESS_H_

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/process/process_handle.h"
#include "build/build_config.h"

namespace base {

class BASE_EXPORT Process {
 public:
  Process() : process_(kNullProcessHandle) {
  }

  explicit Process(ProcessHandle handle) : process_(handle) {
  }

  
  static Process Current();

  static bool CanBackgroundProcesses();

  
  
  ProcessHandle handle() const { return process_; }
  void set_handle(ProcessHandle handle) {
    process_ = handle;
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
};

}  

#endif  
