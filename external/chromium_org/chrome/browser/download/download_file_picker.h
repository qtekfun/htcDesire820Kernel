// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_FILE_PICKER_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_FILE_PICKER_H_

#include "base/callback.h"
#include "ui/shell_dialogs/select_file_dialog.h"

namespace base {
class FilePath;
}

namespace content {
class DownloadItem;
class DownloadManager;
class WebContents;
}

class DownloadFilePicker : public ui::SelectFileDialog::Listener {
 public:
  
  
  
  
  
  
  typedef base::Callback<void(const base::FilePath& virtual_path)>
      FileSelectedCallback;

  
  
  
  static void ShowFilePicker(content::DownloadItem* item,
                             const base::FilePath& suggested_path,
                             const FileSelectedCallback& callback);

 private:
  DownloadFilePicker(content::DownloadItem* item,
                     const base::FilePath& suggested_path,
                     const FileSelectedCallback& callback);
  virtual ~DownloadFilePicker();

  
  
  void OnFileSelected(const base::FilePath& virtual_path);

  
  virtual void FileSelected(const base::FilePath& path,
                            int index,
                            void* params) OVERRIDE;
  virtual void FileSelectionCanceled(void* params) OVERRIDE;

  
  base::FilePath suggested_path_;

  
  FileSelectedCallback file_selected_callback_;

  
  scoped_refptr<ui::SelectFileDialog> select_file_dialog_;

  
  
  bool should_record_file_picker_result_;

  DISALLOW_COPY_AND_ASSIGN(DownloadFilePicker);
};

#endif  
