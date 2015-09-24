// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SHELL_DIALOGS_H_
#define CHROME_BROWSER_UI_SHELL_DIALOGS_H_
#pragma once

#include <string>
#include <vector>

#include "base/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/string16.h"
#include "ui/gfx/native_widget_types.h"

class TabContents;

extern std::wstring AppendExtensionIfNeeded(const std::wstring& filename,
                                            const std::wstring& filter_selected,
                                            const std::wstring& suggested_ext);

class BaseShellDialog {
 public:
  
  
  virtual bool IsRunning(gfx::NativeWindow owning_window) const = 0;

  
  
  virtual void ListenerDestroyed() = 0;

 protected:
  virtual ~BaseShellDialog() {}
};

class SelectFileDialog
    : public base::RefCountedThreadSafe<SelectFileDialog>,
      public BaseShellDialog {
 public:
  enum Type {
    SELECT_NONE,
    SELECT_FOLDER,
    SELECT_SAVEAS_FILE,
    SELECT_OPEN_FILE,
    SELECT_OPEN_MULTI_FILE
  };

  
  
  
  class Listener {
   public:
    
    
    
    
    virtual void FileSelected(const FilePath& path,
                              int index, void* params) = 0;

    
    
    virtual void MultiFilesSelected(
      const std::vector<FilePath>& files, void* params) {}

    
    
    
    virtual void FileSelectionCanceled(void* params) {}

   protected:
    virtual ~Listener() {}
  };

  
  
  static SelectFileDialog* Create(Listener* listener);

  
  
  
  
  
  
  
  
  
  
  struct FileTypeInfo {
    FileTypeInfo();
    ~FileTypeInfo();

    std::vector<std::vector<FilePath::StringType> > extensions;
    std::vector<string16> extension_description_overrides;
    bool include_all_files;
  };

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void SelectFile(Type type,
                  const string16& title,
                  const FilePath& default_path,
                  const FileTypeInfo* file_types,
                  int file_type_index,
                  const FilePath::StringType& default_extension,
                  TabContents* source_contents,
                  gfx::NativeWindow owning_window,
                  void* params);

  
  virtual void set_browser_mode(bool value) {}

 protected:
  friend class base::RefCountedThreadSafe<SelectFileDialog>;
  explicit SelectFileDialog(Listener* listener);
  virtual ~SelectFileDialog();

  
  
  
  
  virtual void SelectFileImpl(Type type,
                              const string16& title,
                              const FilePath& default_path,
                              const FileTypeInfo* file_types,
                              int file_type_index,
                              const FilePath::StringType& default_extension,
                              gfx::NativeWindow owning_window,
                              void* params) = 0;

  
  Listener* listener_;

 private:
  
  
  
  bool CanOpenSelectFileDialog();

  
  
  void CancelFileSelection(void* params);
};

#endif  
