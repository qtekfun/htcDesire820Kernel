// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_RECURSIVE_OPERATION_DELEGATE_H_
#define WEBKIT_BROWSER_FILEAPI_RECURSIVE_OPERATION_DELEGATE_H_

#include <queue>
#include <stack>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/weak_ptr.h"
#include "webkit/browser/fileapi/file_system_operation.h"
#include "webkit/browser/fileapi/file_system_url.h"

namespace fileapi {

class FileSystemContext;
class FileSystemOperationRunner;

class WEBKIT_STORAGE_BROWSER_EXPORT RecursiveOperationDelegate
    : public base::SupportsWeakPtr<RecursiveOperationDelegate> {
 public:
  typedef FileSystemOperation::StatusCallback StatusCallback;
  typedef FileSystemOperation::FileEntryList FileEntryList;

  virtual ~RecursiveOperationDelegate();

  
  
  virtual void Run() = 0;

  
  
  virtual void RunRecursively() = 0;

  
  
  virtual void ProcessFile(const FileSystemURL& url,
                           const StatusCallback& callback) = 0;

  
  
  virtual void ProcessDirectory(const FileSystemURL& url,
                                const StatusCallback& callback) = 0;


  
  
  virtual void PostProcessDirectory(const FileSystemURL& url,
                                    const StatusCallback& callback) = 0;

  
  void Cancel();

 protected:
  explicit RecursiveOperationDelegate(FileSystemContext* file_system_context);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void StartRecursiveOperation(const FileSystemURL& root,
                               const StatusCallback& callback);

  FileSystemContext* file_system_context() { return file_system_context_; }
  const FileSystemContext* file_system_context() const {
    return file_system_context_;
  }

  FileSystemOperationRunner* operation_runner();

  
  
  virtual void OnCancel();

 private:
  void DidTryProcessFile(const FileSystemURL& root,
                         base::PlatformFileError error);
  void ProcessNextDirectory();
  void DidProcessDirectory(base::PlatformFileError error);
  void DidReadDirectory(const FileSystemURL& parent,
                        base::PlatformFileError error,
                        const FileEntryList& entries,
                        bool has_more);
  void ProcessPendingFiles();
  void DidProcessFile(base::PlatformFileError error);
  void ProcessSubDirectory();
  void DidPostProcessDirectory(base::PlatformFileError error);

  
  void Done(base::PlatformFileError error);

  FileSystemContext* file_system_context_;
  StatusCallback callback_;
  std::stack<FileSystemURL> pending_directories_;
  std::stack<std::queue<FileSystemURL> > pending_directory_stack_;
  std::queue<FileSystemURL> pending_files_;
  int inflight_operations_;
  bool canceled_;

  DISALLOW_COPY_AND_ASSIGN(RecursiveOperationDelegate);
};

}  

#endif  
