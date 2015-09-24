// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BROWSER_H_
#define CHROME_BROWSER_UI_BROWSER_H_
#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "base/task.h"
#include "chrome/browser/command_updater.h"
#include "chrome/browser/debugger/devtools_toggle_action.h"
#include "chrome/browser/instant/instant_delegate.h"
#include "chrome/browser/prefs/pref_member.h"
#include "chrome/browser/sessions/session_id.h"
#include "chrome/browser/sessions/tab_restore_service_observer.h"
#include "chrome/browser/sync/profile_sync_service_observer.h"
#include "chrome/browser/tabs/tab_handler.h"
#include "chrome/browser/tabs/tab_strip_model_delegate.h"  
#include "chrome/browser/tabs/tab_strip_model_observer.h"  
#include "chrome/browser/ui/browser_navigator.h"
#include "chrome/browser/ui/search_engines/search_engine_tab_helper_delegate.h"
#include "chrome/browser/ui/shell_dialogs.h"
#include "chrome/browser/ui/tab_contents/tab_contents_wrapper_delegate.h"
#include "chrome/browser/ui/toolbar/toolbar_model.h"
#include "chrome/common/extensions/extension_constants.h"
#include "content/browser/tab_contents/page_navigator.h"
#include "content/browser/tab_contents/tab_contents_delegate.h"
#include "content/common/notification_registrar.h"
#include "content/common/page_transition_types.h"
#include "content/common/page_zoom.h"
#include "ui/gfx/rect.h"

class BrowserTabRestoreServiceDelegate;
class BrowserWindow;
class Extension;
class FindBarController;
class InstantController;
class InstantUnloadHandler;
class PrefService;
class Profile;
class SessionStorageNamespace;
class SkBitmap;
class StatusBubble;
class TabNavigation;
class TabStripModel;
struct WebApplicationInfo;
namespace gfx {
class Point;
}

