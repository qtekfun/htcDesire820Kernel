// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_FILE_SYSTEM_OPERATION_CONTEXT_H_
#define WEBKIT_BROWSER_FILEAPI_FILE_SYSTEM_OPERATION_CONTEXT_H_

#include "base/files/file_path.h"
#include "base/supports_user_data.h"
#include "base/threading/thread_checker.h"
#include "webkit/browser/fileapi/task_runner_bound_observer_list.h"
#include "webkit/browser/webkit_storage_browser_export.h"
#include "webkit/common/quota/quota_types.h"

namespace base {
class SequencedTaskRunner;
}

namespace fileapi {

class FileSystemContext;

class WEBKIT_STORAGE_BROWSER_EXPORT_PRIVATE FileSystemOperationContext
    : public base::SupportsUserData {
 public:
  explicit FileSystemOperationContext(FileSystemContext* context);

  
  
  FileSystemOperationContext(FileSystemContext* context,
                             base::SequencedTaskRunner* task_runner);

  virtual ~FileSystemOperationContext();

  FileSystemContext* file_system_context() const {
    return file_system_context_.get();
  }

  
  
  void set_allowed_bytes_growth(const int64& allowed_bytes_growth) {
    allowed_bytes_growth_ = allowed_bytes_growth;
  }

  
  int64 allowed_bytes_growth() const { return allowed_bytes_growth_; }
  quota::QuotaLimitType quota_limit_type() const { return quota_limit_type_; }
  base::SequencedTaskRunner* task_runner() const { return task_runner_.get(); }

  ChangeObserverList* change_observers() { return &change_observers_; }
  UpdateObserverList* update_observers() { return &update_observers_; }

  
  
  
  void set_change_observers(const ChangeObserverList& list) {
    DCHECK(setter_thread_checker_.CalledOnValidThread());
    change_observers_ = list;
  }
  void set_update_observers(const UpdateObserverList& list) {
    DCHECK(setter_thread_checker_.CalledOnValidThread());
    update_observers_ = list;
  }
  void set_quota_limit_type(quota::QuotaLimitType limit_type) {
    DCHECK(setter_thread_checker_.CalledOnValidThread());
    quota_limit_type_ = limit_type;
  }

 private:
  scoped_refptr<FileSystemContext> file_system_context_;
  scoped_refptr<base::SequencedTaskRunner> task_runner_;

  
  int64 allowed_bytes_growth_;

  
  quota::QuotaLimitType quota_limit_type_;

  
  ChangeObserverList change_observers_;
  UpdateObserverList update_observers_;

  
  base::ThreadChecker setter_thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(FileSystemOperationContext);
};

}  

#endif  
