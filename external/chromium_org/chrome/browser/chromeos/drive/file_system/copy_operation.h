// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_COPY_OPERATION_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_COPY_OPERATION_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/drive/file_errors.h"
#include "chrome/browser/drive/drive_service_interface.h"
#include "google_apis/drive/gdata_errorcode.h"

namespace base {
class FilePath;
class SequencedTaskRunner;
class Time;
}  

namespace google_apis {
class AboutResource;
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

class CreateFileOperation;
class OperationObserver;

class CopyOperation {
 public:
  CopyOperation(base::SequencedTaskRunner* blocking_task_runner,
                OperationObserver* observer,
                JobScheduler* scheduler,
                internal::ResourceMetadata* metadata,
                internal::FileCache* cache,
                const ResourceIdCanonicalizer& id_canonicalizer);
  ~CopyOperation();

  
  
  
  
  
  
  void Copy(const base::FilePath& src_file_path,
            const base::FilePath& dest_file_path,
            bool preserve_last_modified,
            const FileOperationCallback& callback);

  
  
  
  
  
  
  void TransferFileFromLocalToRemote(
      const base::FilePath& local_src_file_path,
      const base::FilePath& remote_dest_file_path,
      const FileOperationCallback& callback);

 private:
  
  struct CopyParams;

  
  void CopyAfterPrepare(const CopyParams& params,
                        ResourceEntry* src_entry,
                        std::string* parent_resource_id,
                        FileError error);

  
  
  
  void TransferFileFromLocalToRemoteAfterPrepare(
      const base::FilePath& local_src_path,
      const base::FilePath& remote_dest_path,
      const FileOperationCallback& callback,
      std::string* gdoc_resource_id,
      std::string* parent_resource_id,
      FileError error);

  
  
  void CopyResourceOnServer(const std::string& resource_id,
                            const std::string& parent_resource_id,
                            const std::string& new_title,
                            const base::Time& last_modified,
                            const FileOperationCallback& callback);

  
  void CopyResourceOnServerAfterServerSideCopy(
      const FileOperationCallback& callback,
      google_apis::GDataErrorCode status,
      scoped_ptr<google_apis::ResourceEntry> resource_entry);

  
  void CopyResourceOnServerAfterUpdateLocalState(
      const FileOperationCallback& callback,
      base::FilePath* file_path,
      FileError error);

  
  
  
  void ScheduleTransferRegularFile(const base::FilePath& local_src_path,
                                   const base::FilePath& remote_dest_path,
                                   const FileOperationCallback& callback);

  
  
  void ScheduleTransferRegularFileAfterGetFileSize(
      const base::FilePath& local_src_path,
      const base::FilePath& remote_dest_path,
      const FileOperationCallback& callback,
      int64 local_file_size);

  
  
  void ScheduleTransferRegularFileAfterGetAboutResource(
      const base::FilePath& local_src_path,
      const base::FilePath& remote_dest_path,
      const FileOperationCallback& callback,
      int64 local_file_size,
      google_apis::GDataErrorCode status,
      scoped_ptr<google_apis::AboutResource> about_resource);

  
  void ScheduleTransferRegularFileAfterCreate(
      const base::FilePath& local_src_path,
      const base::FilePath& remote_dest_path,
      const FileOperationCallback& callback,
      FileError error);

  
  
  void ScheduleTransferRegularFileAfterUpdateLocalState(
      const FileOperationCallback& callback,
      std::string* local_id,
      FileError error);

  scoped_refptr<base::SequencedTaskRunner> blocking_task_runner_;
  OperationObserver* observer_;
  JobScheduler* scheduler_;
  internal::ResourceMetadata* metadata_;
  internal::FileCache* cache_;
  ResourceIdCanonicalizer id_canonicalizer_;

  
  scoped_ptr<CreateFileOperation> create_file_operation_;

  
  
  base::WeakPtrFactory<CopyOperation> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(CopyOperation);
};

}  
}  

#endif  
