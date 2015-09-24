// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_MANAGEMENT_MANAGEMENT_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_MANAGEMENT_MANAGEMENT_API_H_

#include "base/compiler_specific.h"
#include "chrome/browser/extensions/api/profile_keyed_api_factory.h"
#include "chrome/browser/extensions/chrome_extension_function.h"
#include "chrome/browser/extensions/extension_install_prompt.h"
#include "chrome/browser/extensions/extension_uninstall_dialog.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "extensions/browser/event_router.h"

class ExtensionService;
class ExtensionUninstallDialog;

namespace extensions {

class ManagementFunction : public ChromeSyncExtensionFunction {
 protected:
  virtual ~ManagementFunction() {}

  ExtensionService* service();
};

class AsyncManagementFunction : public ChromeAsyncExtensionFunction {
 protected:
  virtual ~AsyncManagementFunction() {}

  ExtensionService* service();
};

class ManagementGetAllFunction : public ManagementFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("management.getAll", MANAGEMENT_GETALL)

 protected:
  virtual ~ManagementGetAllFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class ManagementGetFunction : public ManagementFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("management.get", MANAGEMENT_GET)

 protected:
  virtual ~ManagementGetFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class ManagementGetPermissionWarningsByIdFunction : public ManagementFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("management.getPermissionWarningsById",
                             MANAGEMENT_GETPERMISSIONWARNINGSBYID)

 protected:
  virtual ~ManagementGetPermissionWarningsByIdFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class ManagementGetPermissionWarningsByManifestFunction
    : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION(
      "management.getPermissionWarningsByManifest",
      MANAGEMENT_GETPERMISSIONWARNINGSBYMANIFEST);

  
  void OnParseSuccess(scoped_ptr<base::DictionaryValue> parsed_manifest);
  void OnParseFailure(const std::string& error);

 protected:
  virtual ~ManagementGetPermissionWarningsByManifestFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class ManagementLaunchAppFunction : public ManagementFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("management.launchApp", MANAGEMENT_LAUNCHAPP)

 protected:
  virtual ~ManagementLaunchAppFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class ManagementSetEnabledFunction : public AsyncManagementFunction,
                           public ExtensionInstallPrompt::Delegate {
 public:
  DECLARE_EXTENSION_FUNCTION("management.setEnabled", MANAGEMENT_SETENABLED)

  ManagementSetEnabledFunction();

 protected:
  virtual ~ManagementSetEnabledFunction();

  
  virtual bool RunImpl() OVERRIDE;

  
  virtual void InstallUIProceed() OVERRIDE;
  virtual void InstallUIAbort(bool user_initiated) OVERRIDE;

 private:
  std::string extension_id_;

  
  scoped_ptr<ExtensionInstallPrompt> install_prompt_;
};

class ManagementUninstallFunctionBase : public AsyncManagementFunction,
                          public ExtensionUninstallDialog::Delegate {
 public:
  ManagementUninstallFunctionBase();

  static void SetAutoConfirmForTest(bool should_proceed);

  
  virtual void ExtensionUninstallAccepted() OVERRIDE;
  virtual void ExtensionUninstallCanceled() OVERRIDE;

 protected:
  virtual ~ManagementUninstallFunctionBase();

  bool Uninstall(const std::string& extension_id, bool show_confirm_dialog);
 private:

  
  
  
  void Finish(bool should_uninstall);

  std::string extension_id_;
  scoped_ptr<ExtensionUninstallDialog> extension_uninstall_dialog_;
};

class ManagementUninstallFunction : public ManagementUninstallFunctionBase {
 public:
  DECLARE_EXTENSION_FUNCTION("management.uninstall", MANAGEMENT_UNINSTALL)

  ManagementUninstallFunction();

 private:
  virtual ~ManagementUninstallFunction();

  virtual bool RunImpl() OVERRIDE;
};

class ManagementUninstallSelfFunction : public ManagementUninstallFunctionBase {
 public:
  DECLARE_EXTENSION_FUNCTION("management.uninstallSelf",
      MANAGEMENT_UNINSTALLSELF);

  ManagementUninstallSelfFunction();

 private:
  virtual ~ManagementUninstallSelfFunction();

  virtual bool RunImpl() OVERRIDE;
};

class ManagementEventRouter : public content::NotificationObserver {
 public:
  explicit ManagementEventRouter(Profile* profile);
  virtual ~ManagementEventRouter();

 private:
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  content::NotificationRegistrar registrar_;

  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(ManagementEventRouter);
};

class ManagementAPI : public ProfileKeyedAPI,
                      public extensions::EventRouter::Observer {
 public:
  explicit ManagementAPI(Profile* profile);
  virtual ~ManagementAPI();

  
  virtual void Shutdown() OVERRIDE;

  
  static ProfileKeyedAPIFactory<ManagementAPI>* GetFactoryInstance();

  
  virtual void OnListenerAdded(const extensions::EventListenerInfo& details)
      OVERRIDE;

 private:
  friend class ProfileKeyedAPIFactory<ManagementAPI>;

  Profile* profile_;

  
  static const char* service_name() {
    return "ManagementAPI";
  }
  static const bool kServiceIsNULLWhileTesting = true;

  
  scoped_ptr<ManagementEventRouter> management_event_router_;

  DISALLOW_COPY_AND_ASSIGN(ManagementAPI);
};

}  

#endif  
