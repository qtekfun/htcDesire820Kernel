// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_WEBSTORE_PRIVATE_WEBSTORE_PRIVATE_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_WEBSTORE_PRIVATE_WEBSTORE_PRIVATE_API_H_

#include <string>

#include "chrome/browser/extensions/bundle_installer.h"
#include "chrome/browser/extensions/chrome_extension_function.h"
#include "chrome/browser/extensions/extension_install_prompt.h"
#include "chrome/browser/extensions/webstore_install_helper.h"
#include "chrome/browser/extensions/webstore_installer.h"
#include "chrome/browser/signin/signin_tracker.h"
#include "chrome/common/extensions/api/webstore_private.h"
#include "content/public/browser/gpu_data_manager_observer.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "google_apis/gaia/google_service_auth_error.h"
#include "third_party/skia/include/core/SkBitmap.h"

class ProfileSyncService;

namespace content {
class GpuDataManager;
}

class GPUFeatureChecker;

namespace extensions {

class WebstorePrivateApi {
 public:
  
  static void SetWebstoreInstallerDelegateForTesting(
      WebstoreInstaller::Delegate* delegate);

  
  
  
  static scoped_ptr<WebstoreInstaller::Approval> PopApprovalForTesting(
      Profile* profile, const std::string& extension_id);
};

class WebstorePrivateInstallBundleFunction
    : public ChromeAsyncExtensionFunction,
      public extensions::BundleInstaller::Delegate {
 public:
  DECLARE_EXTENSION_FUNCTION("webstorePrivate.installBundle",
                             WEBSTOREPRIVATE_INSTALLBUNDLE)

  WebstorePrivateInstallBundleFunction();

  
  virtual void OnBundleInstallApproved() OVERRIDE;
  virtual void OnBundleInstallCanceled(bool user_initiated) OVERRIDE;
  virtual void OnBundleInstallCompleted() OVERRIDE;

 protected:
  virtual ~WebstorePrivateInstallBundleFunction();

  
  virtual bool RunImpl() OVERRIDE;

  
  bool ReadBundleInfo(
      const api::webstore_private::InstallBundle::Params& details,
          extensions::BundleInstaller::ItemList* items);

 private:
  scoped_refptr<extensions::BundleInstaller> bundle_;
};

class WebstorePrivateBeginInstallWithManifest3Function
    : public ChromeAsyncExtensionFunction,
      public ExtensionInstallPrompt::Delegate,
      public WebstoreInstallHelper::Delegate,
      public SigninTracker::Observer {
 public:
  DECLARE_EXTENSION_FUNCTION("webstorePrivate.beginInstallWithManifest3",
                             WEBSTOREPRIVATE_BEGININSTALLWITHMANIFEST3)

  
  
  
  enum ResultCode {
    ERROR_NONE = 0,

    
    UNKNOWN_ERROR,

    
    USER_CANCELLED,

    
    MANIFEST_ERROR,

    
    ICON_ERROR,

    
    INVALID_ID,

    
    PERMISSION_DENIED,

    
    INVALID_ICON_URL,

    
    SIGNIN_FAILED,

    
    ALREADY_INSTALLED,
  };

  WebstorePrivateBeginInstallWithManifest3Function();

  
  virtual void OnWebstoreParseSuccess(
      const std::string& id,
      const SkBitmap& icon,
      base::DictionaryValue* parsed_manifest) OVERRIDE;
  virtual void OnWebstoreParseFailure(
      const std::string& id,
      InstallHelperResultCode result_code,
      const std::string& error_message) OVERRIDE;

  
  virtual void InstallUIProceed() OVERRIDE;
  virtual void InstallUIAbort(bool user_initiated) OVERRIDE;

 protected:
  virtual ~WebstorePrivateBeginInstallWithManifest3Function();

  
  virtual bool RunImpl() OVERRIDE;

  
  void SetResultCode(ResultCode code);

 private:
  
  virtual void SigninFailed(const GoogleServiceAuthError& error) OVERRIDE;
  virtual void SigninSuccess() OVERRIDE;

  
  void SigninCompletedOrNotNeeded();

  const char* ResultCodeToString(ResultCode code);

  
  scoped_ptr<api::webstore_private::BeginInstallWithManifest3::Params> params_;

  
  scoped_ptr<base::DictionaryValue> parsed_manifest_;
  SkBitmap icon_;

  
  
  scoped_refptr<extensions::Extension> dummy_extension_;

  
  scoped_ptr<ExtensionInstallPrompt> install_prompt_;

  scoped_ptr<SigninTracker> signin_tracker_;
};

class WebstorePrivateCompleteInstallFunction
    : public ChromeAsyncExtensionFunction,
      public WebstoreInstaller::Delegate {
 public:
  DECLARE_EXTENSION_FUNCTION("webstorePrivate.completeInstall",
                             WEBSTOREPRIVATE_COMPLETEINSTALL)

  WebstorePrivateCompleteInstallFunction();

  
  virtual void OnExtensionInstallSuccess(const std::string& id) OVERRIDE;
  virtual void OnExtensionInstallFailure(
      const std::string& id,
      const std::string& error,
      WebstoreInstaller::FailureReason reason) OVERRIDE;

 protected:
  virtual ~WebstorePrivateCompleteInstallFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  scoped_ptr<WebstoreInstaller::Approval> approval_;
};

class WebstorePrivateEnableAppLauncherFunction
    : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("webstorePrivate.enableAppLauncher",
                             WEBSTOREPRIVATE_ENABLEAPPLAUNCHER)