class Browser : public TabHandlerDelegate,
                public TabContentsDelegate,
                public TabContentsWrapperDelegate,
                public SearchEngineTabHelperDelegate,
                public PageNavigator,
                public CommandUpdater::CommandUpdaterDelegate,
                public NotificationObserver,
                public SelectFileDialog::Listener,
                public TabRestoreServiceObserver,
                public ProfileSyncServiceObserver,
                public InstantDelegate {
 public:
  
  
  
  enum Type {
    TYPE_NORMAL = 1,
    TYPE_POPUP = 2,
    
    
    
    TYPE_APP = 4,
    TYPE_APP_POPUP = TYPE_APP | TYPE_POPUP,
    TYPE_DEVTOOLS = TYPE_APP | 8,

    
    
    
    
    TYPE_APP_PANEL = TYPE_APP | TYPE_POPUP | 16,
    TYPE_ANY = TYPE_NORMAL |
               TYPE_POPUP |
               TYPE_APP |
               TYPE_DEVTOOLS |
               TYPE_APP_PANEL
  };

  
  enum WindowFeature {
    FEATURE_NONE = 0,
    FEATURE_TITLEBAR = 1,
    FEATURE_TABSTRIP = 2,
    FEATURE_TOOLBAR = 4,
    FEATURE_LOCATIONBAR = 8,
    FEATURE_BOOKMARKBAR = 16,
    FEATURE_INFOBAR = 32,
    FEATURE_SIDEBAR = 64,
    FEATURE_DOWNLOADSHELF = 128
  };

  
  enum MaximizedState {
    
    
    MAXIMIZED_STATE_DEFAULT,

    
    MAXIMIZED_STATE_MAXIMIZED,

    
    MAXIMIZED_STATE_UNMAXIMIZED
  };

  

  
  
  
  
  
  Browser(Type type, Profile* profile);
  virtual ~Browser();

  
  
  static Browser* Create(Profile* profile);

  
  
  static Browser* CreateForPopup(Type type, Profile* profile,
                                 TabContents* new_contents,
                                 const gfx::Rect& initial_bounds);

  
  static Browser* CreateForType(Type type, Profile* profile);

  
  
  
  
  static Browser* CreateForApp(const std::string& app_name,
                               const gfx::Size& window_size,
                               Profile* profile,
                               bool is_panel);

  
  
  static Browser* CreateForDevTools(Profile* profile);

  
  void set_override_bounds(const gfx::Rect& bounds) {
    override_bounds_ = bounds;
  }
  void set_maximized_state(MaximizedState state) {
    maximized_state_ = state;
  }
  
  bool bounds_overridden() const {
    return !override_bounds_.IsEmpty();
  }

  
  
  
  void InitBrowserWindow();

  

  Type type() const { return type_; }
  const std::string& app_name() const { return app_name_; }
  Profile* profile() const { return profile_; }
  const std::vector<std::wstring>& user_data_dir_profiles() const;
  gfx::Rect override_bounds() const { return override_bounds_; }

  
  
  InstantController* instant() const { return instant_.get(); }

#if defined(UNIT_TEST)
  
  
  
  void set_window(BrowserWindow* window) {
    DCHECK(!window_);
    window_ = window;
  }
#endif

  
  
  BrowserWindow* window() const { return window_; }
  ToolbarModel* toolbar_model() { return &toolbar_model_; }
  const SessionID& session_id() const { return session_id_; }
  CommandUpdater* command_updater() { return &command_updater_; }
  bool block_command_execution() const { return block_command_execution_; }
  BrowserTabRestoreServiceDelegate* tab_restore_service_delegate() {
    return tab_restore_service_delegate_.get();
  }

  
  
  FindBarController* GetFindBarController();

  
  bool HasFindBarController() const;

  

  void set_user_data_dir_profiles(const std::vector<std::wstring>& profiles);

  

  
  static void OpenEmptyWindow(Profile* profile);

  
  static void OpenWindowWithRestoredTabs(Profile* profile);

  
  
  
  static void OpenURLOffTheRecord(Profile* profile, const GURL& url);

  
  
  
  static TabContents* OpenApplication(
      Profile* profile,
      const Extension* extension,
      extension_misc::LaunchContainer container,
      TabContents* existing_tab);

  
  
  
  
  
  
  
  static TabContents* OpenApplicationWindow(
      Profile* profile,
      const Extension* extension,
      extension_misc::LaunchContainer container,
      const GURL& url,
      Browser** app_browser);

  
  
  
  
  
  
  static TabContents* OpenAppShortcutWindow(Profile* profile,
                                            const GURL& url,
                                            bool update_shortcut);

  
  
  
  static TabContents* OpenApplicationTab(Profile* profile,
                                         const Extension* extension,
                                         TabContents* existing_tab);

  
  static void OpenBookmarkManagerWindow(Profile* profile);

#if defined(OS_MACOSX)
  
  
  static void OpenHistoryWindow(Profile* profile);
  static void OpenDownloadsWindow(Profile* profile);
  static void OpenHelpWindow(Profile* profile);
  static void OpenOptionsWindow(Profile* profile);
  static void OpenClearBrowingDataDialogWindow(Profile* profile);
  static void OpenImportSettingsDialogWindow(Profile* profile);
  static void OpenInstantConfirmDialogWindow(Profile* profile);
#endif

  
  
  static void OpenExtensionsWindow(Profile* profile);

  

  
  std::string GetWindowPlacementKey() const;
  bool ShouldSaveWindowPlacement() const;
  void SaveWindowPlacement(const gfx::Rect& bounds, bool maximized);
  gfx::Rect GetSavedWindowBounds() const;
  bool GetSavedMaximizedState() const;

  
  SkBitmap GetCurrentPageIcon() const;

  
  string16 GetWindowTitleForCurrentTab() const;

  
  static void FormatTitleForDisplay(string16* title);

  

  
  bool ShouldCloseWindow();

  bool IsAttemptingToCloseBrowser() const {
    return is_attempting_to_close_browser_;
  }

  
  
  void OnWindowClosing();

  

  
  void CheckDownloadsInProgress(bool* normal_downloads,
                                bool* incognito_downloads);

  
  
  
  
  void InProgressDownloadResponse(bool cancel_downloads);

  

  TabStripModel* tabstrip_model() const {
    
    return tab_handler_->GetTabStripModel();
  }

  int tab_count() const;
  int active_index() const;
  int GetIndexOfController(const NavigationController* controller) const;
  TabContentsWrapper* GetSelectedTabContentsWrapper() const;
  TabContentsWrapper* GetTabContentsWrapperAt(int index) const;
  
  
  
  
  
  
  TabContents* GetSelectedTabContents() const;
  TabContents* GetTabContentsAt(int index) const;
  void ActivateTabAt(int index, bool user_gesture);
  bool IsTabPinned(int index) const;
  void CloseAllTabs();

  

  
  bool IsTabStripEditable() const;

  
  
  
  
  int GetIndexForInsertionDuringRestore(int relative_index);

  
  
  TabContentsWrapper* AddSelectedTabWithURL(
      const GURL& url,
      PageTransition::Type transition);

  
  
  TabContents* AddTab(TabContentsWrapper* tab_contents,
                      PageTransition::Type type);

  
  
  
  
  
  
  
  
  TabContents* AddRestoredTab(const std::vector<TabNavigation>& navigations,
                              int tab_index,
                              int selected_navigation,
                              const std::string& extension_app_id,
                              bool select,
                              bool pin,
                              bool from_last_session,
                              SessionStorageNamespace* storage_namespace);
  
  
  
  
  void AddTabContents(TabContents* new_contents,
                      WindowOpenDisposition disposition,
                      const gfx::Rect& initial_pos,
                      bool user_gesture);
  void CloseTabContents(TabContents* contents);

  
  
  
  
  void BrowserShowHtmlDialog(HtmlDialogUIDelegate* delegate,
                             gfx::NativeWindow parent_window);

  
  void BrowserRenderWidgetShowing();

  
  
  void BookmarkBarSizeChanged(bool is_animating);

  
  
  void ReplaceRestoredTab(
      const std::vector<TabNavigation>& navigations,
      int selected_navigation,
      bool from_last_session,
      const std::string& extension_app_id,
      SessionStorageNamespace* session_storage_namespace);

  
  
  bool NavigateToIndexWithDisposition(int index, WindowOpenDisposition disp);

  
  
  
  void ShowSingletonTab(const GURL& url);

  
  
  
  
  void UpdateCommandsForFullscreenMode(bool is_fullscreen);

  

  
  

  
  void GoBack(WindowOpenDisposition disposition);
  void GoForward(WindowOpenDisposition disposition);
  void Reload(WindowOpenDisposition disposition);
  void ReloadIgnoringCache(WindowOpenDisposition disposition);  
  void Home(WindowOpenDisposition disposition);
  void OpenCurrentURL();
  void Stop();
  
  void NewWindow();
  void NewIncognitoWindow();
  void CloseWindow();
  void NewTab();
  void CloseTab();
  void SelectNextTab();
  void SelectPreviousTab();
  void OpenTabpose();
  void MoveTabNext();
  void MoveTabPrevious();
  void SelectNumberedTab(int index);
  void SelectLastTab();
  void DuplicateTab();
  void WriteCurrentURLToClipboard();
  void ConvertPopupToTabbedBrowser();
  
  void ToggleFullscreenMode();
  void Exit();
#if defined(OS_CHROMEOS)
  void ToggleCompactNavigationBar();
  void Search();
  void ShowKeyboardOverlay();
#endif

  
  void BookmarkCurrentPage();
  void SavePage();
  void ViewSelectedSource();
  void ShowFindBar();

  
  
  
  
  
  bool SupportsWindowFeature(WindowFeature feature) const;

  
  
  bool CanSupportWindowFeature(WindowFeature feature) const;

  
  
  void Print();
  void EmailPageLocation();
  void ToggleEncodingAutoDetect();
  void OverrideEncoding(int encoding_id);

  
  void Cut();
  void Copy();
  void Paste();

  
  void Find();
  void FindNext();
  void FindPrevious();

  
  void Zoom(PageZoom::Function zoom_function);

  
  void FocusToolbar();
  void FocusLocationBar();  
  void FocusSearch();
  void FocusAppMenu();
  void FocusBookmarksToolbar();
  void FocusChromeOSStatus();
  void FocusNextPane();
  void FocusPreviousPane();

  
  void OpenFile();
  void OpenCreateShortcutsDialog();
  void ToggleDevToolsWindow(DevToolsToggleAction action);
  void OpenTaskManager(bool highlight_background_resources);
  void OpenBugReportDialog();

  void ToggleBookmarkBar();

  void OpenBookmarkManager();
  void ShowAppMenu();
  void ShowBookmarkManagerTab();
  void ShowHistoryTab();
  void ShowDownloadsTab();
  void ShowExtensionsTab();
  void ShowAboutConflictsTab();
  void ShowBrokenPageTab(TabContents* contents);
  void ShowOptionsTab(const std::string& sub_page);
  void OpenClearBrowsingDataDialog();
  void OpenOptionsDialog();
  void OpenPasswordManager();
  void OpenSyncMyBookmarksDialog();
#if defined(ENABLE_REMOTING)
  void OpenRemotingSetupDialog();
#endif
  void OpenImportSettingsDialog();
  void OpenInstantConfirmDialog();
  void OpenAboutChromeDialog();
  void OpenUpdateChromeDialog();
  void OpenHelpTab();
  
  void OpenThemeGalleryTabAndActivate();
  void OpenAutofillHelpTabAndActivate();
  void OpenPrivacyDashboardTabAndActivate();
  void OpenSearchEngineOptionsDialog();
#if defined(OS_CHROMEOS)
  void OpenFileManager();
  void OpenSystemOptionsDialog();
  void OpenInternetOptionsDialog();
  void OpenLanguageOptionsDialog();
  void OpenSystemTabAndActivate();
  void OpenMobilePlanTabAndActivate();
#endif
  void OpenPluginsTabAndActivate();

  virtual void UpdateDownloadShelfVisibility(bool visible);

  
  virtual bool UseVerticalTabs() const;

  

  
  
  
  
  
  
  static void SetNewHomePagePrefs(PrefService* prefs);

  static void RegisterPrefs(PrefService* prefs);
  static void RegisterUserPrefs(PrefService* prefs);

  
  static bool RunUnloadEventsHelper(TabContents* contents);

  
  
  
  
  
  static Browser* GetBrowserForController(
      const NavigationController* controller, int* index);

  
  static Browser* GetTabbedBrowser(Profile* profile, bool match_incognito);

  
  
  static Browser* GetOrCreateTabbedBrowser(Profile* profile);

  
  void ExecuteCommandWithDisposition(int id, WindowOpenDisposition);

  
  
  bool ExecuteCommandIfEnabled(int id);

  
  
  
  bool IsReservedCommandOrKey(int command_id,
                              const NativeWebKeyboardEvent& event);

  
  
  
  
  void SetBlockCommandExecution(bool block);

  
  
  
  
  int GetLastBlockedCommand(WindowOpenDisposition* disposition);

  
  
  void UpdateUIForNavigationInTab(TabContentsWrapper* contents,
                                  PageTransition::Type transition,
                                  bool user_initiated);

  
  
  GURL GetHomePage() const;

  

  
  virtual void OpenURL(const GURL& url, const GURL& referrer,
                       WindowOpenDisposition disposition,
                       PageTransition::Type transition);

  
  virtual void ExecuteCommand(int id);

  
  virtual void TabRestoreServiceChanged(TabRestoreService* service);
  virtual void TabRestoreServiceDestroyed(TabRestoreService* service);

  
  
  static TabContentsWrapper*
      TabContentsFactory(Profile* profile,
                         SiteInstance* site_instance,
                         int routing_id,
                         const TabContents* base_tab_contents,
                         SessionStorageNamespace* session_storage_namespace);

  
  virtual Profile* GetProfile() const;
  virtual Browser* AsBrowser();

  
  virtual TabContentsWrapper* AddBlankTab(bool foreground);
  virtual TabContentsWrapper* AddBlankTabAt(int index, bool foreground);
  virtual Browser* CreateNewStripWithContents(
      TabContentsWrapper* detached_contents,
      const gfx::Rect& window_bounds,
      const DockInfo& dock_info,
      bool maximize);
  virtual int GetDragActions() const;
  
  
  virtual TabContentsWrapper* CreateTabContentsForURL(const GURL& url,
                                               const GURL& referrer,
                                               Profile* profile,
                                               PageTransition::Type transition,
                                               bool defer_load,
                                               SiteInstance* instance) const;
  virtual bool CanDuplicateContentsAt(int index);
  virtual void DuplicateContentsAt(int index);
  virtual void CloseFrameAfterDragSession();
  virtual void CreateHistoricalTab(TabContentsWrapper* contents);
  virtual bool RunUnloadListenerBeforeClosing(TabContentsWrapper* contents);
  virtual bool CanCloseContentsAt(int index);
  virtual bool CanBookmarkAllTabs() const;
  virtual void BookmarkAllTabs();
  virtual bool CanCloseTab() const;
  virtual void ToggleUseVerticalTabs();
  virtual bool CanRestoreTab();
  virtual void RestoreTab();
  virtual bool LargeIconsPermitted() const;

  
  virtual void TabInsertedAt(TabContentsWrapper* contents,
                             int index,
                             bool foreground);
  virtual void TabClosingAt(TabStripModel* tab_strip_model,
                            TabContentsWrapper* contents,
                            int index);
  virtual void TabDetachedAt(TabContentsWrapper* contents, int index);
  virtual void TabDeselected(TabContentsWrapper* contents);
  virtual void TabSelectedAt(TabContentsWrapper* old_contents,
                             TabContentsWrapper* new_contents,
                             int index,
                             bool user_gesture);
  virtual void TabMoved(TabContentsWrapper* contents,
                        int from_index,
                        int to_index);
  virtual void TabReplacedAt(TabStripModel* tab_strip_model,
                             TabContentsWrapper* old_contents,
                             TabContentsWrapper* new_contents,
                             int index);
  virtual void TabPinnedStateChanged(TabContentsWrapper* contents, int index);
  virtual void TabStripEmpty();

  
  bool TabsNeedBeforeUnloadFired();

 protected:
  
  
  virtual BrowserWindow* CreateBrowserWindow();

 private:
  FRIEND_TEST_ALL_PREFIXES(BrowserTest, NoTabsInPopups);
  FRIEND_TEST_ALL_PREFIXES(BrowserTest, ConvertTabToAppShortcut);
  FRIEND_TEST_ALL_PREFIXES(BrowserTest, OpenAppWindowLikeNtp);
  FRIEND_TEST_ALL_PREFIXES(BrowserTest, AppIdSwitch);
  FRIEND_TEST_ALL_PREFIXES(BrowserInitTest, OpenAppShortcutNoPref);
  FRIEND_TEST_ALL_PREFIXES(BrowserInitTest, OpenAppShortcutWindowPref);
  FRIEND_TEST_ALL_PREFIXES(BrowserInitTest, OpenAppShortcutTabPref);
  FRIEND_TEST_ALL_PREFIXES(BrowserInitTest, OpenAppShortcutPanel);

  
  
  enum DetachType {
    
    DETACH_TYPE_DETACH,

    
    DETACH_TYPE_REPLACE,

    
    DETACH_TYPE_EMPTY
  };

  
  virtual void OpenURLFromTab(TabContents* source,
                              const GURL& url,
                              const GURL& referrer,
                              WindowOpenDisposition disposition,
                              PageTransition::Type transition);
  virtual void NavigationStateChanged(const TabContents* source,
                                      unsigned changed_flags);
  virtual void AddNewContents(TabContents* source,
                              TabContents* new_contents,
                              WindowOpenDisposition disposition,
                              const gfx::Rect& initial_pos,
                              bool user_gesture);
  virtual void ActivateContents(TabContents* contents);
  virtual void DeactivateContents(TabContents* contents);
  virtual void LoadingStateChanged(TabContents* source);
  virtual void CloseContents(TabContents* source);
  virtual void MoveContents(TabContents* source, const gfx::Rect& pos);
  virtual void DetachContents(TabContents* source);
  virtual bool IsPopup(const TabContents* source) const;
  virtual bool CanReloadContents(TabContents* source) const;
  virtual void UpdateTargetURL(TabContents* source, const GURL& url);
  virtual void ContentsMouseEvent(
      TabContents* source, const gfx::Point& location, bool motion);
  virtual void ContentsZoomChange(bool zoom_in);
  virtual void OnContentSettingsChange(TabContents* source);
  virtual void SetTabContentBlocked(TabContents* contents, bool blocked);
  virtual void TabContentsFocused(TabContents* tab_content);
  virtual bool TakeFocus(bool reverse);
  virtual bool IsApplication() const;
  virtual void ConvertContentsToApplication(TabContents* source);
  virtual bool ShouldDisplayURLField();
  virtual void ShowHtmlDialog(HtmlDialogUIDelegate* delegate,
                              gfx::NativeWindow parent_window);
  virtual void BeforeUnloadFired(TabContents* source,
                                 bool proceed,
                                 bool* proceed_to_fire_unload);
  virtual void SetFocusToLocationBar(bool select_all);
  virtual void RenderWidgetShowing();
  virtual int GetExtraRenderViewHeight() const;
  virtual void OnStartDownload(DownloadItem* download, TabContents* tab);
  virtual void ShowPageInfo(Profile* profile,
                            const GURL& url,
                            const NavigationEntry::SSLStatus& ssl,
                            bool show_history);
  virtual void ViewSourceForTab(TabContents* source, const GURL& page_url);
  virtual void ViewSourceForFrame(TabContents* source,
                                  const GURL& frame_url,
                                  const std::string& frame_content_state);
  virtual bool PreHandleKeyboardEvent(const NativeWebKeyboardEvent& event,
                                        bool* is_keyboard_shortcut);
  virtual void HandleKeyboardEvent(const NativeWebKeyboardEvent& event);
  virtual void ShowRepostFormWarningDialog(TabContents* tab_contents);
  virtual void ShowContentSettingsPage(ContentSettingsType content_type);
  virtual void ShowCollectedCookiesDialog(TabContents* tab_contents);
  virtual bool ShouldAddNavigationToHistory(
      const history::HistoryAddPageArgs& add_page_args,
      NavigationType::Type navigation_type);
  virtual void ContentRestrictionsChanged(TabContents* source);
  virtual void WorkerCrashed();

  
  virtual void URLStarredChanged(TabContentsWrapper* source,
                                 bool starred) OVERRIDE;
  virtual void OnDidGetApplicationInfo(TabContentsWrapper* source,
                                       int32 page_id) OVERRIDE;
  virtual void OnInstallApplication(
      TabContentsWrapper* source,
      const WebApplicationInfo& app_info) OVERRIDE;

  
  virtual void ConfirmSetDefaultSearchProvider(
      TabContents* tab_contents,
      TemplateURL* template_url,
      TemplateURLModel* template_url_model) OVERRIDE;
  virtual void ConfirmAddSearchProvider(const TemplateURL* template_url,
                                        Profile* profile) OVERRIDE;

  
  virtual void FileSelected(const FilePath& path, int index, void* params);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual void OnStateChanged();

  
  virtual void PrepareForInstant() OVERRIDE;
  virtual void ShowInstant(TabContentsWrapper* preview_contents) OVERRIDE;
  virtual void HideInstant() OVERRIDE;
  virtual void CommitInstant(TabContentsWrapper* preview_contents) OVERRIDE;
  virtual void SetSuggestedText(const string16& text,
                                InstantCompleteBehavior behavior) OVERRIDE;
  virtual gfx::Rect GetInstantBounds() OVERRIDE;

  

  
  void InitCommandState();

  
  void UpdateCommandsForTabState();

  
  void UpdateCommandsForContentRestrictionState();

  
  void UpdateCommandsForDevTools();

  
  void UpdateCommandsForBookmarkEditing();

  
  void UpdatePrintingState(int content_restrictions);

  
  
  
  void UpdateReloadStopState(bool is_loading, bool force);

  

  
  
  
  
  
  void UpdateToolbar(bool should_restore_state);

  
  
  
  
  
  
  void ScheduleUIUpdate(const TabContents* source, unsigned changed_flags);

  
  
  void ProcessPendingUIUpdates();

  
  void RemoveScheduledUpdatesFor(TabContents* contents);

  

  
  
  friend class AutomationProvider;
  friend class TestingAutomationProvider;

  
  
  
  StatusBubble* GetStatusBubble();

  

  
  
  void SyncHistoryWithTabs(int index);

  

  typedef std::set<TabContents*> UnloadListenerSet;

  
  void ProcessPendingTabs();

  
  bool HasCompletedUnloadProcessing() const;

  
  
  void CancelWindowClose();

  
  
  
  bool RemoveFromSet(UnloadListenerSet* set, TabContents* tab);

  
  
  
  
  
  
  
  
  
  void ClearUnloadState(TabContents* tab, bool process_now);

  

  
  
  
  bool CanCloseWithInProgressDownloads();

  

  
  
  void SetAsDelegate(TabContentsWrapper* tab, Browser* delegate);

  
  
  
  void FindInPage(bool find_next, bool forward_direction);

  
  
  
  void CloseFrame();

  void TabDetachedAtImpl(TabContentsWrapper* contents,
      int index, DetachType type);

  
  
  
  static void RegisterAppPrefs(const std::string& app_name, Profile* profile);

  
  void ReloadInternal(WindowOpenDisposition disposition, bool ignore_cache);

  
  bool ShouldOpenNewTabForWindowDisposition(WindowOpenDisposition disposition);

  
  
  TabContents* GetOrCloneTabForDisposition(WindowOpenDisposition disposition);

  
  
  void UpdateTabStripModelInsertionPolicy();

  
  
  void UseVerticalTabsChanged();

  
  
  
  
  bool SupportsWindowFeatureImpl(WindowFeature feature,
                                 bool check_fullscreen) const;

  
  
  
  
  
  
  
  bool IsClosingPermitted();

  
  
  bool OpenInstant(WindowOpenDisposition disposition);

  
  void CreateInstantIfNecessary();

  
  void ViewSource(TabContentsWrapper* tab);

  
  browser::NavigateParams GetSingletonTabNavigateParams(const GURL& url);

  
  void ViewSource(TabContentsWrapper* tab,
                  const GURL& url,
                  const std::string& content_state);

  

  NotificationRegistrar registrar_;

  
  const Type type_;

  
  Profile* const profile_;

  
  BrowserWindow* window_;

  
  scoped_ptr<TabHandler> tab_handler_;

  
  CommandUpdater command_updater_;

  
  
  std::string app_name_;

  
  
  
  const SessionID session_id_;

  
  ToolbarModel toolbar_model_;

  

  typedef std::map<const TabContents*, int> UpdateMap;

  
  
  
  
  UpdateMap scheduled_updates_;

  
  ScopedRunnableMethodFactory<Browser> chrome_updater_factory_;

  

  
  
  UnloadListenerSet tabs_needing_before_unload_fired_;

  
  
  UnloadListenerSet tabs_needing_unload_fired_;

  
  
  bool is_attempting_to_close_browser_;

  

  enum CancelDownloadConfirmationState {
    NOT_PROMPTED,          
    WAITING_FOR_RESPONSE,  
                           
    RESPONSE_RECEIVED      
  };

  
  
  CancelDownloadConfirmationState cancel_download_confirmation_state_;

  

  
  
  
  
  
  gfx::Rect override_bounds_;
  MaximizedState maximized_state_;

  
  ScopedRunnableMethodFactory<Browser> method_factory_;

  
  
  scoped_ptr<FindBarController> find_bar_controller_;

  
  scoped_refptr<SelectFileDialog> select_file_dialog_;

  
  BooleanPrefMember encoding_auto_detect_;

  
  BooleanPrefMember printing_enabled_;

  
  BooleanPrefMember dev_tools_disabled_;

  
  BooleanPrefMember instant_enabled_;

  
  BooleanPrefMember incognito_mode_allowed_;

  
  BooleanPrefMember edit_bookmarks_enabled_;

  
  bool block_command_execution_;

  
  int last_blocked_command_id_;

  
  WindowOpenDisposition last_blocked_command_disposition_;

  
  
  enum WebAppAction {
    NONE,             
    CREATE_SHORTCUT,  
    UPDATE_SHORTCUT   
  };

  
  
  WebAppAction pending_web_app_action_;

  
  mutable BooleanPrefMember use_vertical_tabs_;

  
  
  TabRestoreService* tab_restore_service_;

  
  scoped_ptr<BrowserTabRestoreServiceDelegate> tab_restore_service_delegate_;

  scoped_ptr<InstantController> instant_;
  scoped_ptr<InstantUnloadHandler> instant_unload_handler_;

  DISALLOW_COPY_AND_ASSIGN(Browser);
};

#endif  
