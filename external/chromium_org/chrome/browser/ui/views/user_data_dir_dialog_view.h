// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_USER_DATA_DIR_DIALOG_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_USER_DATA_DIR_DIALOG_VIEW_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/message_loop/message_loop.h"
#include "ui/shell_dialogs/select_file_dialog.h"
#include "ui/views/window/dialog_delegate.h"

namespace views {
class MessageBoxView;
}

class UserDataDirDialogView : public views::DialogDelegate,
                              public base::MessageLoopForUI::Dispatcher,
                              public ui::SelectFileDialog::Listener {
 public:
  explicit UserDataDirDialogView(const base::FilePath& user_data_dir);
  virtual ~UserDataDirDialogView();

  base::FilePath user_data_dir() const { return user_data_dir_; }

  
  virtual base::string16 GetDialogButtonLabel(
      ui::DialogButton button) const OVERRIDE;
  virtual base::string16 GetWindowTitle() const OVERRIDE;
  virtual void DeleteDelegate() OVERRIDE;
  virtual bool Accept() OVERRIDE;
  virtual bool Cancel() OVERRIDE;

  
  virtual views::View* GetContentsView() OVERRIDE;
  virtual views::Widget* GetWidget() OVERRIDE;
  virtual const views::Widget* GetWidget() const OVERRIDE;

  
  virtual bool Dispatch(const base::NativeEvent& msg) OVERRIDE;

  
  virtual void FileSelected(const base::FilePath& path,
                            int index,
                            void* params) OVERRIDE;
  virtual void FileSelectionCanceled(void* params) OVERRIDE;

 private:
  
  base::FilePath user_data_dir_;

  views::MessageBoxView* message_box_view_;
  scoped_refptr<ui::SelectFileDialog> select_file_dialog_;

  
  
  bool is_blocking_;

  DISALLOW_COPY_AND_ASSIGN(UserDataDirDialogView);
};

#endif  
