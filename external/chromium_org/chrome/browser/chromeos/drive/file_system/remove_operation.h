// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_REMOVE_OPERATION_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_REMOVE_OPERATION_H_

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

namespace drive {

class ResourceEntry;

namespace internal {
class FileCache;
class ResourceMetadata;
}  

namespace file_system {

class OperationObserver;

class RemoveOperation {
 public:
  RemoveOperation(base::SequencedTaskRunner* blocking_task_runner,
                  OperationObserver* observer,
                  internal::ResourceMetadata* metadata,
                  internal::FileCache* cache);
  ~RemoveOperation();

  
  
  
  
  
  void Remove(const base::FilePath& path,
              bool is_recursive,
              const FileOperationCallback& callback);

 private:
  
  void RemoveAfterUpdateLocalState(const FileOperationCallback& callback,
                                   const std::string* local_id,
                                   const base::FilePath* changed_directory_path,
                                   FileError error);

  scoped_refptr<base::SequencedTaskRunner> blocking_task_runner_;
  OperationObserver* observer_;
  internal::ResourceMetadata* metadata_;
  internal::FileCache* cache_;

  
  
  base::WeakPtrFactory<RemoveOperation> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(RemoveOperation);
};

}  
}  

#endif  
