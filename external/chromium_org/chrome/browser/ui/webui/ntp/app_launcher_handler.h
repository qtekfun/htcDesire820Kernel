// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NTP_APP_LAUNCHER_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_NTP_APP_LAUNCHER_HANDLER_H_

#include <set>
#include <string>

#include "apps/metrics_names.h"
#include "base/memory/scoped_ptr.h"
#include "base/prefs/pref_change_registrar.h"
#include "chrome/browser/extensions/extension_uninstall_dialog.h"
#include "chrome/browser/favicon/favicon_service.h"
#include "chrome/browser/ui/extensions/extension_enable_flow_delegate.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "chrome/common/extensions/extension_constants.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_ui_message_handler.h"
#include "extensions/common/extension.h"
#include "sync/api/string_ordinal.h"

class ExtensionEnableFlow;
class ExtensionService;
class PrefChangeRegistrar;
class Profile;

namespace chrome {
struct FaviconImageResult;
}

class AppLauncherHandler : public content::WebUIMessageHandler,
                           public ExtensionUninstallDialog::Delegate,
                           public ExtensionEnableFlowDelegate,
                           public content::NotificationObserver {
 public:
  explicit AppLauncherHandler(ExtensionService* extension_service);
  virtual ~AppLauncherHandler();

  
  static void CreateAppInfo(
      const extensions::Extension* extension,
      ExtensionService* service,
      base::DictionaryValue* value);

  
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void FillAppDictionary(base::DictionaryValue* value);

  
  
  
  base::DictionaryValue* GetAppInfo(const extensions::Extension* extension);

  
  void FillPromoDictionary(base::DictionaryValue* value);

  
  void HandleGetApps(const base::ListValue* args);

  
  
  
  void HandleLaunchApp(const base::ListValue* args);

  
  
  void HandleSetLaunchType(const base::ListValue* args);

  
  
  
  void HandleUninstallApp(const base::ListValue* args);

  
  
  void HandleCreateAppShortcut(const base::ListValue* args);

  
  
  void HandleReorderApps(const base::ListValue* args);

  
  
  void HandleSetPageIndex(const base::ListValue* args);

  
  
  void HandleSaveAppPageName(const base::ListValue* args);

  
  
  void HandleGenerateAppForLink(const base::ListValue* args);

  
  void StopShowingAppLauncherPromo(const base::ListValue* args);
  void OnLearnMore(const base::ListValue* args);

 private:
  struct AppInstallInfo {
    AppInstallInfo();
    ~AppInstallInfo();

    bool is_bookmark_app;
    base::string16 title;
    GURL app_url;
    syncer::StringOrdinal page_ordinal;
  };

  
  void CleanupAfterUninstall();

  
  void PromptToEnableApp(const std::string& extension_id);

  
  virtual void ExtensionUninstallAccepted() OVERRIDE;
  virtual void ExtensionUninstallCanceled() OVERRIDE;

  
  virtual void ExtensionEnableFlowFinished() OVERRIDE;
  virtual void ExtensionEnableFlowAborted(bool user_initiated) OVERRIDE;

  
  
  ExtensionUninstallDialog* GetExtensionUninstallDialog();

  
  void OnFaviconForApp(scoped_ptr<AppInstallInfo> install_info,
                       const chrome::FaviconImageResult& image_result);

  
  void SetAppToBeHighlighted();

  void OnExtensionPreferenceChanged();

  void OnLocalStatePreferenceChanged();

  
  
  ExtensionService* const extension_service_;

  
  
  content::NotificationRegistrar registrar_;

  
  PrefChangeRegistrar extension_pref_change_registrar_;

  
  PrefChangeRegistrar local_state_pref_change_registrar_;

  
  scoped_ptr<ExtensionUninstallDialog> extension_uninstall_dialog_;

  
  scoped_ptr<ExtensionEnableFlow> extension_enable_flow_;

  
  std::set<std::string> visible_apps_;

  
  
  std::string extension_id_prompting_;

  
  
  bool ignore_changes_;

  
  
  bool attempted_bookmark_app_install_;

  
  bool has_loaded_apps_;

  
  
  
  std::string highlight_app_id_;

  
  CancelableTaskTracker cancelable_task_tracker_;

  DISALLOW_COPY_AND_ASSIGN(AppLauncherHandler);
};

#endif  
