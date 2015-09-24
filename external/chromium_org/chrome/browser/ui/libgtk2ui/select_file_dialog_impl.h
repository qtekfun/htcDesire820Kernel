// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_LIBGTK2UI_SELECT_FILE_DIALOG_IMPL_H_
#define CHROME_BROWSER_UI_LIBGTK2UI_SELECT_FILE_DIALOG_IMPL_H_

#include <set>

#include "base/compiler_specific.h"
#include "base/nix/xdg_util.h"
#include "ui/aura/window.h"
#include "ui/shell_dialogs/select_file_dialog.h"
#include "ui/shell_dialogs/select_file_policy.h"

namespace libgtk2ui {

class SelectFileDialogImpl : public ui::SelectFileDialog {
 public:
  
  static ui::SelectFileDialog* Create(Listener* listener,
                                      ui::SelectFilePolicy* policy);

  
  static SelectFileDialogImpl* NewSelectFileDialogImplGTK(
      Listener* listener,
      ui::SelectFilePolicy* policy);
  
  static SelectFileDialogImpl* NewSelectFileDialogImplKDE(
      Listener* listener,
      ui::SelectFilePolicy* policy,
      base::nix::DesktopEnvironment desktop);

  
  
  static bool CheckKDEDialogWorksOnUIThread();

  
  virtual bool IsRunning(gfx::NativeWindow parent_window) const OVERRIDE;
  virtual void ListenerDestroyed() OVERRIDE;

 protected:
  explicit SelectFileDialogImpl(Listener* listener,
                                ui::SelectFilePolicy* policy);
  virtual ~SelectFileDialogImpl();

  
  
  virtual void SelectFileImpl(
      Type type,
      const base::string16& title,
      const base::FilePath& default_path,
      const FileTypeInfo* file_types,
      int file_type_index,
      const base::FilePath::StringType& default_extension,
      gfx::NativeWindow owning_window,
      void* params) = 0;

  
  
  
  
  bool CallDirectoryExistsOnUIThread(const base::FilePath& path);

  
  FileTypeInfo file_types_;

  
  
  size_t file_type_index_;

  
  std::set<aura::Window*> parents_;

  
  Type type_;

  
  
  static base::FilePath* last_saved_path_;
  static base::FilePath* last_opened_path_;

  DISALLOW_COPY_AND_ASSIGN(SelectFileDialogImpl);
};

}  

#endif  
