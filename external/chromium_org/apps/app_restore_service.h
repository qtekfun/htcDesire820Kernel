// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef APPS_APP_RESTORE_SERVICE_H_
#define APPS_APP_RESTORE_SERVICE_H_

#include <string>
#include <vector>

#include "apps/app_lifetime_monitor.h"
#include "apps/shell_window_registry.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"

namespace extensions {
class Extension;
}

class Profile;

namespace apps {

class AppRestoreService : public BrowserContextKeyedService,
                          public AppLifetimeMonitor::Observer {
 public:
  
  
  static bool ShouldRestoreApps(bool is_browser_restart);

  explicit AppRestoreService(Profile* profile);

  
  
  void HandleStartup(bool should_restore_apps);

  
  
  bool IsAppRestorable(const std::string& extension_id);

  static AppRestoreService* Get(Profile* profile);

 private:
  
  virtual void OnAppStart(Profile* profile, const std::string& app_id) OVERRIDE;
  virtual void OnAppActivated(Profile* profile,
                              const std::string& app_id) OVERRIDE;
  virtual void OnAppDeactivated(Profile* profile,
                                const std::string& app_id) OVERRIDE;
  virtual void OnAppStop(Profile* profile, const std::string& app_id) OVERRIDE;
  virtual void OnChromeTerminating() OVERRIDE;

  
  virtual void Shutdown() OVERRIDE;

  void RecordAppStart(const std::string& extension_id);
  void RecordAppStop(const std::string& extension_id);
  void RecordAppActiveState(const std::string& id, bool is_active);

  void RestoreApp(const extensions::Extension* extension);

  void StartObservingAppLifetime();
  void StopObservingAppLifetime();

  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(AppRestoreService);
};

}  

#endif  
