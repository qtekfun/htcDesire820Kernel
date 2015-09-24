// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_SHELL_DIALOGS_LINUX_SHELL_DIALOG_H_
#define UI_SHELL_DIALOGS_LINUX_SHELL_DIALOG_H_

#include "ui/shell_dialogs/select_file_dialog.h"
#include "ui/shell_dialogs/shell_dialogs_export.h"

namespace ui {

class SHELL_DIALOGS_EXPORT LinuxShellDialog {
 public:
  virtual ~LinuxShellDialog() {}

  
  
  
  static void SetInstance(LinuxShellDialog* instance);

  
  
  
  
  
  static const LinuxShellDialog* instance();

  
  virtual SelectFileDialog* CreateSelectFileDialog(
      SelectFileDialog::Listener* listener,
      SelectFilePolicy* policy) const = 0;
};

}  

#endif  

