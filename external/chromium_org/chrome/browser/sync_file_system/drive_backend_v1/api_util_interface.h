// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_DRIVE_BACKEND_V1_API_UTIL_INTERFACE_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_DRIVE_BACKEND_V1_API_UTIL_INTERFACE_H_

#include <string>

#include "base/callback_forward.h"
#include "google_apis/drive/gdata_errorcode.h"
#include "google_apis/drive/gdata_wapi_parser.h"

class GURL;
class Profile;

namespace base {
class Time;
}

namespace google_apis {
class DriveUploaderInterface;
}

namespace webkit_blob {
class ScopedFile;
}

namespace sync_file_system {
namespace drive_backend {

class APIUtilObserver {
 public:
  APIUtilObserver() {}
  virtual ~APIUtilObserver() {}
  virtual void OnAuthenticated() = 0;
  virtual void OnNetworkConnected() = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(APIUtilObserver);
};

class APIUtilInterface {
 public:
  typedef base::Callback<void(google_apis::GDataErrorCode error)>
      GDataErrorCallback;
  typedef base::Callback<void(google_apis::GDataErrorCode error,
                              const std::string& file_md5,
                              int64 file_size,
                              const base::Time& last_updated,
                              webkit_blob::ScopedFile downloaded)>
      DownloadFileCallback;
  typedef base::Callback<void(google_apis::GDataErrorCode error,
                              const std::string& resource_id,
                              const std::string& file_md5)> UploadFileCallback;
  typedef base::Callback<
      void(google_apis::GDataErrorCode error, const std::string& resource_id)>
      ResourceIdCallback;
  typedef base::Callback<void(google_apis::GDataErrorCode error,
                              int64 changestamp)> ChangeStampCallback;
  typedef base::Callback<void(google_apis::GDataErrorCode error,
                              scoped_ptr<google_apis::ResourceList> feed)>
      ResourceListCallback;
  typedef base::Callback<void(google_apis::GDataErrorCode error,
                              scoped_ptr<google_apis::ResourceEntry> entry)>
      ResourceEntryCallback;

  APIUtilInterface() {}
  virtual ~APIUtilInterface() {}

  virtual void AddObserver(APIUtilObserver* observer) = 0;
  virtual void RemoveObserver(APIUtilObserver* observer) = 0;

  
  
  
  
  
  
  
  virtual void GetDriveDirectoryForSyncRoot(
      const ResourceIdCallback& callback) = 0;

  
  
  
  
  
  
  
  virtual void GetDriveDirectoryForOrigin(
      const std::string& sync_root_resource_id,
      const GURL& origin,
      const ResourceIdCallback& callback) = 0;

  
  
  virtual void GetLargestChangeStamp(const ChangeStampCallback& callback) = 0;

  
  
  virtual void GetResourceEntry(const std::string& resource_id,
                                const ResourceEntryCallback& callback) = 0;

  
  
  
  
  
  virtual void ListFiles(const std::string& directory_resource_id,
                         const ResourceListCallback& callback) = 0;

  
  
  
  
  
  virtual void ListChanges(int64 start_changestamp,
                           const ResourceListCallback& callback) = 0;

  
  
  virtual void ContinueListing(const GURL& next_link,
                               const ResourceListCallback& callback) = 0;

  
  
  
  
  
  
  
  virtual void DownloadFile(const std::string& resource_id,
                            const std::string& local_file_md5,
                            const DownloadFileCallback& callback) = 0;

  
  
  
  
  virtual void UploadNewFile(const std::string& directory_resource_id,
                             const base::FilePath& local_file_path,
                             const std::string& title,
                             const UploadFileCallback& callback) = 0;

  
  
  
  
  
  
  virtual void UploadExistingFile(const std::string& resource_id,
                                  const std::string& remote_file_md5,
                                  const base::FilePath& local_file_path,
                                  const UploadFileCallback& callback) = 0;

  
  
  
  
  virtual void CreateDirectory(const std::string& parent_resource_id,
                               const std::string& title,
                               const ResourceIdCallback& callback) = 0;

  
  virtual bool IsAuthenticated() const = 0;

  
  
  
  
  
  
  
  
  
  
  
  virtual void DeleteFile(const std::string& resource_id,
                          const std::string& remote_file_md5,
                          const GDataErrorCallback& callback) = 0;

  
  
  virtual void EnsureSyncRootIsNotInMyDrive(
      const std::string& sync_root_resource_id) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(APIUtilInterface);
};

}  
}  

#endif  
