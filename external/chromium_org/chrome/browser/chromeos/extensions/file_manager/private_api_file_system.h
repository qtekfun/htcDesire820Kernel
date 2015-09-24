// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_EXTENSIONS_FILE_MANAGER_PRIVATE_API_FILE_SYSTEM_H_
#define CHROME_BROWSER_CHROMEOS_EXTENSIONS_FILE_MANAGER_PRIVATE_API_FILE_SYSTEM_H_

#include <string>

#include "base/platform_file.h"
#include "chrome/browser/chromeos/drive/file_errors.h"
#include "chrome/browser/chromeos/extensions/file_manager/private_api_base.h"

class GURL;

namespace base {
class FilePath;
}

namespace fileapi {
class FileSystemContext;
}

namespace extensions {

class FileBrowserPrivateRequestFileSystemFunction
    : public LoggedAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.requestFileSystem",
                             FILEBROWSERPRIVATE_REQUESTFILESYSTEM)

 protected:
  virtual ~FileBrowserPrivateRequestFileSystemFunction() {}

  
  virtual bool RunImpl() OVERRIDE;

 private:
  void RespondSuccessOnUIThread(const std::string& name,
                                const GURL& root_url);
  void RespondFailedOnUIThread(base::PlatformFileError error_code);

  
  
  void DidFail(base::PlatformFileError error_code);

  
  
  bool SetupFileSystemAccessPermissions(
      scoped_refptr<fileapi::FileSystemContext> file_system_context,
      int child_id,
      scoped_refptr<const extensions::Extension> extension);
};

class FileWatchFunctionBase : public LoggedAsyncExtensionFunction {
 protected:
  virtual ~FileWatchFunctionBase() {}

  
  virtual void PerformFileWatchOperation(
      const base::FilePath& local_path,
      const base::FilePath& virtual_path,
      const std::string& extension_id) = 0;

  
  virtual bool RunImpl() OVERRIDE;

  
  void Respond(bool success);
};

class FileBrowserPrivateAddFileWatchFunction : public FileWatchFunctionBase {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.addFileWatch",
                             FILEBROWSERPRIVATE_ADDFILEWATCH)

 protected:
  virtual ~FileBrowserPrivateAddFileWatchFunction() {}

  
  virtual void PerformFileWatchOperation(
      const base::FilePath& local_path,
      const base::FilePath& virtual_path,
      const std::string& extension_id) OVERRIDE;
};


class FileBrowserPrivateRemoveFileWatchFunction : public FileWatchFunctionBase {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.removeFileWatch",
                             FILEBROWSERPRIVATE_REMOVEFILEWATCH)

 protected:
  virtual ~FileBrowserPrivateRemoveFileWatchFunction() {}

  
  virtual void PerformFileWatchOperation(
      const base::FilePath& local_path,
      const base::FilePath& virtual_path,
      const std::string& extension_id) OVERRIDE;
};

class FileBrowserPrivateGetSizeStatsFunction
    : public LoggedAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.getSizeStats",
                             FILEBROWSERPRIVATE_GETSIZESTATS)

 protected:
  virtual ~FileBrowserPrivateGetSizeStatsFunction() {}

  
  virtual bool RunImpl() OVERRIDE;

 private:
  void GetDriveAvailableSpaceCallback(drive::FileError error,
                                      int64 bytes_total,
                                      int64 bytes_used);

  void GetSizeStatsCallback(const uint64* total_size,
                            const uint64* remaining_size);
};

class FileBrowserPrivateValidatePathNameLengthFunction
    : public LoggedAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.validatePathNameLength",
                             FILEBROWSERPRIVATE_VALIDATEPATHNAMELENGTH)

 protected:
  virtual ~FileBrowserPrivateValidatePathNameLengthFunction() {}

  void OnFilePathLimitRetrieved(size_t current_length, size_t max_length);

  
  virtual bool RunImpl() OVERRIDE;
};

class FileBrowserPrivateFormatVolumeFunction
    : public LoggedAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.formatVolume",
                             FILEBROWSERPRIVATE_FORMATVOLUME)

 protected:
  virtual ~FileBrowserPrivateFormatVolumeFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class FileBrowserPrivateStartCopyFunction
    : public LoggedAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.startCopy",
                             FILEBROWSERPRIVATE_STARTCOPY)

 protected:
  virtual ~FileBrowserPrivateStartCopyFunction() {}

  
  virtual bool RunImpl() OVERRIDE;

 private:
  
  void RunAfterStartCopy(int operation_id);
};

class FileBrowserPrivateCancelCopyFunction
    : public LoggedAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.cancelCopy",
                             FILEBROWSERPRIVATE_CANCELCOPY)

 protected:
  virtual ~FileBrowserPrivateCancelCopyFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

}  

#endif  
