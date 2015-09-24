// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_CREATE_DIRECTORY_OPERATION_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_CREATE_DIRECTORY_OPERATION_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/drive/file_errors.h"
#include "google_apis/drive/gdata_errorcode.h"

namespace base {
class FilePath;
class SequencedTaskRunner;
}  

namespace google_apis {
class ResourceEntry;
}  

namespace drive {

class JobScheduler;
class ResourceEntry;

namespace internal {
class ResourceMetadata;
}  

namespace file_system {

class OperationObserver;

class CreateDirectoryOperation {
 public:
  CreateDirectoryOperation(base::SequencedTaskRunner* blocking_task_runner,
                           OperationObserver* observer,
                           JobScheduler* scheduler,
                           internal::ResourceMetadata* metadata);
  ~CreateDirectoryOperation();

  
  
  
  
  
  
  
  void CreateDirectory(const base::FilePath& directory_path,
                       bool is_exclusive,
                       bool is_recursive,
                       const FileOperationCallback& callback);

 private:
  
  
  
  
  static base::FilePath GetExistingDeepestDirectory(
      internal::ResourceMetadata* metadata,
      const base::FilePath& directory_path,
      ResourceEntry* entry);

  
  
  void CreateDirectoryAfterGetExistingDeepestDirectory(
      const base::FilePath& directory_path,
      bool is_exclusive,
      bool is_recursive,
      const FileOperationCallback& callback,
      ResourceEntry* existing_deepest_directory_entry,
      const base::FilePath& existing_deepest_directory_path);

  
  
  
  
  
  void CreateDirectoryRecursively(
      const std::string& parent_resource_id,
      const base::FilePath& relative_file_path,
      const FileOperationCallback& callback);

  
  
  void CreateDirectoryRecursivelyAfterAddNewDirectory(
      const base::FilePath& remaining_path,
      const FileOperationCallback& callback,
      google_apis::GDataErrorCode gdata_error,
      scoped_ptr<google_apis::ResourceEntry> resource_entry);

  
  
  void CreateDirectoryRecursivelyAfterUpdateLocalState(
      const std::string& resource_id,
      const base::FilePath& remaining_path,
      const FileOperationCallback& callback,
      base::FilePath* file_path,
      FileError error);

  scoped_refptr<base::SequencedTaskRunner> blocking_task_runner_;
  OperationObserver* observer_;
  JobScheduler* scheduler_;
  internal::ResourceMetadata* metadata_;

  
  
  base::WeakPtrFactory<CreateDirectoryOperation> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(CreateDirectoryOperation);
};

}  
}  

#endif  
