// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_OPEN_FILE_OPERATION_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_OPEN_FILE_OPERATION_H_

#include <map>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/drive/file_errors.h"
#include "chrome/browser/chromeos/drive/file_system_interface.h"

namespace base {
class FilePath;
class SequencedTaskRunner;
}  

namespace drive {

class JobScheduler;
class ResourceEntry;

namespace internal {
class ResourceMetadata;
class FileCache;
}  

namespace file_system {

class CreateFileOperation;
class DownloadOperation;
class OperationObserver;

class OpenFileOperation {
 public:
  OpenFileOperation(base::SequencedTaskRunner* blocking_task_runner,
                    OperationObserver* observer,
                    JobScheduler* scheduler,
                    internal::ResourceMetadata* metadata,
                    internal::FileCache* cache,
                    const base::FilePath& temporary_file_directory);
  ~OpenFileOperation();

  
  
  
  
  
  
  
  
  void OpenFile(const base::FilePath& file_path,
                OpenMode open_mode,
                const std::string& mime_type,
                const OpenFileCallback& callback);

 private:
  
  void OpenFileAfterCreateFile(const base::FilePath& file_path,
                               const OpenFileCallback& callback,
                               FileError error);

  
  void OpenFileAfterFileDownloaded(const OpenFileCallback& callback,
                                   FileError error,
                                   const base::FilePath& local_file_path,
                                   scoped_ptr<ResourceEntry> entry);

  
  void OpenFileAfterMarkDirty(const base::FilePath& local_file_path,
                              const std::string& local_id,
                              const OpenFileCallback& callback,
                              FileError error);

  
  void CloseFile(const std::string& local_id);

  scoped_refptr<base::SequencedTaskRunner> blocking_task_runner_;
  OperationObserver* observer_;
  internal::FileCache* cache_;

  scoped_ptr<CreateFileOperation> create_file_operation_;
  scoped_ptr<DownloadOperation> download_operation_;

  
  
  std::map<std::string, int> open_files_;

  
  
  base::WeakPtrFactory<OpenFileOperation> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(OpenFileOperation);
};

}  
}  

#endif  
