// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_FILE_SYSTEM_FILE_SYSTEM_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_FILE_SYSTEM_FILE_SYSTEM_API_H_

#include "chrome/browser/extensions/chrome_extension_function.h"
#include "chrome/common/extensions/api/file_system.h"
#include "ui/shell_dialogs/select_file_dialog.h"

namespace base {
class FilePath;
}

namespace extensions {
class ExtensionPrefs;

namespace file_system_api {


base::FilePath GetLastChooseEntryDirectory(const ExtensionPrefs* prefs,
                                           const std::string& extension_id);

void SetLastChooseEntryDirectory(ExtensionPrefs* prefs,
                                 const std::string& extension_id,
                                 const base::FilePath& path);

}  

class FileSystemGetDisplayPathFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileSystem.getDisplayPath",
                             FILESYSTEM_GETDISPLAYPATH)

 protected:
  virtual ~FileSystemGetDisplayPathFunction() {}
  virtual bool RunImpl() OVERRIDE;
};

class FileSystemEntryFunction : public ChromeAsyncExtensionFunction {
 protected:
  FileSystemEntryFunction();

  virtual ~FileSystemEntryFunction() {}

  
  
  
  
  void CheckWritableFiles(const std::vector<base::FilePath>& path);

  
  
  
  void RegisterFileSystemsAndSendResponse(
      const std::vector<base::FilePath>& path);

  
  void CreateResponse();

  
  void AddEntryToResponse(const base::FilePath& path,
                          const std::string& id_override);

  
  void HandleWritableFileError(const base::FilePath& error_path);

  
  bool multiple_;

  
  bool is_directory_;

  
  base::DictionaryValue* response_;
};

class FileSystemGetWritableEntryFunction : public FileSystemEntryFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileSystem.getWritableEntry",
                             FILESYSTEM_GETWRITABLEENTRY)

 protected:
  virtual ~FileSystemGetWritableEntryFunction() {}
  virtual bool RunImpl() OVERRIDE;

 private:
  void CheckPermissionAndSendResponse();
  void SetIsDirectoryOnFileThread();

  
  base::FilePath path_;
};

class FileSystemIsWritableEntryFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileSystem.isWritableEntry",
                             FILESYSTEM_ISWRITABLEENTRY)

 protected:
  virtual ~FileSystemIsWritableEntryFunction() {}
  virtual bool RunImpl() OVERRIDE;
};

class FileSystemChooseEntryFunction : public FileSystemEntryFunction {
 public:
  
  static void SkipPickerAndAlwaysSelectPathForTest(base::FilePath* path);
  static void SkipPickerAndAlwaysSelectPathsForTest(
      std::vector<base::FilePath>* paths);
  static void SkipPickerAndSelectSuggestedPathForTest();
  static void SkipPickerAndAlwaysCancelForTest();
  static void StopSkippingPickerForTest();
  
  static void SkipDirectoryConfirmationForTest();
  static void AutoCancelDirectoryConfirmationForTest();
  static void StopSkippingDirectoryConfirmationForTest();
  
  
  
  static void RegisterTempExternalFileSystemForTest(const std::string& name,
                                                    const base::FilePath& path);

  DECLARE_EXTENSION_FUNCTION("fileSystem.chooseEntry", FILESYSTEM_CHOOSEENTRY)

  typedef std::vector<linked_ptr<extensions::api::file_system::AcceptOption> >
      AcceptOptions;

  static void BuildFileTypeInfo(
      ui::SelectFileDialog::FileTypeInfo* file_type_info,
      const base::FilePath::StringType& suggested_extension,
      const AcceptOptions* accepts,
      const bool* acceptsAllTypes);
  static void BuildSuggestion(const std::string* opt_name,
                              base::FilePath* suggested_name,
                              base::FilePath::StringType* suggested_extension);

 protected:
  class FilePicker;

  virtual ~FileSystemChooseEntryFunction() {}
  virtual bool RunImpl() OVERRIDE;
  void ShowPicker(const ui::SelectFileDialog::FileTypeInfo& file_type_info,
                  ui::SelectFileDialog::Type picker_type);

 private:
  void SetInitialPathOnFileThread(const base::FilePath& suggested_name,
                                  const base::FilePath& previous_path);

  
  void FilesSelected(const std::vector<base::FilePath>& path);
  void FileSelectionCanceled();

  
  
  
  
  
  void ConfirmDirectoryAccessOnFileThread(
      const std::vector<base::FilePath>& paths,
      content::WebContents* web_contents);
  void OnDirectoryAccessConfirmed(const std::vector<base::FilePath>& paths);

  base::FilePath initial_path_;
};

class FileSystemRetainEntryFunction : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileSystem.retainEntry", FILESYSTEM_RETAINENTRY)

 protected:
  virtual ~FileSystemRetainEntryFunction() {}
  virtual bool RunImpl() OVERRIDE;

 private:
  
  
  void RetainFileEntry(const std::string& entry_id);

  void SetIsDirectoryOnFileThread();

  
  bool is_directory_;

  
  base::FilePath path_;
};

class FileSystemIsRestorableFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileSystem.isRestorable", FILESYSTEM_ISRESTORABLE)

 protected:
  virtual ~FileSystemIsRestorableFunction() {}
  virtual bool RunImpl() OVERRIDE;
};

class FileSystemRestoreEntryFunction : public FileSystemEntryFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileSystem.restoreEntry", FILESYSTEM_RESTOREENTRY)

 protected:
  virtual ~FileSystemRestoreEntryFunction() {}
  virtual bool RunImpl() OVERRIDE;
};

}  

#endif  
