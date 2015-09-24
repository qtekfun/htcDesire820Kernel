// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DRIVE_DRIVE_UPLOADER_H_
#define CHROME_BROWSER_DRIVE_DRIVE_UPLOADER_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/drive/drive_service_interface.h"
#include "google_apis/drive/gdata_errorcode.h"
#include "google_apis/drive/gdata_wapi_parser.h"

class GURL;

namespace base {
class FilePath;
class TaskRunner;
}

namespace google_apis {
struct UploadRangeResponse;
}

namespace drive {
class DriveServiceInterface;

typedef base::Callback<void(
    google_apis::GDataErrorCode error,
    const GURL& upload_location,
    scoped_ptr<google_apis::ResourceEntry> resource_entry)>
    UploadCompletionCallback;

class DriveUploaderInterface {
 public:
  virtual ~DriveUploaderInterface() {}

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual google_apis::CancelCallback UploadNewFile(
      const std::string& parent_resource_id,
      const base::FilePath& local_file_path,
      const std::string& title,
      const std::string& content_type,
      const UploadCompletionCallback& callback,
      const google_apis::ProgressCallback& progress_callback) = 0;

  
  
  
  
  
  //   resource id of the existing file to be overwritten.
  
  
  
  
  
  virtual google_apis::CancelCallback UploadExistingFile(
      const std::string& resource_id,
      const base::FilePath& local_file_path,
      const std::string& content_type,
      const std::string& etag,
      const UploadCompletionCallback& callback,
      const google_apis::ProgressCallback& progress_callback) = 0;

  
  
  
  
  
  
  virtual google_apis::CancelCallback ResumeUploadFile(
      const GURL& upload_location,
      const base::FilePath& local_file_path,
      const std::string& content_type,
      const UploadCompletionCallback& callback,
      const google_apis::ProgressCallback& progress_callback) = 0;
};

class DriveUploader : public DriveUploaderInterface {
 public:
  DriveUploader(DriveServiceInterface* drive_service,
                base::TaskRunner* blocking_task_runner);
  virtual ~DriveUploader();

  
  virtual google_apis::CancelCallback UploadNewFile(
      const std::string& parent_resource_id,
      const base::FilePath& local_file_path,
      const std::string& title,
      const std::string& content_type,
      const UploadCompletionCallback& callback,
      const google_apis::ProgressCallback& progress_callback) OVERRIDE;
  virtual google_apis::CancelCallback UploadExistingFile(
      const std::string& resource_id,
      const base::FilePath& local_file_path,
      const std::string& content_type,
      const std::string& etag,
      const UploadCompletionCallback& callback,
      const google_apis::ProgressCallback& progress_callback) OVERRIDE;
  virtual google_apis::CancelCallback ResumeUploadFile(
      const GURL& upload_location,
      const base::FilePath& local_file_path,
      const std::string& content_type,
      const UploadCompletionCallback& callback,
      const google_apis::ProgressCallback& progress_callback) OVERRIDE;

 private:
  struct UploadFileInfo;
  typedef base::Callback<void(scoped_ptr<UploadFileInfo> upload_file_info)>
      StartInitiateUploadCallback;

  
  google_apis::CancelCallback StartUploadFile(
      scoped_ptr<UploadFileInfo> upload_file_info,
      const StartInitiateUploadCallback& start_initiate_upload_callback);
  void StartUploadFileAfterGetFileSize(
      scoped_ptr<UploadFileInfo> upload_file_info,
      const StartInitiateUploadCallback& start_initiate_upload_callback,
      bool get_file_size_result);

  
  
  void StartInitiateUploadNewFile(
      const std::string& parent_resource_id,
      const std::string& title,
      scoped_ptr<UploadFileInfo> upload_file_info);

  
  
  void StartInitiateUploadExistingFile(
      const std::string& resource_id,
      const std::string& etag,
      scoped_ptr<UploadFileInfo> upload_file_info);

  
  void OnUploadLocationReceived(scoped_ptr<UploadFileInfo> upload_file_info,
                                google_apis::GDataErrorCode code,
                                const GURL& upload_location);

  
  
  void StartGetUploadStatus(scoped_ptr<UploadFileInfo> upload_file_info);

  
  void UploadNextChunk(scoped_ptr<UploadFileInfo> upload_file_info);

  
  void OnUploadRangeResponseReceived(
      scoped_ptr<UploadFileInfo> upload_file_info,
      const google_apis::UploadRangeResponse& response,
      scoped_ptr<google_apis::ResourceEntry> entry);
  void OnUploadProgress(const google_apis::ProgressCallback& callback,
                        int64 start_position,
                        int64 total_size,
                        int64 progress_of_chunk,
                        int64 total_of_chunk);

  
  void UploadFailed(scoped_ptr<UploadFileInfo> upload_file_info,
                    google_apis::GDataErrorCode error);

  
  
  DriveServiceInterface* drive_service_;  

  scoped_refptr<base::TaskRunner> blocking_task_runner_;

  
  
  base::WeakPtrFactory<DriveUploader> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(DriveUploader);
};

}  

#endif  
