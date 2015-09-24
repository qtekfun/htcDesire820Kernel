// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_DOWNLOAD_OPERATION_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_DOWNLOAD_OPERATION_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/chromeos/drive/file_errors.h"
#include "chrome/browser/chromeos/drive/file_system_interface.h"
#include "chrome/browser/chromeos/drive/job_list.h"
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
class FileCache;
class ResourceMetadata;
}  

namespace file_system {

class OperationObserver;

class DownloadOperation {
 public:
  DownloadOperation(base::SequencedTaskRunner* blocking_task_runner,
                    OperationObserver* observer,
                    JobScheduler* scheduler,
                    internal::ResourceMetadata* metadata,
                    internal::FileCache* cache,
                    const base::FilePath& temporary_file_directory);
  ~DownloadOperation();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void EnsureFileDownloadedByLocalId(
      const std::string& local_id,
      const ClientContext& context,
      const GetFileContentInitializedCallback& initialized_callback,
      const google_apis::GetContentCallback& get_content_callback,
      const GetFileCallback& completion_callback);

  
  
  void EnsureFileDownloadedByPath(
      const base::FilePath& file_path,
      const ClientContext& context,
      const GetFileContentInitializedCallback& initialized_callback,
      const google_apis::GetContentCallback& get_content_callback,
      const GetFileCallback& completion_callback);

 private:
  
  class DownloadParams;

  
  
  void EnsureFileDownloadedAfterCheckPreCondition(
      scoped_ptr<DownloadParams> params,
      const ClientContext& context,
      base::FilePath* drive_file_path,
      base::FilePath* cache_file_path,
      FileError error);

  
  
  void EnsureFileDownloadedAfterPrepareForDownloadFile(
      scoped_ptr<DownloadParams> params,
      const ClientContext& context,
      const base::FilePath& drive_file_path,
      base::FilePath* temp_download_file_path,
      FileError error);

  
  void EnsureFileDownloadedAfterDownloadFile(
      const base::FilePath& drive_file_path,
      scoped_ptr<DownloadParams> params,
      google_apis::GDataErrorCode gdata_error,
      const base::FilePath& downloaded_file_path);

  
  
  void EnsureFileDownloadedAfterUpdateLocalState(
      const base::FilePath& file_path,
      scoped_ptr<DownloadParams> params,
      base::FilePath* cache_file_path,
      FileError error);

  
  void CancelJob(JobID job_id);

  scoped_refptr<base::SequencedTaskRunner> blocking_task_runner_;
  OperationObserver* observer_;
  JobScheduler* scheduler_;
  internal::ResourceMetadata* metadata_;
  internal::FileCache* cache_;
  const base::FilePath temporary_file_directory_;

  
  
  base::WeakPtrFactory<DownloadOperation> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(DownloadOperation);
};

}  
}  

#endif  
