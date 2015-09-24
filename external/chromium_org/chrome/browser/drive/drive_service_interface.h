// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DRIVE_DRIVE_SERVICE_INTERFACE_H_
#define CHROME_BROWSER_DRIVE_DRIVE_SERVICE_INTERFACE_H_

#include <string>

#include "google_apis/drive/auth_service_interface.h"
#include "google_apis/drive/base_requests.h"
#include "google_apis/drive/drive_common_callbacks.h"

namespace base {
class Time;
}

namespace drive {

typedef base::Callback<std::string(
    const std::string& resource_id)> ResourceIdCanonicalizer;

class DriveServiceObserver {
 public:
  
  virtual void OnReadyToSendRequests() {}

  
  virtual void OnRefreshTokenInvalid() {}

 protected:
  virtual ~DriveServiceObserver() {}
};

class DriveServiceInterface {
 public:
  virtual ~DriveServiceInterface() {}

  

  
  virtual void Initialize(const std::string& account_id) = 0;

  
  virtual void AddObserver(DriveServiceObserver* observer) = 0;

  
  virtual void RemoveObserver(DriveServiceObserver* observer) = 0;

  
  virtual bool CanSendRequest() const = 0;

  
  
  virtual ResourceIdCanonicalizer GetResourceIdCanonicalizer() const = 0;

  

  
  virtual bool HasAccessToken() const = 0;

  
  virtual void RequestAccessToken(
      const google_apis::AuthStatusCallback& callback) = 0;

  
  virtual bool HasRefreshToken() const = 0;

  
  virtual void ClearAccessToken() = 0;

  
  virtual void ClearRefreshToken() = 0;

  

  
  virtual std::string GetRootResourceId() const = 0;

  
  
  
  
  
  
  
  virtual google_apis::CancelCallback GetAllResourceList(
      const google_apis::GetResourceListCallback& callback) = 0;

  
  
  
  
  
  
  
  
  virtual google_apis::CancelCallback GetResourceListInDirectory(
      const std::string& directory_resource_id,
      const google_apis::GetResourceListCallback& callback) = 0;

  
  
  
  
  
  
  
  
  virtual google_apis::CancelCallback Search(
      const std::string& search_query,
      const google_apis::GetResourceListCallback& callback) = 0;

  
  
  
  
  
  
  
  
  
  virtual google_apis::CancelCallback SearchByTitle(
      const std::string& title,
      const std::string& directory_resource_id,
      const google_apis::GetResourceListCallback& callback) = 0;

  
  
  
  
  
  
  
  virtual google_apis::CancelCallback GetChangeList(
      int64 start_changestamp,
      const google_apis::GetResourceListCallback& callback) = 0;

  
  
  
  
  
  
  virtual google_apis::CancelCallback GetRemainingChangeList(
      const GURL& next_link,
      const google_apis::GetResourceListCallback& callback) = 0;

  
  
  
  
  
  
  virtual google_apis::CancelCallback GetRemainingFileList(
      const GURL& next_link,
      const google_apis::GetResourceListCallback& callback) = 0;

  
  
  
  
  virtual google_apis::CancelCallback GetResourceEntry(
      const std::string& resource_id,
      const google_apis::GetResourceEntryCallback& callback) = 0;

  
  
  
  
  virtual google_apis::CancelCallback GetShareUrl(
      const std::string& resource_id,
      const GURL& embed_origin,
      const google_apis::GetShareUrlCallback& callback) = 0;

  
  
  
  virtual google_apis::CancelCallback GetAboutResource(
      const google_apis::AboutResourceCallback& callback) = 0;

  
  
  
  virtual google_apis::CancelCallback GetAppList(
      const google_apis::AppListCallback& callback) = 0;

  
  
  
  
  
  virtual google_apis::CancelCallback DeleteResource(
      const std::string& resource_id,
      const std::string& etag,
      const google_apis::EntryActionCallback& callback) = 0;

  
  
  
  virtual google_apis::CancelCallback TrashResource(
      const std::string& resource_id,
      const google_apis::EntryActionCallback& callback) = 0;

  
  
  
  
  
  
  
  
  
  virtual google_apis::CancelCallback CopyResource(
      const std::string& resource_id,
      const std::string& parent_resource_id,
      const std::string& new_title,
      const base::Time& last_modified,
      const google_apis::GetResourceEntryCallback& callback) = 0;

  
  
  
  
  
  
  
  
  virtual google_apis::CancelCallback UpdateResource(
      const std::string& resource_id,
      const std::string& parent_resource_id,
      const std::string& new_title,
      const base::Time& last_modified,
      const base::Time& last_viewed_by_me,
      const google_apis::GetResourceEntryCallback& callback) = 0;

  
  
  
  
  virtual google_apis::CancelCallback RenameResource(
      const std::string& resource_id,
      const std::string& new_title,
      const google_apis::EntryActionCallback& callback) = 0;

  
  
  
  
  virtual google_apis::CancelCallback AddResourceToDirectory(
      const std::string& parent_resource_id,
      const std::string& resource_id,
      const google_apis::EntryActionCallback& callback) = 0;

  
  
  
  
  virtual google_apis::CancelCallback RemoveResourceFromDirectory(
      const std::string& parent_resource_id,
      const std::string& resource_id,
      const google_apis::EntryActionCallback& callback) = 0;

  
  
  
  
  
  
  
  
  virtual google_apis::CancelCallback AddNewDirectory(
      const std::string& parent_resource_id,
      const std::string& directory_title,
      const google_apis::GetResourceEntryCallback& callback) = 0;

  
  
  
  
  
  
  
  
  
  
  
  virtual google_apis::CancelCallback DownloadFile(
      const base::FilePath& local_cache_path,
      const std::string& resource_id,
      const google_apis::DownloadActionCallback& download_action_callback,
      const google_apis::GetContentCallback& get_content_callback,
      const google_apis::ProgressCallback& progress_callback) = 0;

  
  
  
  
  virtual google_apis::CancelCallback InitiateUploadNewFile(
      const std::string& content_type,
      int64 content_length,
      const std::string& parent_resource_id,
      const std::string& title,
      const google_apis::InitiateUploadCallback& callback) = 0;

  
  
  
  
  virtual google_apis::CancelCallback InitiateUploadExistingFile(
      const std::string& content_type,
      int64 content_length,
      const std::string& resource_id,
      const std::string& etag,
      const google_apis::InitiateUploadCallback& callback) = 0;

  
  
  virtual google_apis::CancelCallback ResumeUpload(
      const GURL& upload_url,
      int64 start_position,
      int64 end_position,
      int64 content_length,
      const std::string& content_type,
      const base::FilePath& local_file_path,
      const google_apis::UploadRangeCallback& callback,
      const google_apis::ProgressCallback& progress_callback) = 0;

  
  
  
  
  virtual google_apis::CancelCallback GetUploadStatus(
      const GURL& upload_url,
      int64 content_length,
      const google_apis::UploadRangeCallback& callback) = 0;

  
  
  
  virtual google_apis::CancelCallback AuthorizeApp(
      const std::string& resource_id,
      const std::string& app_id,
      const google_apis::AuthorizeAppCallback& callback) = 0;

  
  
  
  
  
  
  
  
  
  virtual google_apis::CancelCallback GetResourceListInDirectoryByWapi(
      const std::string& directory_resource_id,
      const google_apis::GetResourceListCallback& callback) = 0;

  
  
  
  
  virtual google_apis::CancelCallback GetRemainingResourceList(
      const GURL& next_link,
      const google_apis::GetResourceListCallback& callback) = 0;
};

}  

#endif  
