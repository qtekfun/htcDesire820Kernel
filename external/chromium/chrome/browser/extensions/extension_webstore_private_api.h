// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_WEBSTORE_PRIVATE_API_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_WEBSTORE_PRIVATE_API_H_
#pragma once

#include <string>

#include "chrome/browser/browser_signin.h"
#include "chrome/browser/extensions/extension_function.h"
#include "chrome/browser/extensions/extension_install_ui.h"
#include "chrome/common/net/gaia/google_service_auth_error.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class ProfileSyncService;

class WebstorePrivateApi {
 public:
  
  
  static void SetTestingProfileSyncService(ProfileSyncService* service);

  
  
  static void SetTestingBrowserSignin(BrowserSignin* signin);
};

class BeginInstallFunction : public SyncExtensionFunction {
 public:
  
  
  static void SetIgnoreUserGestureForTests(bool ignore);
 protected:
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("webstorePrivate.beginInstall");
};

class BeginInstallWithManifestFunction : public AsyncExtensionFunction,
                                         public ExtensionInstallUI::Delegate {
 public:
  BeginInstallWithManifestFunction();

  
  
  
  enum ResultCode {
    ERROR_NONE = 0,

    
    UNKNOWN_ERROR,

    
    USER_CANCELLED,

    
    MANIFEST_ERROR,

    
    ICON_ERROR,

    
    INVALID_ID,

    
    PERMISSION_DENIED,

    
    NO_GESTURE,
  };

  
  
  static void SetIgnoreUserGestureForTests(bool ignore);

  
  
  void OnParseSuccess(const SkBitmap& icon, DictionaryValue* parsed_manifest);

  
  
  void OnParseFailure(ResultCode result_code, const std::string& error_message);

  
  virtual void InstallUIProceed() OVERRIDE;
  virtual void InstallUIAbort() OVERRIDE;

 protected:
  virtual ~BeginInstallWithManifestFunction();
  virtual bool RunImpl();

  
  void SetResult(ResultCode code);

 private:
  
  std::string id_;
  std::string manifest_;
  std::string icon_data_;

  
  scoped_ptr<DictionaryValue> parsed_manifest_;
  SkBitmap icon_;

  
  
  scoped_refptr<Extension> dummy_extension_;
  DECLARE_EXTENSION_FUNCTION_NAME("webstorePrivate.beginInstallWithManifest");
};

class CompleteInstallFunction : public SyncExtensionFunction {
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("webstorePrivate.completeInstall");
};

class GetBrowserLoginFunction : public SyncExtensionFunction {
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("webstorePrivate.getBrowserLogin");
};

class GetStoreLoginFunction : public SyncExtensionFunction {
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("webstorePrivate.getStoreLogin");
};

class SetStoreLoginFunction : public SyncExtensionFunction {
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("webstorePrivate.setStoreLogin");
};

class PromptBrowserLoginFunction : public AsyncExtensionFunction,
                                   public NotificationObserver,
                                   public BrowserSignin::SigninDelegate {
 public:
  PromptBrowserLoginFunction();
  
  virtual void OnLoginSuccess();
  virtual void OnLoginFailure(const GoogleServiceAuthError& error);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 protected:
  virtual ~PromptBrowserLoginFunction();
  virtual bool RunImpl();

 private:
  
  virtual string16 GetLoginMessage();

  
  bool waiting_for_token_;

  
  NotificationRegistrar registrar_;

  DECLARE_EXTENSION_FUNCTION_NAME("webstorePrivate.promptBrowserLogin");
};

#endif  
