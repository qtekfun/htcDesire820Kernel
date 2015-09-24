// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_SHELL_DIALOGS_SELECT_FILE_DIALOG_H_
#define UI_SHELL_DIALOGS_SELECT_FILE_DIALOG_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/shell_dialogs/base_shell_dialog.h"
#include "ui/shell_dialogs/shell_dialogs_export.h"

namespace ui {
class SelectFileDialogFactory;
class SelectFilePolicy;
struct SelectedFileInfo;
class ShellDialogsDelegate;

class SHELL_DIALOGS_EXPORT SelectFileDialog
    : public base::RefCountedThreadSafe<SelectFileDialog>,
      public BaseShellDialog {
 public:
  enum Type {
    SELECT_NONE,

    
    SELECT_FOLDER,

    
    
    SELECT_UPLOAD_FOLDER,

    
    SELECT_SAVEAS_FILE,

    
    SELECT_OPEN_FILE,

    
    SELECT_OPEN_MULTI_FILE
  };

  
  
  
  class SHELL_DIALOGS_EXPORT Listener {
   public:
    
    
    
    
    virtual void FileSelected(const base::FilePath& path,
                              int index, void* params) = 0;

    
    
    
    
    virtual void FileSelectedWithExtraInfo(
        const SelectedFileInfo& file,
        int index,
        void* params);

    
    
    virtual void MultiFilesSelected(
        const std::vector<base::FilePath>& files, void* params) {}

    
    
    
    
    virtual void MultiFilesSelectedWithExtraInfo(
        const std::vector<SelectedFileInfo>& files,
        void* params);

    
    
    
    virtual void FileSelectionCanceled(void* params) {}

   protected:
    virtual ~Listener() {}
  };

  
  
  
  
  
  
  static void SetFactory(SelectFileDialogFactory* factory);

  
  
  
  static scoped_refptr<SelectFileDialog> Create(Listener* listener,
                                                SelectFilePolicy* policy);

  
  struct SHELL_DIALOGS_EXPORT FileTypeInfo {
    FileTypeInfo();
    ~FileTypeInfo();

    
    
    
    
    
    
    std::vector<std::vector<base::FilePath::StringType> > extensions;

    
    
    
    std::vector<base::string16> extension_description_overrides;

    
    bool include_all_files;

    
    
    
    
    
    bool support_drive;
  };

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void SelectFile(Type type,
                  const base::string16& title,
                  const base::FilePath& default_path,
                  const FileTypeInfo* file_types,
                  int file_type_index,
                  const base::FilePath::StringType& default_extension,
                  gfx::NativeWindow owning_window,
                  void* params);
  bool HasMultipleFileTypeChoices();

  
  static void SetShellDialogsDelegate(ShellDialogsDelegate* delegate);

 protected:
  friend class base::RefCountedThreadSafe<SelectFileDialog>;
  explicit SelectFileDialog(Listener* listener, SelectFilePolicy* policy);
  virtual ~SelectFileDialog();

  
  
  
  
  virtual void SelectFileImpl(
      Type type,
      const base::string16& title,
      const base::FilePath& default_path,
      const FileTypeInfo* file_types,
      int file_type_index,
      const base::FilePath::StringType& default_extension,
      gfx::NativeWindow owning_window,
      void* params) = 0;

  
  ShellDialogsDelegate* GetShellDialogsDelegate();

  
  Listener* listener_;

 private:
  
  
  
  bool CanOpenSelectFileDialog();

  
  
  void CancelFileSelection(void* params);

  
  virtual bool HasMultipleFileTypeChoicesImpl() = 0;

  scoped_ptr<SelectFilePolicy> select_file_policy_;

  DISALLOW_COPY_AND_ASSIGN(SelectFileDialog);
};

}  

#endif  
