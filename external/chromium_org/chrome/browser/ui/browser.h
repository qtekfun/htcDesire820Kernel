// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BROWSER_H_
#define CHROME_BROWSER_UI_BROWSER_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/prefs/pref_member.h"
#include "base/strings/string16.h"
#include "chrome/browser/devtools/devtools_toggle_action.h"
#include "chrome/browser/sessions/session_id.h"
#include "chrome/browser/ui/bookmarks/bookmark_bar.h"
#include "chrome/browser/ui/bookmarks/bookmark_tab_helper_delegate.h"
#include "chrome/browser/ui/browser_navigator.h"
#include "chrome/browser/ui/chrome_web_modal_dialog_manager_delegate.h"
#include "chrome/browser/ui/host_desktop.h"
#include "chrome/browser/ui/search_engines/search_engine_tab_helper_delegate.h"
#include "chrome/browser/ui/tab_contents/core_tab_helper_delegate.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "chrome/browser/ui/toolbar/toolbar_model.h"
#include "chrome/browser/ui/zoom/zoom_observer.h"
#include "chrome/common/content_settings.h"
#include "chrome/common/content_settings_types.h"
#include "chrome/common/extensions/extension_constants.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/page_navigator.h"
#include "content/public/browser/web_contents_delegate.h"
#include "content/public/common/page_transition_types.h"
#include "content/public/common/page_zoom.h"
#include "ui/base/ui_base_types.h"
#include "ui/base/window_open_disposition.h"
#include "ui/gfx/rect.h"
#include "ui/shell_dialogs/select_file_dialog.h"

class BrowserContentSettingBubbleModelDelegate;
class BrowserInstantController;
class BrowserLanguageStateObserver;
class BrowserSyncedWindowDelegate;
class BrowserToolbarModelDelegate;
class BrowserTabRestoreServiceDelegate;
class BrowserWindow;
class FindBarController;
class FullscreenController;
class PrefService;
class Profile;
class SearchDelegate;
class SearchModel;
class StatusBubble;
class TabStripModel;
class TabStripModelDelegate;
struct WebApplicationInfo;

namespace chrome {
class BrowserCommandController;
class FastUnloadController;
class UnloadController;
class ValidationMessageBubble;
}

namespace content {
class NavigationController;
class PageState;
class SessionStorageNamespace;
}

namespace extensions {
class Extension;
class WindowController;
}

namespace gfx {
class Image;
class Point;
}

namespace ui {
struct SelectedFileInfo;
class WebDialogDelegate;
}

namespace web_modal {
class WebContentsModalDialogHost;
}

