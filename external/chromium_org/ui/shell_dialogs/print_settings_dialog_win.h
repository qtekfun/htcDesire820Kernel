// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_SHELL_DIALOGS_PRINT_SETTINGS_DIALOG_WIN_H_
#define UI_SHELL_DIALOGS_PRINT_SETTINGS_DIALOG_WIN_H_

#include <ocidl.h>
#include <commdlg.h>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/message_loop/message_loop_proxy.h"
#include "ui/shell_dialogs/base_shell_dialog_win.h"
#include "ui/shell_dialogs/shell_dialogs_export.h"

namespace ui {

class SHELL_DIALOGS_EXPORT PrintSettingsDialogWin
    : public base::RefCountedThreadSafe<PrintSettingsDialogWin>,
      public BaseShellDialogImpl {
 public:
  class SHELL_DIALOGS_EXPORT Observer {
   public:
    virtual void PrintSettingsConfirmed(PRINTDLGEX* dialog_options) = 0;
    virtual void PrintSettingsCancelled(PRINTDLGEX* dialog_options) = 0;
  };
  typedef HRESULT(__stdcall* PrintDialogFunc)(PRINTDLGEX*);

  explicit PrintSettingsDialogWin(Observer* observer);
  virtual ~PrintSettingsDialogWin();

  
  
  
  
  void GetPrintSettings(
      PrintDialogFunc print_dialog_func,
      HWND owning_window,
      PRINTDLGEX* dialog_options);

 private:
  
  
  struct ExecutePrintSettingsParams {
    ExecutePrintSettingsParams(RunState run_state,
                               HWND owner,
                               PrintDialogFunc print_dialog_func,
                               PRINTDLGEX* dialog_options)
        : run_state(run_state),
          owner(owner),
          print_dialog_func(print_dialog_func),
          dialog_options(dialog_options),
          ui_proxy(base::MessageLoopForUI::current()->message_loop_proxy()) {}

    RunState run_state;
    HWND owner;
    PrintDialogFunc print_dialog_func;
    PRINTDLGEX* dialog_options;
    scoped_refptr<base::MessageLoopProxy> ui_proxy;
  };

  
  
  void ExecutePrintSettings(const ExecutePrintSettingsParams& params);

  
  
  void PrintSettingsCompleted(HRESULT hresult,
                              const ExecutePrintSettingsParams& params);

  
  Observer* observer_;

  DISALLOW_COPY_AND_ASSIGN(PrintSettingsDialogWin);
};

}  

#endif  
