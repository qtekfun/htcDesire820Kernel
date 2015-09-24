// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_TOUCH_OPERATION_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_TOUCH_OPERATION_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/drive/file_errors.h"
#include "google_apis/drive/gdata_errorcode.h"

namespace base {
class FilePath;
class SequencedTaskRunner;
class Time;
}  

namespace google_apis {
class ResourceEntry;
}  

namespace drive {
namespace internal {
class ResourceMetadata;
}  

class ResourceEntry;

namespace file_system {

class OperationObserver;

class TouchOperation {
 public:
  TouchOperation(base::SequencedTaskRunner* blocking_task_runner,
                 OperationObserver* observer,
                 internal::ResourceMetadata* metadata);
  ~TouchOperation();

  
  
  
  void TouchFile(const base::FilePath& file_path,
                 const base::Time& last_access_time,
                 const base::Time& last_modified_time,
                 const FileOperationCallback& callback);

 private:
  
  void TouchFileAfterUpdateLocalState(const base::FilePath& file_path,
                                      const FileOperationCallback& callback,
                                      const std::string* local_id,
                                      FileError error);

  scoped_refptr<base::SequencedTaskRunner> blocking_task_runner_;
  OperationObserver* observer_;
  internal::ResourceMetadata* metadata_;

  
  
  base::WeakPtrFactory<TouchOperation> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(TouchOperation);
};

}  
}  

#endif  
