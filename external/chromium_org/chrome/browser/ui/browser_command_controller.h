// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BROWSER_COMMAND_CONTROLLER_H_
#define CHROME_BROWSER_UI_BROWSER_COMMAND_CONTROLLER_H_

#include <vector>

#include "base/prefs/pref_change_registrar.h"
#include "base/prefs/pref_member.h"
#include "chrome/browser/command_updater.h"
#include "chrome/browser/command_updater_delegate.h"
#include "chrome/browser/profiles/profile_info_cache_observer.h"
#include "chrome/browser/sessions/tab_restore_service_observer.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "ui/base/window_open_disposition.h"

class Browser;
class BrowserWindow;
class Profile;
class ProfileManager;

namespace content {
struct NativeWebKeyboardEvent;
}

namespace chrome {

class BrowserCommandController : public CommandUpdaterDelegate,
                                 public ProfileInfoCacheObserver,
                                 public TabStripModelObserver,
                                 public TabRestoreServiceObserver {
 public:
  BrowserCommandController(Browser* browser, ProfileManager* profile_manager);
  virtual ~BrowserCommandController();

  CommandUpdater* command_updater() { return &command_updater_; }
  bool block_command_execution() const { return block_command_execution_; }

  
  
  
  bool IsReservedCommandOrKey(int command_id,
                              const content::NativeWebKeyboardEvent& event);

  
  
  
  
  void SetBlockCommandExecution(bool block);

  
  
  
  
  int GetLastBlockedCommand(WindowOpenDisposition* disposition);

  
  
  void TabStateChanged();
  void ContentRestrictionsChanged();
  void FullscreenStateChanged();
  void PrintingStateChanged();
  void LoadingStateChanged(bool is_loading, bool force);

  
  

  
  static void UpdateOpenFileState(CommandUpdater* command_updater);

  
  
  static void UpdateSharedCommandsForIncognitoAvailability(
      CommandUpdater* command_updater,
      Profile* profile);

 private:
  class InterstitialObserver;

  
  virtual void ExecuteCommandWithDisposition(
      int id,
      WindowOpenDisposition disposition) OVERRIDE;

  
  virtual void OnProfileAdded(const base::FilePath& profile_path) OVERRIDE;
  virtual void OnProfileWasRemoved(const base::FilePath& profile_path,
                                   const base::string16& profile_name) OVERRIDE;

  
  virtual void TabInsertedAt(content::WebContents* contents,
                             int index,
                             bool foreground) OVERRIDE;
  virtual void TabDetachedAt(content::WebContents* contents,
                             int index) OVERRIDE;
  virtual void TabReplacedAt(TabStripModel* tab_strip_model,
                             content::WebContents* old_contents,
                             content::WebContents* new_contents,
                             int index) OVERRIDE;
  virtual void TabBlockedStateChanged(content::WebContents* contents,
                                      int index) OVERRIDE;

  
  virtual void TabRestoreServiceChanged(TabRestoreService* service) OVERRIDE;
  virtual void TabRestoreServiceDestroyed(TabRestoreService* service) OVERRIDE;

  
  
  
  
  bool IsShowingMainUI();

  
  void InitCommandState();

  
  void UpdateCommandsForIncognitoAvailability();

  
  void UpdateCommandsForTabState();

  
  void UpdateCommandsForContentRestrictionState();

  
  void UpdateCommandsForDevTools();

  
  void UpdateCommandsForBookmarkEditing();

  
  void UpdateCommandsForBookmarkBar();

  
  
  void UpdateCommandsForFileSelectionDialogs();

  
  
  void UpdateCommandsForFullscreenMode();

  
  
  void UpdateCommandsForMultipleProfiles();

  
  void UpdatePrintingState();

  
  void OnSigninAllowedPrefChange();

  
  void UpdateSaveAsState();

  
  void UpdateShowSyncState(bool show_main_ui);

  
  
  
  void UpdateReloadStopState(bool is_loading, bool force);

  
  void UpdateCommandsForFind();

  
  
  void AddInterstitialObservers(content::WebContents* contents);
  void RemoveInterstitialObservers(content::WebContents* contents);

  inline BrowserWindow* window();
  inline Profile* profile();

  Browser* browser_;

  ProfileManager* profile_manager_;

  
  CommandUpdater command_updater_;

  
  bool block_command_execution_;

  
  int last_blocked_command_id_;

  
  WindowOpenDisposition last_blocked_command_disposition_;

  std::vector<InterstitialObserver*> interstitial_observers_;

  PrefChangeRegistrar profile_pref_registrar_;
  PrefChangeRegistrar local_pref_registrar_;
  BooleanPrefMember pref_signin_allowed_;

  DISALLOW_COPY_AND_ASSIGN(BrowserCommandController);
};

}  

#endif  
