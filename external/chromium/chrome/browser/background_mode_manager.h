// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BACKGROUND_MODE_MANAGER_H_
#define CHROME_BROWSER_BACKGROUND_MODE_MANAGER_H_
#pragma once

#include "base/gtest_prod_util.h"
#include "chrome/browser/background_application_list_model.h"
#include "chrome/browser/prefs/pref_change_registrar.h"
#include "chrome/browser/profiles/profile_keyed_service.h"
#include "chrome/browser/status_icons/status_icon.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/base/models/simple_menu_model.h"

class Browser;
class CommandLine;
class Extension;
class PrefService;
class Profile;
class StatusIcon;
class StatusTray;

class BackgroundModeManager
    : public NotificationObserver,
      public ui::SimpleMenuModel::Delegate,
      public BackgroundApplicationListModel::Observer,
      public ProfileKeyedService {
 public:
  BackgroundModeManager(Profile* profile, CommandLine* command_line);
  virtual ~BackgroundModeManager();

  static bool IsBackgroundModeEnabled(const CommandLine* command_line);
  static void RegisterPrefs(PrefService* prefs);

 private:
  friend class TestBackgroundModeManager;
  friend class BackgroundModeManagerTest;
  FRIEND_TEST_ALL_PREFIXES(BackgroundModeManagerTest,
                           BackgroundAppLoadUnload);
  FRIEND_TEST_ALL_PREFIXES(BackgroundModeManagerTest,
                           BackgroundAppInstallUninstall);
  FRIEND_TEST_ALL_PREFIXES(BackgroundModeManagerTest,
                           BackgroundPrefDisabled);
  FRIEND_TEST_ALL_PREFIXES(BackgroundModeManagerTest,
                           BackgroundPrefDynamicDisable);
  FRIEND_TEST_ALL_PREFIXES(BackgroundModeManagerTest,
                           BackgroundPrefDynamicEnable);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual bool IsCommandIdChecked(int command_id) const;
  virtual bool IsCommandIdEnabled(int command_id) const;
  virtual bool GetAcceleratorForCommandId(int command_id,
                                          ui::Accelerator* accelerator);
  virtual void ExecuteCommand(int command_id);

  
  virtual void ExecuteApplication(int application_id);

  
  virtual void OnApplicationDataChanged(const Extension* extension);
  virtual void OnApplicationListChanged();

  
  void OnBackgroundAppLoaded();

  
  void OnBackgroundAppUnloaded();

  
  
  
  void OnBackgroundAppInstalled(const Extension* extension);

  
  
  void OnBackgroundAppUninstalled();

  
  
  virtual void EnableLaunchOnStartup(bool should_launch);

  
  
  void DisplayAppInstalledNotification(const Extension* extension);

  
  
  void StartBackgroundMode();

  
  
  void EndBackgroundMode();

  
  
  
  
  void EndKeepAliveForStartup();

  
  
  string16 GetPreferencesMenuLabel();

  
  
  virtual void CreateStatusTrayIcon();

  
  
  virtual void RemoveStatusTrayIcon();

  
  
  
  void UpdateContextMenuEntryIcon(const Extension* extension);

  
  
  
  virtual void UpdateStatusTrayIconContextMenu();

  
  
  
  Browser* GetBrowserWindow();

  NotificationRegistrar registrar_;

  
  Profile* profile_;

  
  BackgroundApplicationListModel applications_;

  
  int background_app_count_;

  
  
  ui::SimpleMenuModel* context_menu_;

  
  
  int context_menu_application_offset_;

  
  
  
  bool in_background_mode_;

  
  
  
  bool keep_alive_for_startup_;

  
  
  StatusTray* status_tray_;

  
  StatusIcon* status_icon_;

  DISALLOW_COPY_AND_ASSIGN(BackgroundModeManager);
};

#endif  
