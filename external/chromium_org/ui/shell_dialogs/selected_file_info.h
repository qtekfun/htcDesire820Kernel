// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_SHELL_DIALOGS_SELECTED_FILE_INFO_H_
#define UI_SHELL_DIALOGS_SELECTED_FILE_INFO_H_

#include <vector>

#include "base/files/file_path.h"
#include "base/strings/string16.h"
#include "ui/shell_dialogs/shell_dialogs_export.h"

namespace ui {

struct SHELL_DIALOGS_EXPORT SelectedFileInfo {
  
  base::FilePath file_path;

  
  
  
  
  
  
  base::FilePath local_path;

  
  
  
  
  base::FilePath::StringType display_name;

  SelectedFileInfo();
  SelectedFileInfo(const base::FilePath& in_file_path,
                   const base::FilePath& in_local_path);
  ~SelectedFileInfo();
};

}  

#endif  