  WebstorePrivateEnableAppLauncherFunction();

 protected:
  virtual ~WebstorePrivateEnableAppLauncherFunction();

  
  virtual bool RunImpl() OVERRIDE;
};

class WebstorePrivateGetBrowserLoginFunction
    : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("webstorePrivate.getBrowserLogin",
                             WEBSTOREPRIVATE_GETBROWSERLOGIN)

 protected:
  virtual ~WebstorePrivateGetBrowserLoginFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class WebstorePrivateGetStoreLoginFunction
    : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("webstorePrivate.getStoreLogin",
                             WEBSTOREPRIVATE_GETSTORELOGIN)

 protected:
  virtual ~WebstorePrivateGetStoreLoginFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class WebstorePrivateSetStoreLoginFunction
    : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("webstorePrivate.setStoreLogin",
                             WEBSTOREPRIVATE_SETSTORELOGIN)

 protected:
  virtual ~WebstorePrivateSetStoreLoginFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class WebstorePrivateGetWebGLStatusFunction
    : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("webstorePrivate.getWebGLStatus",
                             WEBSTOREPRIVATE_GETWEBGLSTATUS)

  WebstorePrivateGetWebGLStatusFunction();

 protected:
  virtual ~WebstorePrivateGetWebGLStatusFunction();

  void OnFeatureCheck(bool feature_allowed);

  
  virtual bool RunImpl() OVERRIDE;

 private:
  void CreateResult(bool webgl_allowed);

  scoped_refptr<GPUFeatureChecker> feature_checker_;
};

class WebstorePrivateGetIsLauncherEnabledFunction
    : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("webstorePrivate.getIsLauncherEnabled",
                             WEBSTOREPRIVATE_GETISLAUNCHERENABLED)

  WebstorePrivateGetIsLauncherEnabledFunction() {}

 protected:
  virtual ~WebstorePrivateGetIsLauncherEnabledFunction() {}

  
  virtual bool RunImpl() OVERRIDE;

 private:
  void OnIsLauncherCheckCompleted(bool is_enabled);
};

class WebstorePrivateIsInIncognitoModeFunction
    : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("webstorePrivate.isInIncognitoMode",
                             WEBSTOREPRIVATE_ISININCOGNITOMODEFUNCTION)

  WebstorePrivateIsInIncognitoModeFunction() {}

 protected:
  virtual ~WebstorePrivateIsInIncognitoModeFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

}  

#endif  
