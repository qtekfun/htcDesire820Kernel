// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_SHELL_DIALOGS_SELECT_FILE_POLICY_H_
#define UI_SHELL_DIALOGS_SELECT_FILE_POLICY_H_

#include "ui/shell_dialogs/shell_dialogs_export.h"

namespace ui {

class SHELL_DIALOGS_EXPORT SelectFilePolicy {
 public:
  virtual ~SelectFilePolicy();

  
  virtual bool CanOpenSelectFileDialog() = 0;

  
  virtual void SelectFileDenied() = 0;
};

}  

#endif  

