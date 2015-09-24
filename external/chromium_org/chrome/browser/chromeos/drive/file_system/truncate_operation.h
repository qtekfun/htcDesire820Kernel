// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_TRUNCATE_OPERATION_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_TRUNCATE_OPERATION_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/drive/file_errors.h"

namespace base {
class FilePath;
class SequencedTaskRunner;
}  

namespace drive {

class JobScheduler;
class ResourceEntry;

namespace internal {
class FileCache;
class ResourceMetadata;
}  

namespace file_system {

class OperationObserver;
class DownloadOperation;

class TruncateOperation {
 public:
  TruncateOperation(base::SequencedTaskRunner* blocking_task_runner,
                    OperationObserver* observer,
                    JobScheduler* scheduler,
                    internal::ResourceMetadata* metadata,
                    internal::FileCache* cache,
                    const base::FilePath& temporary_file_directory);
  ~TruncateOperation();

  
  
  
  void Truncate(const base::FilePath& file_path,
                int64 length,
                const FileOperationCallback& callback);
 private:
  
  void TruncateAfterEnsureFileDownloadedByPath(
      int64 length,
      const FileOperationCallback& callback,
      FileError error,
      const base::FilePath& local_file_path,
      scoped_ptr<ResourceEntry> resource_entry);

  
  void TruncateAfterTruncateOnBlockingPool(
      const std::string& local_id,
      const FileOperationCallback& callback,
      FileError error);

  scoped_refptr<base::SequencedTaskRunner> blocking_task_runner_;
  OperationObserver* observer_;
  internal::ResourceMetadata* metadata_;
  internal::FileCache* cache_;

  scoped_ptr<DownloadOperation> download_operation_;

  
  
  base::WeakPtrFactory<TruncateOperation> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(TruncateOperation);
};

}  
}  

#endif  