class Browser : public TabStripModelObserver,
                public content::WebContentsDelegate,
                public CoreTabHelperDelegate,
                public SearchEngineTabHelperDelegate,
                public ChromeWebModalDialogManagerDelegate,
                public BookmarkTabHelperDelegate,
                public ZoomObserver,
                public content::PageNavigator,
                public content::NotificationObserver,
                public ui::SelectFileDialog::Listener {
 public:
  
  
  
  enum Type {
    
    
    TYPE_TABBED = 1,
    TYPE_POPUP = 2
  };

  
  
  
  enum AppType {
    APP_TYPE_HOST = 1,
    APP_TYPE_CHILD = 2
  };

  
  enum WindowFeature {
    FEATURE_NONE = 0,
    FEATURE_TITLEBAR = 1,
    FEATURE_TABSTRIP = 2,
    FEATURE_TOOLBAR = 4,
    FEATURE_LOCATIONBAR = 8,
    FEATURE_BOOKMARKBAR = 16,
    FEATURE_INFOBAR = 32,
    FEATURE_DOWNLOADSHELF = 64
  };

  
  
  enum DownloadClosePreventionType {
    
    DOWNLOAD_CLOSE_OK,

    
    
    DOWNLOAD_CLOSE_BROWSER_SHUTDOWN,

    
    
    DOWNLOAD_CLOSE_LAST_WINDOW_IN_INCOGNITO_PROFILE,
  };

  struct CreateParams {
    CreateParams(Profile* profile, chrome::HostDesktopType host_desktop_type);
    CreateParams(Type type,
                 Profile* profile,
                 chrome::HostDesktopType host_desktop_type);

    static CreateParams CreateForApp(Type type,
                                     const std::string& app_name,
                                     const gfx::Rect& window_bounds,
                                     Profile* profile,
                                     chrome::HostDesktopType host_desktop_type);

    static CreateParams CreateForDevTools(
        Profile* profile,
        chrome::HostDesktopType host_desktop_type);

    
    Type type;

    
    Profile* profile;

    
    chrome::HostDesktopType host_desktop_type;

    
    
    
    
    std::string app_name;

    
    AppType app_type;

    
    gfx::Rect initial_bounds;

    ui::WindowShowState initial_show_state;

    bool is_session_restore;

    
    
    BrowserWindow* window;
  };

  

  explicit Browser(const CreateParams& params);
  virtual ~Browser();

  
  void set_override_bounds(const gfx::Rect& bounds) {
    override_bounds_ = bounds;
  }
  ui::WindowShowState initial_show_state() const { return initial_show_state_; }
  void set_initial_show_state(ui::WindowShowState initial_show_state) {
    initial_show_state_ = initial_show_state;
  }
  
  bool bounds_overridden() const {
    return !override_bounds_.IsEmpty();
  }
  
  
  
  void set_is_session_restore(bool is_session_restore) {
    is_session_restore_ = is_session_restore;
  }
  bool is_session_restore() const {
    return is_session_restore_;
  }
  chrome::HostDesktopType host_desktop_type() const {
    return host_desktop_type_;
  }

  

  Type type() const { return type_; }
  const std::string& app_name() const { return app_name_; }
  AppType app_type() const { return app_type_; }
  Profile* profile() const { return profile_; }
  gfx::Rect override_bounds() const { return override_bounds_; }

  
  
  BrowserWindow* window() const { return window_; }
  ToolbarModel* toolbar_model() { return toolbar_model_.get(); }
  const ToolbarModel* toolbar_model() const { return toolbar_model_.get(); }
#if defined(UNIT_TEST)
  void swap_toolbar_models(scoped_ptr<ToolbarModel>* toolbar_model) {
    toolbar_model->swap(toolbar_model_);
  }
#endif
  TabStripModel* tab_strip_model() const { return tab_strip_model_.get(); }
  chrome::BrowserCommandController* command_controller() {
    return command_controller_.get();
  }
  SearchModel* search_model() { return search_model_.get(); }
  const SearchModel* search_model() const {
      return search_model_.get();
  }
  SearchDelegate* search_delegate() {
    return search_delegate_.get();
  }
  const SessionID& session_id() const { return session_id_; }
  BrowserContentSettingBubbleModelDelegate*
      content_setting_bubble_model_delegate() {
    return content_setting_bubble_model_delegate_.get();
  }
  BrowserTabRestoreServiceDelegate* tab_restore_service_delegate() {
    return tab_restore_service_delegate_.get();
  }
  BrowserSyncedWindowDelegate* synced_window_delegate() {
    return synced_window_delegate_.get();
  }
  BrowserInstantController* instant_controller() {
    return instant_controller_.get();
  }

  
  
  FindBarController* GetFindBarController();

  
  bool HasFindBarController() const;

  
  BookmarkBar::State bookmark_bar_state() const { return bookmark_bar_state_; }

  

  
  gfx::Image GetCurrentPageIcon() const;

  
  base::string16 GetWindowTitleForCurrentTab() const;

  
  static void FormatTitleForDisplay(base::string16* title);

  

  
  
  
  
  bool ShouldCloseWindow();

  
  
  
  
  
  
  
  
  
  bool CallBeforeUnloadHandlers(
      const base::Callback<void(bool)>& on_close_confirmed);

  
  
  void ResetBeforeUnloadHandlers();

  
  
  bool TabsNeedBeforeUnloadFired();

  
  bool HasCompletedUnloadProcessing() const;

  bool IsAttemptingToCloseBrowser() const;

  
  
  void OnWindowClosing();

  

  
  
  
  
  void InProgressDownloadResponse(bool cancel_downloads);

  
  
  
  
  
  DownloadClosePreventionType OkToCloseWithInProgressDownloads(
      int* num_downloads_blocking) const;

  

  
  
  
  void WindowFullscreenStateChanged();

  
  void VisibleSSLStateChanged(content::WebContents* web_contents);

  
  
  void OnWebContentsInstantSupportDisabled(
      const content::WebContents* web_contents);

  

  
  

  
  
  void ToggleFullscreenModeWithExtension(const GURL& extension_url);
#if defined(OS_WIN)
  
  void SetMetroSnapMode(bool enable);
#endif

  
  
  
  
  
  bool SupportsWindowFeature(WindowFeature feature) const;

  
  
  bool CanSupportWindowFeature(WindowFeature feature) const;

  
  
  void ToggleEncodingAutoDetect();
  void OverrideEncoding(int encoding_id);

  
  void OpenFile();

  void UpdateDownloadShelfVisibility(bool visible);

  

  
  
  void UpdateUIForNavigationInTab(content::WebContents* contents,
                                  content::PageTransition transition,
                                  bool user_initiated);

  

  
  virtual content::WebContents* OpenURL(
      const content::OpenURLParams& params) OVERRIDE;

  
  virtual void TabInsertedAt(content::WebContents* contents,
                             int index,
                             bool foreground) OVERRIDE;
  virtual void TabClosingAt(TabStripModel* tab_strip_model,
                            content::WebContents* contents,
                            int index) OVERRIDE;
  virtual void TabDetachedAt(content::WebContents* contents,
                             int index) OVERRIDE;
  virtual void TabDeactivated(content::WebContents* contents) OVERRIDE;
  virtual void ActiveTabChanged(content::WebContents* old_contents,
                                content::WebContents* new_contents,
                                int index,
                                int reason) OVERRIDE;
  virtual void TabMoved(content::WebContents* contents,
                        int from_index,
                        int to_index) OVERRIDE;
  virtual void TabReplacedAt(TabStripModel* tab_strip_model,
                             content::WebContents* old_contents,
                             content::WebContents* new_contents,
                             int index) OVERRIDE;
  virtual void TabPinnedStateChanged(content::WebContents* contents,
                                     int index) OVERRIDE;
  virtual void TabStripEmpty() OVERRIDE;

  
  virtual bool CanOverscrollContent() const OVERRIDE;
  virtual bool PreHandleKeyboardEvent(
      content::WebContents* source,
      const content::NativeWebKeyboardEvent& event,
      bool* is_keyboard_shortcut) OVERRIDE;
  virtual void HandleKeyboardEvent(
      content::WebContents* source,
      const content::NativeWebKeyboardEvent& event) OVERRIDE;
  virtual void OverscrollUpdate(int delta_y) OVERRIDE;
  virtual void ShowValidationMessage(content::WebContents* web_contents,
                                     const gfx::Rect& anchor_in_root_view,
                                     const string16& main_text,
                                     const string16& sub_text) OVERRIDE;
  virtual void HideValidationMessage(
      content::WebContents* web_contents) OVERRIDE;
  virtual void MoveValidationMessage(
      content::WebContents* web_contents,
      const gfx::Rect& anchor_in_root_view) OVERRIDE;

  bool is_type_tabbed() const { return type_ == TYPE_TABBED; }
  bool is_type_popup() const { return type_ == TYPE_POPUP; }

  bool is_app() const;
  bool is_devtools() const;

  
  bool IsMouseLocked() const;

  
  void OnWindowDidShow();

  
  void ShowFirstRunBubble();

  
  void ShowDownload(content::DownloadItem* download);

  FullscreenController* fullscreen_controller() const {
    return fullscreen_controller_.get();
  }

  extensions::WindowController* extension_window_controller() const {
    return extension_window_controller_.get();
  }

 private:
  friend class BrowserTest;
  friend class FullscreenControllerInteractiveTest;
  friend class FullscreenControllerTest;
  FRIEND_TEST_ALL_PREFIXES(AppModeTest, EnableAppModeTest);
  FRIEND_TEST_ALL_PREFIXES(BrowserCommandControllerTest,
                           IsReservedCommandOrKeyIsApp);
  FRIEND_TEST_ALL_PREFIXES(BrowserCommandControllerTest, AppFullScreen);
  FRIEND_TEST_ALL_PREFIXES(BrowserTest, NoTabsInPopups);
  FRIEND_TEST_ALL_PREFIXES(BrowserTest, ConvertTabToAppShortcut);
  FRIEND_TEST_ALL_PREFIXES(BrowserTest, OpenAppWindowLikeNtp);
  FRIEND_TEST_ALL_PREFIXES(BrowserTest, AppIdSwitch);
  FRIEND_TEST_ALL_PREFIXES(FullscreenControllerTest,
                           TabEntersPresentationModeFromWindowed);
  FRIEND_TEST_ALL_PREFIXES(FullscreenExitBubbleControllerTest,
                           DenyExitsFullscreen);
  FRIEND_TEST_ALL_PREFIXES(StartupBrowserCreatorTest, OpenAppShortcutNoPref);
  FRIEND_TEST_ALL_PREFIXES(StartupBrowserCreatorTest,
                           OpenAppShortcutWindowPref);
  FRIEND_TEST_ALL_PREFIXES(StartupBrowserCreatorTest, OpenAppShortcutTabPref);

  class InterstitialObserver;

  
  
  enum DetachType {
    
    DETACH_TYPE_DETACH,

    
    DETACH_TYPE_REPLACE,

    
    DETACH_TYPE_EMPTY
  };

  
  enum BookmarkBarStateChangeReason {
    
    BOOKMARK_BAR_STATE_CHANGE_INIT,

    
    BOOKMARK_BAR_STATE_CHANGE_TAB_SWITCH,

    
    BOOKMARK_BAR_STATE_CHANGE_PREF_CHANGE,

    
    BOOKMARK_BAR_STATE_CHANGE_TAB_STATE,

    
    BOOKMARK_BAR_STATE_CHANGE_TOGGLE_FULLSCREEN,
  };

  
  virtual content::WebContents* OpenURLFromTab(
      content::WebContents* source,
      const content::OpenURLParams& params) OVERRIDE;
  virtual void NavigationStateChanged(const content::WebContents* source,
                                      unsigned changed_flags) OVERRIDE;
  virtual void AddNewContents(content::WebContents* source,
                              content::WebContents* new_contents,
                              WindowOpenDisposition disposition,
                              const gfx::Rect& initial_pos,
                              bool user_gesture,
                              bool* was_blocked) OVERRIDE;
  virtual void ActivateContents(content::WebContents* contents) OVERRIDE;
  virtual void DeactivateContents(content::WebContents* contents) OVERRIDE;
  virtual void LoadingStateChanged(content::WebContents* source) OVERRIDE;
  virtual void CloseContents(content::WebContents* source) OVERRIDE;
  virtual void MoveContents(content::WebContents* source,
                            const gfx::Rect& pos) OVERRIDE;
  virtual bool IsPopupOrPanel(
      const content::WebContents* source) const OVERRIDE;
  virtual void UpdateTargetURL(content::WebContents* source, int32 page_id,
                               const GURL& url) OVERRIDE;
  virtual void ContentsMouseEvent(content::WebContents* source,
                                  const gfx::Point& location,
                                  bool motion) OVERRIDE;
  virtual void ContentsZoomChange(bool zoom_in) OVERRIDE;
  virtual void WebContentsFocused(content::WebContents* content) OVERRIDE;
  virtual bool TakeFocus(content::WebContents* source, bool reverse) OVERRIDE;
  virtual gfx::Rect GetRootWindowResizerRect() const OVERRIDE;
  virtual void BeforeUnloadFired(content::WebContents* source,
                                 bool proceed,
                                 bool* proceed_to_fire_unload) OVERRIDE;
  virtual bool ShouldFocusLocationBarByDefault(
      content::WebContents* source) OVERRIDE;
  virtual void SetFocusToLocationBar(bool select_all) OVERRIDE;
  virtual void RenderWidgetShowing() OVERRIDE;
  virtual int GetExtraRenderViewHeight() const OVERRIDE;
  virtual void ViewSourceForTab(content::WebContents* source,
                                const GURL& page_url) OVERRIDE;
  virtual void ViewSourceForFrame(
      content::WebContents* source,
      const GURL& frame_url,
      const content::PageState& frame_page_state) OVERRIDE;
  virtual void ShowRepostFormWarningDialog(
      content::WebContents* source) OVERRIDE;
  virtual bool ShouldCreateWebContents(
      content::WebContents* web_contents,
      int route_id,
      WindowContainerType window_container_type,
      const base::string16& frame_name,
      const GURL& target_url,
      const std::string& partition_id,
      content::SessionStorageNamespace* session_storage_namespace) OVERRIDE;
  virtual void WebContentsCreated(content::WebContents* source_contents,
                                  int64 source_frame_id,
                                  const base::string16& frame_name,
                                  const GURL& target_url,
                                  content::WebContents* new_contents) OVERRIDE;
  virtual void RendererUnresponsive(content::WebContents* source) OVERRIDE;
  virtual void RendererResponsive(content::WebContents* source) OVERRIDE;
  virtual void WorkerCrashed(content::WebContents* source) OVERRIDE;
  virtual void DidNavigateMainFramePostCommit(
      content::WebContents* web_contents) OVERRIDE;
  virtual void DidNavigateToPendingEntry(
      content::WebContents* web_contents) OVERRIDE;
  virtual content::JavaScriptDialogManager*
      GetJavaScriptDialogManager() OVERRIDE;
  virtual content::ColorChooser* OpenColorChooser(
      content::WebContents* web_contents,
      SkColor color,
      const std::vector<content::ColorSuggestion>& suggestions) OVERRIDE;
  virtual void RunFileChooser(
      content::WebContents* web_contents,
      const content::FileChooserParams& params) OVERRIDE;
  virtual void EnumerateDirectory(content::WebContents* web_contents,
                                  int request_id,
                                  const base::FilePath& path) OVERRIDE;
  virtual bool EmbedsFullscreenWidget() const OVERRIDE;
  virtual void ToggleFullscreenModeForTab(content::WebContents* web_contents,
      bool enter_fullscreen) OVERRIDE;
  virtual bool IsFullscreenForTabOrPending(
      const content::WebContents* web_contents) const OVERRIDE;
  virtual void JSOutOfMemory(content::WebContents* web_contents) OVERRIDE;
  virtual void RegisterProtocolHandler(content::WebContents* web_contents,
                                       const std::string& protocol,
                                       const GURL& url,
                                       const base::string16& title,
                                       bool user_gesture) OVERRIDE;
  virtual void UpdatePreferredSize(content::WebContents* source,
                                   const gfx::Size& pref_size) OVERRIDE;
  virtual void ResizeDueToAutoResize(content::WebContents* source,
                                     const gfx::Size& new_size) OVERRIDE;
  virtual void FindReply(content::WebContents* web_contents,
                         int request_id,
                         int number_of_matches,
                         const gfx::Rect& selection_rect,
                         int active_match_ordinal,
                         bool final_update) OVERRIDE;
  virtual void RequestToLockMouse(content::WebContents* web_contents,
                                  bool user_gesture,
                                  bool last_unlocked_by_target) OVERRIDE;
  virtual void LostMouseLock() OVERRIDE;
  virtual void RequestMediaAccessPermission(
      content::WebContents* web_contents,
      const content::MediaStreamRequest& request,
      const content::MediaResponseCallback& callback) OVERRIDE;
  virtual bool RequestPpapiBrokerPermission(
      content::WebContents* web_contents,
      const GURL& url,
      const base::FilePath& plugin_path,
      const base::Callback<void(bool)>& callback) OVERRIDE;
  virtual gfx::Size GetSizeForNewRenderView(
      const content::WebContents* web_contents) const OVERRIDE;

  
  
  virtual void SwapTabContents(content::WebContents* old_contents,
                               content::WebContents* new_contents) OVERRIDE;
  virtual bool CanReloadContents(
      content::WebContents* web_contents) const OVERRIDE;
  virtual bool CanSaveContents(
      content::WebContents* web_contents) const OVERRIDE;

  
  virtual void ConfirmAddSearchProvider(TemplateURL* template_url,
                                        Profile* profile) OVERRIDE;

  
  virtual void SetWebContentsBlocked(content::WebContents* web_contents,
                                     bool blocked) OVERRIDE;
  virtual web_modal::WebContentsModalDialogHost*
      GetWebContentsModalDialogHost() OVERRIDE;

  
  virtual void URLStarredChanged(content::WebContents* web_contents,
                                 bool starred) OVERRIDE;

  
  virtual void OnZoomChanged(content::WebContents* source,
                             bool can_show_bubble) OVERRIDE;

  
  virtual void FileSelected(const base::FilePath& path,
                            int index,
                            void* params) OVERRIDE;
  virtual void FileSelectedWithExtraInfo(
      const ui::SelectedFileInfo& file_info,
      int index,
      void* params) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  

  
  void OnDevToolsDisabledChanged();

  

  
  
  
  
  
  void UpdateToolbar(bool should_restore_state);

  
  
  
  
  
  
  void ScheduleUIUpdate(const content::WebContents* source,
                        unsigned changed_flags);

  
  
  void ProcessPendingUIUpdates();

  
  void RemoveScheduledUpdatesFor(content::WebContents* contents);

  

  
  
  friend class AutomationProvider;
  friend class BrowserProxy;
  friend class TestingAutomationProvider;

  
  
  
  StatusBubble* GetStatusBubble();

  

  
  
  void SyncHistoryWithTabs(int index);

  

  
  
  
  bool CanCloseWithInProgressDownloads();

  

  
  
  void SetAsDelegate(content::WebContents* web_contents, Browser* delegate);

  
  
  
  void FindInPage(bool find_next, bool forward_direction);

  
  
  
  void CloseFrame();

  void TabDetachedAtImpl(content::WebContents* contents,
                         int index,
                         DetachType type);

  
  void ReloadInternal(WindowOpenDisposition disposition, bool ignore_cache);

  
  
  content::WebContents* GetOrCloneTabForDisposition(
      WindowOpenDisposition disposition);

  
  
  
  
  bool SupportsWindowFeatureImpl(WindowFeature feature,
                                 bool check_fullscreen) const;

  
  
  void UpdateBookmarkBarState(BookmarkBarStateChangeReason reason);

  bool ShouldHideUIForFullscreen() const;

  
  
  bool MaybeCreateBackgroundContents(
      int route_id,
      content::WebContents* opener_web_contents,
      const base::string16& frame_name,
      const GURL& target_url,
      const std::string& partition_id,
      content::SessionStorageNamespace* session_storage_namespace);

  

  std::vector<InterstitialObserver*> interstitial_observers_;

  content::NotificationRegistrar registrar_;

  PrefChangeRegistrar profile_pref_registrar_;

  
  const Type type_;

  
  Profile* const profile_;

  
  BrowserWindow* window_;

  scoped_ptr<TabStripModelDelegate> tab_strip_model_delegate_;
  scoped_ptr<TabStripModel> tab_strip_model_;

  
  
  
  
  std::string app_name_;

  
  AppType app_type_;

  
  
  
  const SessionID session_id_;

  
  scoped_ptr<ToolbarModel> toolbar_model_;

  
  
  
  
  
  scoped_ptr<SearchModel> search_model_;

  

  typedef std::map<const content::WebContents*, int> UpdateMap;

  
  
  
  
  UpdateMap scheduled_updates_;

  

  enum CancelDownloadConfirmationState {
    NOT_PROMPTED,          
    WAITING_FOR_RESPONSE,  
                           
    RESPONSE_RECEIVED      
  };

  
  
  CancelDownloadConfirmationState cancel_download_confirmation_state_;

  

  
  
  
  
  
  
  gfx::Rect override_bounds_;
  ui::WindowShowState initial_show_state_;

  
  bool is_session_restore_;

  const chrome::HostDesktopType host_desktop_type_;

  scoped_ptr<chrome::UnloadController> unload_controller_;
  scoped_ptr<chrome::FastUnloadController> fast_unload_controller_;

  
  
  scoped_ptr<FindBarController> find_bar_controller_;

  
  scoped_refptr<ui::SelectFileDialog> select_file_dialog_;

  
  BooleanPrefMember encoding_auto_detect_;

  
  scoped_ptr<BrowserContentSettingBubbleModelDelegate>
      content_setting_bubble_model_delegate_;

  
  scoped_ptr<BrowserToolbarModelDelegate> toolbar_model_delegate_;

  
  
  scoped_ptr<SearchDelegate> search_delegate_;

  
  scoped_ptr<BrowserTabRestoreServiceDelegate> tab_restore_service_delegate_;

  
  scoped_ptr<BrowserSyncedWindowDelegate> synced_window_delegate_;

  scoped_ptr<BrowserInstantController> instant_controller_;

  BookmarkBar::State bookmark_bar_state_;

  scoped_ptr<FullscreenController> fullscreen_controller_;

  scoped_ptr<extensions::WindowController> extension_window_controller_;

  scoped_ptr<chrome::BrowserCommandController> command_controller_;

  
  bool window_has_shown_;

  
  base::WeakPtrFactory<Browser> chrome_updater_factory_;

  
  base::WeakPtrFactory<Browser> weak_factory_;

  scoped_ptr<BrowserLanguageStateObserver> language_state_observer_;

  scoped_ptr<chrome::ValidationMessageBubble> validation_message_bubble_;

  DISALLOW_COPY_AND_ASSIGN(Browser);
};

#endif  
