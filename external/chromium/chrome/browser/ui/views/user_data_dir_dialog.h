// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_USER_DATA_DIR_DIALOG_H_
#define CHROME_BROWSER_UI_VIEWS_USER_DATA_DIR_DIALOG_H_
#pragma once

#include "base/basictypes.h"
#include "base/message_loop.h"
#include "chrome/browser/ui/shell_dialogs.h"
#include "views/window/dialog_delegate.h"

class FilePath;

namespace views {
class MessageBoxView;
class Window;
}

class UserDataDirDialog : public views::DialogDelegate,
                          public MessageLoopForUI::Dispatcher,
                          public SelectFileDialog::Listener {
 public:
  
  
  
  
  static FilePath RunUserDataDirDialog(const FilePath& user_data_dir);
  virtual ~UserDataDirDialog();

  FilePath user_data_dir() const { return user_data_dir_; }

  
  virtual std::wstring GetDialogButtonLabel(
      MessageBoxFlags::DialogButton button) const;
  virtual std::wstring GetWindowTitle() const;
  virtual void DeleteDelegate();
  virtual bool Accept();
  virtual bool Cancel();

  
  virtual bool IsAlwaysOnTop() const { return false; }
  virtual bool IsModal() const { return false; }
  virtual views::View* GetContentsView();

  
  virtual bool Dispatch(const MSG& msg);

  
  virtual void FileSelected(const FilePath& path, int index, void* params);
  virtual void FileSelectionCanceled(void* params);

 private:
  explicit UserDataDirDialog(const FilePath& user_data_dir);

  
  FilePath user_data_dir_;

  views::MessageBoxView* message_box_view_;
  scoped_refptr<SelectFileDialog> select_file_dialog_;

  
  
  bool is_blocking_;

  DISALLOW_COPY_AND_ASSIGN(UserDataDirDialog);
};

#endif  
