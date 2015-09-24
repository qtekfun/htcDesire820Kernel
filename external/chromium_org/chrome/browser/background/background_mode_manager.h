// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BACKGROUND_BACKGROUND_MODE_MANAGER_H_
#define CHROME_BROWSER_BACKGROUND_BACKGROUND_MODE_MANAGER_H_

#include <map>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_vector.h"
#include "base/prefs/pref_change_registrar.h"
#include "chrome/browser/background/background_application_list_model.h"
#include "chrome/browser/profiles/profile_info_cache_observer.h"
#include "chrome/browser/status_icons/status_icon.h"
#include "chrome/browser/status_icons/status_icon_menu_model.h"
#include "chrome/browser/ui/browser_list_observer.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class Browser;
class CommandLine;
class PrefRegistrySimple;
class Profile;
class ProfileInfoCache;
class StatusIcon;
class StatusTray;

namespace extensions {
class Extension;
}

typedef std::vector<int> CommandIdExtensionVector;

class BackgroundModeManager
    : public content::NotificationObserver,
      public chrome::BrowserListObserver,
      public BackgroundApplicationListModel::Observer,
      public ProfileInfoCacheObserver,
      public StatusIconMenuModel::Delegate {
 public:
  BackgroundModeManager(CommandLine* command_line,
                        ProfileInfoCache* profile_cache);
  virtual ~BackgroundModeManager();

  static void RegisterPrefs(PrefRegistrySimple* registry);

  virtual void RegisterProfile(Profile* profile);

  static void LaunchBackgroundApplication(Profile* profile,
      const extensions::Extension* extension);

  
  virtual bool IsBackgroundModeActive();

  
  
  
  virtual void SuspendBackgroundMode();

  
  
  virtual void ResumeBackgroundMode();

  
  int NumberOfBackgroundModeData();

 private:
  friend class AppBackgroundPageApiTest;
  friend class BackgroundModeManagerTest;
  friend class TestBackgroundModeManager;
  FRIEND_TEST_ALL_PREFIXES(BackgroundModeManagerTest,
                           BackgroundAppLoadUnload);
  FRIEND_TEST_ALL_PREFIXES(BackgroundModeManagerTest,
                           BackgroundLaunchOnStartup);
  FRIEND_TEST_ALL_PREFIXES(BackgroundModeManagerTest,
                           BackgroundAppInstallUninstallWhileDisabled);
  FRIEND_TEST_ALL_PREFIXES(BackgroundModeManagerTest,
                           BackgroundModeDisabledPreventsKeepAliveOnStartup);
  FRIEND_TEST_ALL_PREFIXES(BackgroundModeManagerTest,
                           DisableBackgroundModeUnderTestFlag);
  FRIEND_TEST_ALL_PREFIXES(BackgroundModeManagerTest,
                           EnableAfterBackgroundAppInstall);
  FRIEND_TEST_ALL_PREFIXES(BackgroundModeManagerTest,
                           MultiProfile);
  FRIEND_TEST_ALL_PREFIXES(BackgroundModeManagerTest,
                           ProfileInfoCacheStorage);
  FRIEND_TEST_ALL_PREFIXES(BackgroundModeManagerTest,
                           ProfileInfoCacheObserver);
  FRIEND_TEST_ALL_PREFIXES(BackgroundModeManagerTest,
                           BackgroundMenuGeneration);
  FRIEND_TEST_ALL_PREFIXES(BackgroundModeManagerTest,
                           BackgroundMenuGenerationMultipleProfile);
  FRIEND_TEST_ALL_PREFIXES(BackgroundAppBrowserTest,
                           ReloadBackgroundApp);

  class BackgroundModeData : public StatusIconMenuModel::Delegate {
   public:
    explicit BackgroundModeData(
        Profile* profile,
        CommandIdExtensionVector* command_id_extension_vector);
    virtual ~BackgroundModeData();

    
    scoped_ptr<BackgroundApplicationListModel> applications_;

    
    virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

    
    
    
    Browser* GetBrowserWindow();

    
    int GetBackgroundAppCount() const;

    
    
    
    
    void BuildProfileMenu(StatusIconMenuModel* menu,
                          StatusIconMenuModel* containing_menu);

    
    
    void SetName(const base::string16& new_profile_name);

    
    
    base::string16 name();

    
    static bool BackgroundModeDataCompare(const BackgroundModeData* bmd1,
                                          const BackgroundModeData* bmd2);

   private:
    
    base::string16 name_;

    
    Profile* profile_;

    
    
    
    
    CommandIdExtensionVector* command_id_extension_vector_;
  };

  
  
  
  
  
  
  
  typedef linked_ptr<BackgroundModeData> BackgroundModeInfo;

  typedef std::map<Profile*, BackgroundModeInfo> BackgroundModeInfoMap;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void OnBackgroundModeEnabledPrefChanged();

  
  virtual void OnApplicationDataChanged(const extensions::Extension* extension,
                                        Profile* profile) OVERRIDE;
  virtual void OnApplicationListChanged(Profile* profile) OVERRIDE;

  
  virtual void OnProfileAdded(const base::FilePath& profile_path) OVERRIDE;
  virtual void OnProfileWillBeRemoved(
      const base::FilePath& profile_path) OVERRIDE;
  virtual void OnProfileNameChanged(
      const base::FilePath& profile_path,
      const base::string16& old_profile_name) OVERRIDE;

  
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

  
  virtual void OnBrowserAdded(Browser* browser) OVERRIDE;

  
  
  
  void OnBackgroundAppInstalled(
      const extensions::Extension* extension);

  
  
  
  void CheckReloadStatus(
      const extensions::Extension* extension,
      bool* is_being_reloaded);

  
  
  virtual void EnableLaunchOnStartup(bool should_launch);

  
  
  virtual void DisplayAppInstalledNotification(
      const extensions::Extension* extension);

  
  
  void StartBackgroundMode();

  
  
  void EndBackgroundMode();

  
  
  virtual void UpdateKeepAliveAndTrayIcon();

  
  
  
  
  void EndKeepAliveForStartup();

  
  
  base::string16 GetPreferencesMenuLabel();

  
  
  virtual void CreateStatusTrayIcon();

  
  
  virtual void RemoveStatusTrayIcon();

  
  
  
  
  virtual void UpdateStatusTrayIconContextMenu();

  
  
  BackgroundModeManager::BackgroundModeData* GetBackgroundModeData(
      Profile* const profile) const;

  
  
  
  BackgroundModeInfoMap::iterator GetBackgroundModeIterator(
      const base::string16& profile_name);

  
  
  virtual bool IsBackgroundModePrefEnabled() const;

  
  void DisableBackgroundMode();

  
  void EnableBackgroundMode();

  
  
  virtual int GetBackgroundAppCount() const;

  
  virtual int GetBackgroundAppCountForProfile(Profile* const profile) const;

  
  bool ShouldBeInBackgroundMode() const;

  
  
  ProfileInfoCache* profile_cache_;

  
  content::NotificationRegistrar registrar_;
  PrefChangeRegistrar pref_registrar_;

  
  BackgroundModeInfoMap background_mode_data_;

  
  CommandIdExtensionVector command_id_extension_vector_;

  
  ScopedVector<StatusIconMenuModel> submenus;

  
  
  StatusTray* status_tray_;

  
  StatusIcon* status_icon_;

  
  
  StatusIconMenuModel* context_menu_;

  
  
  
  bool in_background_mode_;

  
  
  
  bool keep_alive_for_startup_;

  
  
  
  bool keep_alive_for_test_;

  
  bool background_mode_suspended_;

  
  bool keeping_alive_;

  DISALLOW_COPY_AND_ASSIGN(BackgroundModeManager);
};

#endif  
