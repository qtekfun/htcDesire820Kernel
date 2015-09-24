// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_SAVE_PACKAGE_FILE_PICKER_H_
#define CHROME_BROWSER_DOWNLOAD_SAVE_PACKAGE_FILE_PICKER_H_

#include "base/memory/ref_counted.h"
#include "content/public/browser/download_manager_delegate.h"
#include "ui/shell_dialogs/select_file_dialog.h"

class DownloadPrefs;

class SavePackageFilePicker : public ui::SelectFileDialog::Listener {
 public:
  SavePackageFilePicker(
      content::WebContents* web_contents,
      const base::FilePath& suggested_path,
      const base::FilePath::StringType& default_extension,
      bool can_save_as_complete,
      DownloadPrefs* download_prefs,
      const content::SavePackagePathPickedCallback& callback);
  virtual ~SavePackageFilePicker();

  
  
  static void SetShouldPromptUser(bool should_prompt);

 private:
  
  virtual void FileSelected(const base::FilePath& path,
                            int index,
                            void* unused_params) OVERRIDE;
  virtual void FileSelectionCanceled(void* unused_params) OVERRIDE;

  bool ShouldSaveAsMHTML() const;

  
  int render_process_id_;

  
  bool can_save_as_complete_;

  DownloadPrefs* download_prefs_;

  content::SavePackagePathPickedCallback callback_;

  
  scoped_refptr<ui::SelectFileDialog> select_file_dialog_;

  DISALLOW_COPY_AND_ASSIGN(SavePackageFilePicker);
};

#endif  
