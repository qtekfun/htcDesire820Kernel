// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef CHROME_BROWSER_UI_METRO_DRIVER_FILE_PICKER_ASH_H_
#define CHROME_BROWSER_UI_METRO_DRIVER_FILE_PICKER_ASH_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/strings/string16.h"

class ChromeAppViewAsh;
struct MetroViewerHostMsg_SaveAsDialogParams;

namespace base {
class FilePath;
}

class FilePickerSessionBase {
 public:
  
  explicit FilePickerSessionBase(ChromeAppViewAsh* app_view,
                                 const string16& title,
                                 const string16& filter,
                                 const base::FilePath& default_path);

  virtual ~FilePickerSessionBase() {
  }

  
  bool Run();

  const string16& result() const {
    return result_;
  }

  bool success() const {
    return success_;
  }

 protected:
  
  
  
  virtual HRESULT StartFilePicker() = 0;

  
  bool success_;

  
  string16 title_;

  
  string16 filter_;

  
  base::FilePath default_path_;

  
  
  ChromeAppViewAsh* app_view_;

  string16 result_;

 private:
  
  
  bool DoFilePicker();

  DISALLOW_COPY_AND_ASSIGN(FilePickerSessionBase);
};

class OpenFilePickerSession : public FilePickerSessionBase {
 public:
  explicit OpenFilePickerSession(ChromeAppViewAsh* app_view,
                                 const string16& title,
                                 const string16& filter,
                                 const base::FilePath& default_path,
                                 bool allow_multi_select);

  const std::vector<base::FilePath>& filenames() const {
    return filenames_;
  }

  const bool allow_multi_select() const {
    return allow_multi_select_;
  }

 private:
  virtual HRESULT StartFilePicker() OVERRIDE;

  typedef winfoundtn::IAsyncOperation<winstorage::StorageFile*>
      SingleFileAsyncOp;
  typedef winfoundtn::Collections::IVectorView<
      winstorage::StorageFile*> StorageFileVectorCollection;
  typedef winfoundtn::IAsyncOperation<StorageFileVectorCollection*>
      MultiFileAsyncOp;

  
  HRESULT SinglePickerDone(SingleFileAsyncOp* async, AsyncStatus status);

  
  HRESULT MultiPickerDone(MultiFileAsyncOp* async, AsyncStatus status);

  
  
  static HRESULT ComposeMultiFileResult(StorageFileVectorCollection* files,
                                        string16* result);

 private:
  
  bool allow_multi_select_;
  
  
  std::vector<base::FilePath> filenames_;

  DISALLOW_COPY_AND_ASSIGN(OpenFilePickerSession);
};

class SaveFilePickerSession : public FilePickerSessionBase {
 public:
  explicit SaveFilePickerSession(
      ChromeAppViewAsh* app_view,
      const MetroViewerHostMsg_SaveAsDialogParams& params);

  int SaveFilePickerSession::filter_index() const;

 private:
  virtual HRESULT StartFilePicker() OVERRIDE;

  typedef winfoundtn::IAsyncOperation<winstorage::StorageFile*>
      SaveFileAsyncOp;

  
  HRESULT FilePickerDone(SaveFileAsyncOp* async, AsyncStatus status);

  int filter_index_;

  DISALLOW_COPY_AND_ASSIGN(SaveFilePickerSession);
};

class FolderPickerSession : public FilePickerSessionBase {
 public:
  explicit FolderPickerSession(ChromeAppViewAsh* app_view,
                               const string16& title);

 private:
  virtual HRESULT StartFilePicker() OVERRIDE;

  typedef winfoundtn::IAsyncOperation<winstorage::StorageFolder*>
      FolderPickerAsyncOp;

  
  HRESULT FolderPickerDone(FolderPickerAsyncOp* async, AsyncStatus status);

  DISALLOW_COPY_AND_ASSIGN(FolderPickerSession);
};

#endif  

