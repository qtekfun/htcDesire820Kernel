// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_SHELL_DIALOGS_SELECT_FILE_DIALOG_FACTORY_H_
#define UI_SHELL_DIALOGS_SELECT_FILE_DIALOG_FACTORY_H_

#include "ui/shell_dialogs/select_file_dialog.h"
#include "ui/shell_dialogs/shell_dialogs_export.h"

namespace ui {
class SelectFilePolicy;

class SHELL_DIALOGS_EXPORT SelectFileDialogFactory {
 public:
  virtual ~SelectFileDialogFactory();

  virtual SelectFileDialog* Create(ui::SelectFileDialog::Listener* listener,
                                   ui::SelectFilePolicy* policy) = 0;
};

}  

#endif  

