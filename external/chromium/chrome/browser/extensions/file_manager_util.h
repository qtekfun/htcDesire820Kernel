// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_FILE_MANAGER_UTIL_H_
#define CHROME_BROWSER_EXTENSIONS_FILE_MANAGER_UTIL_H_
#pragma once

#include "base/file_path.h"
#include "chrome/browser/ui/shell_dialogs.h"
#include "googleurl/src/gurl.h"

class Profile;

class FileManagerUtil {
 public:
  
  static GURL GetFileBrowserUrl();
  
  
  static bool ConvertFileToFileSystemUrl(Profile* profile,
      const FilePath& full_file_path, const GURL& origin_url, GURL* url);
  
  static GURL GetFileBrowserUrlWithParams(
      SelectFileDialog::Type type,
      const string16& title,
      const FilePath& default_path,
      const SelectFileDialog::FileTypeInfo* file_types,
      int file_type_index,
      const FilePath::StringType& default_extension);

  
  
  static void ShowFullTabUrl(Profile* profile,
                             const FilePath& default_path);

  static void ViewItem(const FilePath& full_path, bool enqueue);

 private:
  FileManagerUtil() {}
  
  static std::string GetDialogTypeAsString(SelectFileDialog::Type dialog_type);
  
  static std::string GetArgumentsJson(
      SelectFileDialog::Type type,
      const string16& title,
      const FilePath& default_path,
      const SelectFileDialog::FileTypeInfo* file_types,
      int file_type_index,
      const FilePath::StringType& default_extension);

  DISALLOW_COPY_AND_ASSIGN(FileManagerUtil);
};

#endif  
