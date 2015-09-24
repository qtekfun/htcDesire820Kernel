// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_TEST_TEST_FILE_SYSTEM_BACKEND_H_
#define CONTENT_PUBLIC_TEST_TEST_FILE_SYSTEM_BACKEND_H_

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "webkit/browser/fileapi/async_file_util_adapter.h"
#include "webkit/browser/fileapi/file_system_backend.h"
#include "webkit/browser/fileapi/task_runner_bound_observer_list.h"

namespace base {
class SequencedTaskRunner;
}

namespace fileapi {

class AsyncFileUtilAdapter;
class FileSystemQuotaUtil;

class TestFileSystemBackend : public FileSystemBackend {
 public:
  TestFileSystemBackend(
      base::SequencedTaskRunner* task_runner,
      const base::FilePath& base_path);
  virtual ~TestFileSystemBackend();

  
  virtual bool CanHandleType(FileSystemType type) const OVERRIDE;
  virtual void Initialize(FileSystemContext* context) OVERRIDE;
  virtual void OpenFileSystem(
      const GURL& origin_url,
      FileSystemType type,
      OpenFileSystemMode mode,
      const OpenFileSystemCallback& callback) OVERRIDE;
  virtual AsyncFileUtil* GetAsyncFileUtil(FileSystemType type) OVERRIDE;
  virtual CopyOrMoveFileValidatorFactory* GetCopyOrMoveFileValidatorFactory(
      FileSystemType type,
      base::PlatformFileError* error_code) OVERRIDE;
  virtual FileSystemOperation* CreateFileSystemOperation(
      const FileSystemURL& url,
      FileSystemContext* context,
      base::PlatformFileError* error_code) const OVERRIDE;
  virtual scoped_ptr<webkit_blob::FileStreamReader> CreateFileStreamReader(
      const FileSystemURL& url,
      int64 offset,
      const base::Time& expected_modification_time,
      FileSystemContext* context) const OVERRIDE;
  virtual scoped_ptr<FileStreamWriter> CreateFileStreamWriter(
      const FileSystemURL& url,
      int64 offset,
      FileSystemContext* context) const OVERRIDE;
  virtual FileSystemQuotaUtil* GetQuotaUtil() OVERRIDE;

  
  
  void InitializeCopyOrMoveFileValidatorFactory(
      scoped_ptr<CopyOrMoveFileValidatorFactory> factory);

  const UpdateObserverList* GetUpdateObservers(FileSystemType type) const;
  void AddFileChangeObserver(FileChangeObserver* observer);

  
  
  
  void set_require_copy_or_move_validator(bool flag) {
    require_copy_or_move_validator_ = flag;
  }

 private:
  class QuotaUtil;

  base::FilePath base_path_;
  scoped_refptr<base::SequencedTaskRunner> task_runner_;
  scoped_ptr<AsyncFileUtilAdapter> file_util_;
  scoped_ptr<QuotaUtil> quota_util_;

  bool require_copy_or_move_validator_;
  scoped_ptr<CopyOrMoveFileValidatorFactory>
      copy_or_move_file_validator_factory_;

  DISALLOW_COPY_AND_ASSIGN(TestFileSystemBackend);
};

}  

#endif  
