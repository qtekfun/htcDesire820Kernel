// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_SHELL_DIALOGS_BASE_SHELL_DIALOG_H_
#define UI_SHELL_DIALOGS_BASE_SHELL_DIALOG_H_

#include "ui/gfx/native_widget_types.h"
#include "ui/shell_dialogs/shell_dialogs_export.h"

namespace ui {

class SHELL_DIALOGS_EXPORT BaseShellDialog {
 public:
  
  
  virtual bool IsRunning(gfx::NativeWindow owning_window) const = 0;

  
  
  virtual void ListenerDestroyed() = 0;

 protected:
  virtual ~BaseShellDialog();
};

}  

#endif  

