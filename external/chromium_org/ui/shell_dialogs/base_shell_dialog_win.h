// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_SHELL_DIALOGS_BASE_SHELL_DIALOG_WIN_H_
#define UI_SHELL_DIALOGS_BASE_SHELL_DIALOG_WIN_H_

#include <shlobj.h>
#include <set>

#include "ui/shell_dialogs/base_shell_dialog.h"
#include "ui/shell_dialogs/shell_dialogs_export.h"

namespace base {
class Thread;
}

namespace ui {

class SHELL_DIALOGS_EXPORT BaseShellDialogImpl {
 public:
  BaseShellDialogImpl();
  virtual ~BaseShellDialogImpl();

 protected:
  
  struct RunState {
    
    HWND owner;

    
    base::Thread* dialog_thread;
  };

  
  
  
  RunState BeginRun(HWND owner);

  
  
  
  
  
  
  void EndRun(RunState run_state);

  
  
  bool IsRunningDialogForOwner(HWND owner) const;

  
  
  
  
  
  
  
  
  void DisableOwner(HWND owner);

 private:
  typedef std::set<HWND> Owners;

  
  
  
  
  static base::Thread* CreateDialogThread();

  
  void EnableOwner(HWND owner);

  
  
  
  
  
  
  
  
  
  
  
  
  static Owners owners_;
  static int instance_count_;

  DISALLOW_COPY_AND_ASSIGN(BaseShellDialogImpl);
};

}  

#endif  

