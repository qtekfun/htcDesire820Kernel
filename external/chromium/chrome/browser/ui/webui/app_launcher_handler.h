// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_APP_LAUNCHER_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_APP_LAUNCHER_HANDLER_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/extension_install_ui.h"
#include "chrome/browser/extensions/extension_uninstall_dialog.h"
#include "chrome/browser/prefs/pref_change_registrar.h"
#include "chrome/common/extensions/extension.h"
#include "chrome/common/extensions/extension_constants.h"
#include "content/browser/webui/web_ui.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class ExtensionPrefs;
class ExtensionService;
class NotificationRegistrar;
class PrefChangeRegistrar;
class Profile;

namespace gfx {
class Rect;
}

class AppLauncherHandler : public WebUIMessageHandler,
                           public ExtensionUninstallDialog::Delegate,
                           public ExtensionInstallUI::Delegate,
                           public NotificationObserver {
 public:
  explicit AppLauncherHandler(ExtensionService* extension_service);
  virtual ~AppLauncherHandler();

  
  static void CreateAppInfo(const Extension* extension,
                            ExtensionPrefs* extension_prefs,
                            DictionaryValue* value);

  
  static bool HandlePing(Profile* profile, const std::string& path);

  
  virtual WebUIMessageHandler* Attach(WebUI* web_ui);
  virtual void RegisterMessages();

  
  virtual void Observe(NotificationType type,
                      const NotificationSource& source,
                      const NotificationDetails& details);

  
  void FillAppDictionary(DictionaryValue* value);

  
  void FillPromoDictionary(DictionaryValue* value);

  
  void HandleGetApps(const ListValue* args);

  
  void HandleLaunchApp(const ListValue* args);

  
  void HandleSetLaunchType(const ListValue* args);

  
  void HandleUninstallApp(const ListValue* args);

  
  void HandleHideAppsPromo(const ListValue* args);

  
  void HandleCreateAppShortcut(const ListValue* args);

  
  void HandleReorderApps(const ListValue* args);

  
  void HandleSetPageIndex(const ListValue* args);

  
  void HandlePromoSeen(const ListValue* args);

 private:
  
  
  static void RecordWebStoreLaunch(bool promo_active);

  
  
  static void RecordAppLaunchByID(bool promo_active,
                                  extension_misc::AppLaunchBucket bucket);

  
  
  static void RecordAppLaunchByURL(Profile* profile,
                                   std::string escaped_url,
                                   extension_misc::AppLaunchBucket bucket);

  
  void PromptToEnableApp(const std::string& extension_id);

  
  virtual void ExtensionDialogAccepted();
  virtual void ExtensionDialogCanceled();

  
  virtual void InstallUIProceed();
  virtual void InstallUIAbort();

  
  
  ExtensionUninstallDialog* GetExtensionUninstallDialog();

  
  
  ExtensionInstallUI* GetExtensionInstallUI();

  
  void UninstallDefaultApps();

  
  scoped_refptr<ExtensionService> extensions_service_;

  
  
  NotificationRegistrar registrar_;

  
  PrefChangeRegistrar pref_change_registrar_;

  
  scoped_ptr<ExtensionUninstallDialog> extension_uninstall_dialog_;

  
  scoped_ptr<ExtensionInstallUI> extension_install_ui_;

  
  std::string extension_id_prompting_;

  
  bool promo_active_;

  
  
  bool ignore_changes_;

  DISALLOW_COPY_AND_ASSIGN(AppLauncherHandler);
};

#endif  
