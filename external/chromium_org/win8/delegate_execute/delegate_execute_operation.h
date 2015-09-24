// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WIN8_DELEGATE_EXECUTE_DELEGATE_EXECUTE_OPERATION_H_
#define WIN8_DELEGATE_EXECUTE_DELEGATE_EXECUTE_OPERATION_H_

#include <windows.h>
#include <atldef.h>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/strings/string16.h"

class CommandLine;

namespace delegate_execute {

class DelegateExecuteOperation {
 public:
  enum OperationType {
    DELEGATE_EXECUTE,
    RELAUNCH_CHROME,
  };

  DelegateExecuteOperation();
  ~DelegateExecuteOperation();

  bool Init(const CommandLine* cmd_line);

  OperationType operation_type() const {
    return operation_type_;
  }

  const string16& relaunch_flags() const {
    return relaunch_flags_;
  }

  const string16& mutex() const {
    return mutex_;
  }

  
  DWORD GetParentPid() const;

  const base::FilePath& shortcut() const {
    return relaunch_shortcut_;
  }

 private:
  OperationType operation_type_;
  string16 relaunch_flags_;
  base::FilePath relaunch_shortcut_;
  string16 mutex_;

  DISALLOW_COPY_AND_ASSIGN(DelegateExecuteOperation);
};

}  

#endif  
