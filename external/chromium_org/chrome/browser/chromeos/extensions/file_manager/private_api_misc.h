// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_EXTENSIONS_FILE_MANAGER_PRIVATE_API_MISC_H_
#define CHROME_BROWSER_CHROMEOS_EXTENSIONS_FILE_MANAGER_PRIVATE_API_MISC_H_

#include "chrome/browser/chromeos/extensions/file_manager/private_api_base.h"
#include "chrome/browser/chromeos/file_manager/zip_file_creator.h"
#include "google_apis/drive/gdata_errorcode.h"

namespace google_apis {
class AuthServiceInterface;
}

namespace extensions {

class FileBrowserPrivateLogoutUserForReauthenticationFunction
    : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.logoutUserForReauthentication",
                             FILEBROWSERPRIVATE_LOGOUTUSERFORREAUTHENTICATION)

 protected:
  virtual ~FileBrowserPrivateLogoutUserForReauthenticationFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class FileBrowserPrivateGetPreferencesFunction
    : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.getPreferences",
                             FILEBROWSERPRIVATE_GETPREFERENCES)

 protected:
  virtual ~FileBrowserPrivateGetPreferencesFunction() {}

  virtual bool RunImpl() OVERRIDE;
};

class FileBrowserPrivateSetPreferencesFunction
    : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.setPreferences",
                             FILEBROWSERPRIVATE_SETPREFERENCES)

 protected:
  virtual ~FileBrowserPrivateSetPreferencesFunction() {}

  virtual bool RunImpl() OVERRIDE;
};

class FileBrowserPrivateZipSelectionFunction
    : public LoggedAsyncExtensionFunction,
      public file_manager::ZipFileCreator::Observer {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.zipSelection",
                             FILEBROWSERPRIVATE_ZIPSELECTION)

  FileBrowserPrivateZipSelectionFunction();

 protected:
  virtual ~FileBrowserPrivateZipSelectionFunction();

  
  virtual bool RunImpl() OVERRIDE;

  
  virtual void OnZipDone(bool success) OVERRIDE;

 private:
  scoped_refptr<file_manager::ZipFileCreator> zip_file_creator_;
};

class FileBrowserPrivateZoomFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.zoom",
                             FILEBROWSERPRIVATE_ZOOM);

 protected:
  virtual ~FileBrowserPrivateZoomFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class FileBrowserPrivateInstallWebstoreItemFunction
    : public LoggedAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.installWebstoreItem",
                             FILEBROWSERPRIVATE_INSTALLWEBSTOREITEM);

 protected:
  virtual ~FileBrowserPrivateInstallWebstoreItemFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
  void OnInstallComplete(bool success, const std::string& error);

 private:
  std::string webstore_item_id_;
};

class FileBrowserPrivateRequestWebStoreAccessTokenFunction
    : public LoggedAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileBrowserPrivate.requestWebStoreAccessToken",
                             FILEBROWSERPRIVATE_REQUESTWEBSTOREACCESSTOKEN);

  FileBrowserPrivateRequestWebStoreAccessTokenFunction();

 protected:
  virtual ~FileBrowserPrivateRequestWebStoreAccessTokenFunction();
  virtual bool RunImpl() OVERRIDE;

 private:
  scoped_ptr<google_apis::AuthServiceInterface> auth_service_;

  void OnAccessTokenFetched(google_apis::GDataErrorCode code,
                            const std::string& access_token);

};

}  

#endif  
