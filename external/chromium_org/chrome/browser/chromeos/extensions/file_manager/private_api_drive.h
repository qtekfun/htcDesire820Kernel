// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_EXTENSIONS_FILE_MANAGER_PRIVATE_API_DRIVE_H_
#define CHROME_BROWSER_CHROMEOS_EXTENSIONS_FILE_MANAGER_PRIVATE_API_DRIVE_H_

#include "chrome/browser/chromeos/drive/file_errors.h"
#include "chrome/browser/chromeos/drive/file_system_interface.h"
#include "chrome/browser/chromeos/extensions/file_manager/private_api_base.h"

namespace drive {
class FileCacheEntry;
class ResourceEntry;
struct DriveAppInfo;
struct SearchResultInfo;
}

namespace extensions {
namespace api {
namespace file_browser_private{
struct DriveEntryProperties;
}
}

class FileBrowserPrivateGetDriveEntryPropertiesFunction
    : public LoggedAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.getDriveEntryProperties",
                             FILEBROWSERPRIVATE_GETDRIVEFILEPROPERTIES)

  FileBrowserPrivateGetDriveEntryPropertiesFunction();

 protected:
  virtual ~FileBrowserPrivateGetDriveEntryPropertiesFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  void OnGetFileInfo(drive::FileError error,
                     scoped_ptr<drive::ResourceEntry> entry);

  void CacheStateReceived(bool success,
                          const drive::FileCacheEntry& cache_entry);

  void CompleteGetFileProperties(drive::FileError error);

  base::FilePath file_path_;
  scoped_ptr<extensions::api::file_browser_private::
             DriveEntryProperties> properties_;
};

class FileBrowserPrivatePinDriveFileFunction
    : public LoggedAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.pinDriveFile",
                             FILEBROWSERPRIVATE_PINDRIVEFILE)

 protected:
  virtual ~FileBrowserPrivatePinDriveFileFunction() {}

  
  virtual bool RunImpl() OVERRIDE;

 private:
  
  void OnPinStateSet(drive::FileError error);
};

class FileBrowserPrivateGetDriveFilesFunction
    : public LoggedAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.getDriveFiles",
                             FILEBROWSERPRIVATE_GETDRIVEFILES)

  FileBrowserPrivateGetDriveFilesFunction();

 protected:
  virtual ~FileBrowserPrivateGetDriveFilesFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  
  
  void GetFileOrSendResponse();

  
  
  void OnFileReady(drive::FileError error,
                   const base::FilePath& local_path,
                   scoped_ptr<drive::ResourceEntry> entry);

  std::queue<base::FilePath> remaining_drive_paths_;
  std::vector<std::string> local_paths_;
};

class FileBrowserPrivateCancelFileTransfersFunction
    : public LoggedAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.cancelFileTransfers",
                             FILEBROWSERPRIVATE_CANCELFILETRANSFERS)

 protected:
  virtual ~FileBrowserPrivateCancelFileTransfersFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class FileBrowserPrivateSearchDriveFunction
    : public LoggedAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.searchDrive",
                             FILEBROWSERPRIVATE_SEARCHDRIVE)

 protected:
  virtual ~FileBrowserPrivateSearchDriveFunction() {}

  virtual bool RunImpl() OVERRIDE;

 private:
  
  void OnSearch(drive::FileError error,
                const GURL& next_link,
                scoped_ptr<std::vector<drive::SearchResultInfo> > result_paths);
};

class FileBrowserPrivateSearchDriveMetadataFunction
    : public LoggedAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.searchDriveMetadata",
                             FILEBROWSERPRIVATE_SEARCHDRIVEMETADATA)

 protected:
  virtual ~FileBrowserPrivateSearchDriveMetadataFunction() {}

  virtual bool RunImpl() OVERRIDE;

 private:
  
  void OnSearchMetadata(drive::FileError error,
                        scoped_ptr<drive::MetadataSearchResultVector> results);
};

class FileBrowserPrivateGetDriveConnectionStateFunction
    : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION(
      "fileBrowserPrivate.getDriveConnectionState",
      FILEBROWSERPRIVATE_GETDRIVECONNECTIONSTATE);

 protected:
  virtual ~FileBrowserPrivateGetDriveConnectionStateFunction() {}

  virtual bool RunImpl() OVERRIDE;
};

class FileBrowserPrivateRequestAccessTokenFunction
    : public LoggedAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.requestAccessToken",
                             FILEBROWSERPRIVATE_REQUESTACCESSTOKEN)

 protected:
  virtual ~FileBrowserPrivateRequestAccessTokenFunction() {}

  
  virtual bool RunImpl() OVERRIDE;

  
  void OnAccessTokenFetched(google_apis::GDataErrorCode code,
                            const std::string& access_token);
};

class FileBrowserPrivateGetShareUrlFunction
    : public LoggedAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.getShareUrl",
                             FILEBROWSERPRIVATE_GETSHAREURL)

 protected:
  virtual ~FileBrowserPrivateGetShareUrlFunction() {}

  
  virtual bool RunImpl() OVERRIDE;

  
  
  void OnGetShareUrl(drive::FileError error, const GURL& share_url);
};

}  

#endif  
