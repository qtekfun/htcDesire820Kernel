// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_GET_FILE_FOR_SAVING_OPERATION_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_GET_FILE_FOR_SAVING_OPERATION_H_

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
namespace internal {
class FileCache;
class FileWriteWatcher;
class ResourceMetadata;
}  

class JobScheduler;
class ResourceEntry;

namespace file_system {

class CreateFileOperation;
class DownloadOperation;
class OperationObserver;

class GetFileForSavingOperation {
 public:
  GetFileForSavingOperation(base::SequencedTaskRunner* blocking_task_runner,
                            OperationObserver* observer,
                            JobScheduler* scheduler,
                            internal::ResourceMetadata* metadata,
                            internal::FileCache* cache,
                            const base::FilePath& temporary_file_directory);
  ~GetFileForSavingOperation();

  
  
  
  
  void GetFileForSaving(const base::FilePath& file_path,
                        const GetFileCallback& callback);

  internal::FileWriteWatcher* file_write_watcher_for_testing() {
    return file_write_watcher_.get();
  }

 private:
  void GetFileForSavingAfterCreate(const base::FilePath& file_path,
                                   const GetFileCallback& callback,
                                   FileError error);
  void GetFileForSavingAfterDownload(const GetFileCallback& callback,
                                     FileError error,
                                     const base::FilePath& cache_path,
                                     scoped_ptr<ResourceEntry> entry);
  void GetFileForSavingAfterMarkDirty(const GetFileCallback& callback,
                                      const base::FilePath& cache_path,
                                      scoped_ptr<ResourceEntry> entry,
                                      FileError error);
  void GetFileForSavingAfterWatch(const GetFileCallback& callback,
                                  const base::FilePath& cache_path,
                                  scoped_ptr<ResourceEntry> entry,
                                  bool success);
  // Called when the cache file for |local_id| is written.
  void OnWriteEvent(const std::string& local_id);

  scoped_ptr<CreateFileOperation> create_file_operation_;
  scoped_ptr<DownloadOperation> download_operation_;
  scoped_ptr<internal::FileWriteWatcher> file_write_watcher_;
  scoped_refptr<base::SequencedTaskRunner> blocking_task_runner_;
  OperationObserver* observer_;
  internal::ResourceMetadata* metadata_;
  internal::FileCache* cache_;

  
  
  base::WeakPtrFactory<GetFileForSavingOperation> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(GetFileForSavingOperation);
};

}  
}  

#endif  
