// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_RUNTIME_RUNTIME_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_RUNTIME_RUNTIME_API_H_

#include <string>

#include "chrome/browser/extensions/chrome_extension_function.h"
#include "chrome/common/extensions/api/runtime.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "extensions/browser/update_observer.h"

class Profile;

namespace base {
class Version;
}

namespace content {
class BrowserContext;
}

namespace extensions {
class Extension;
class ExtensionHost;

class RuntimeAPI : public BrowserContextKeyedService,
                   public content::NotificationObserver,
                   public extensions::UpdateObserver {
 public:
  explicit RuntimeAPI(content::BrowserContext* context);
  virtual ~RuntimeAPI();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  void OnExtensionsReady();
  void OnExtensionLoaded(const Extension* extension);
  void OnExtensionInstalled(const Extension* extension);
  void OnExtensionUninstalled(const Extension* extension);

  
  virtual void OnAppUpdateAvailable(const Extension* extension) OVERRIDE;
  virtual void OnChromeUpdateAvailable() OVERRIDE;

  content::BrowserContext* browser_context_;

  
  
  bool dispatch_chrome_updated_event_;

  
  
  bool registered_for_updates_;

  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(RuntimeAPI);
};

class RuntimeEventRouter {
 public:
  
  static void DispatchOnStartupEvent(content::BrowserContext* context,
                                     const std::string& extension_id);

  
  static void DispatchOnInstalledEvent(content::BrowserContext* context,
                                       const std::string& extension_id,
                                       const base::Version& old_version,
                                       bool chrome_updated);

  
  static void DispatchOnUpdateAvailableEvent(
      Profile* profile,
      const std::string& extension_id,
      const base::DictionaryValue* manifest);

  
  static void DispatchOnBrowserUpdateAvailableEvent(Profile* profile);

  
  static void DispatchOnRestartRequiredEvent(
      Profile* profile,
      const std::string& app_id,
      api::runtime::OnRestartRequired::Reason reason);

  
  static void OnExtensionUninstalled(Profile* profile,
                                     const std::string& extension_id);
};

class RuntimeGetBackgroundPageFunction : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("runtime.getBackgroundPage",
                             RUNTIME_GETBACKGROUNDPAGE)

 protected:
  virtual ~RuntimeGetBackgroundPageFunction() {}
  virtual bool RunImpl() OVERRIDE;

 private:
  void OnPageLoaded(ExtensionHost*);
};

class RuntimeSetUninstallUrlFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("runtime.setUninstallUrl",
                             RUNTIME_SETUNINSTALLURL)

 protected:
  virtual ~RuntimeSetUninstallUrlFunction() {}
  virtual bool RunImpl() OVERRIDE;
};

class RuntimeReloadFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("runtime.reload", RUNTIME_RELOAD)

 protected:
  virtual ~RuntimeReloadFunction() {}
  virtual bool RunImpl() OVERRIDE;
};

class RuntimeRequestUpdateCheckFunction : public ChromeAsyncExtensionFunction,
                                          public content::NotificationObserver {
 public:
  DECLARE_EXTENSION_FUNCTION("runtime.requestUpdateCheck",
                             RUNTIME_REQUESTUPDATECHECK)

  RuntimeRequestUpdateCheckFunction();
 protected:
  virtual ~RuntimeRequestUpdateCheckFunction() {}
  virtual bool RunImpl() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;
 private:
  void CheckComplete();
  void ReplyUpdateFound(const std::string& version);

  content::NotificationRegistrar registrar_;
  bool did_reply_;
};

class RuntimeRestartFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("runtime.restart", RUNTIME_RESTART)

 protected:
  virtual ~RuntimeRestartFunction() {}
  virtual bool RunImpl() OVERRIDE;
};

class RuntimeGetPlatformInfoFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("runtime.getPlatformInfo",
                             RUNTIME_GETPLATFORMINFO);
 protected:
  virtual ~RuntimeGetPlatformInfoFunction() {}
  virtual bool RunImpl() OVERRIDE;
};

class RuntimeGetPackageDirectoryEntryFunction
    : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("runtime.getPackageDirectoryEntry",
                             RUNTIME_GETPACKAGEDIRECTORYENTRY)

 protected:
  virtual ~RuntimeGetPackageDirectoryEntryFunction() {}
  virtual bool RunImpl() OVERRIDE;
};

}  

#endif  
