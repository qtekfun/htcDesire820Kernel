// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_VIEW_H_

#include <map>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer/timer.h"
#include "build/build_config.h"
#include "chrome/browser/devtools/devtools_window.h"
#include "chrome/browser/infobars/infobar_container.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/omnibox/omnibox_popup_model_observer.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "chrome/browser/ui/views/frame/browser_frame.h"
#include "chrome/browser/ui/views/frame/immersive_mode_controller.h"
#include "chrome/browser/ui/views/frame/scroll_end_effect_controller.h"
#include "chrome/browser/ui/views/load_complete_listener.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/sys_color_change_listener.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/single_split_view_listener.h"
#include "ui/views/controls/webview/unhandled_keyboard_event_handler.h"
#include "ui/views/widget/widget_delegate.h"
#include "ui/views/widget/widget_observer.h"
#include "ui/views/window/client_view.h"

#if defined(OS_WIN)
#include "chrome/browser/hang_monitor/hung_plugin_action.h"
#include "chrome/browser/hang_monitor/hung_window_detector.h"
#endif


class BookmarkBarView;
class Browser;
class BrowserViewLayout;
class ContentsContainer;
class DownloadShelfView;
class FullscreenExitBubbleViews;
class InfoBarContainerView;
class LocationBarView;
class StatusBubbleViews;
class SearchViewController;
class TabStrip;
class TabStripModel;
class ToolbarView;
class TopContainerView;

#if defined(OS_WIN)
class JumpList;
#endif

namespace autofill {
class PasswordGenerator;
}

namespace content {
class RenderWidgetHost;
}

namespace extensions {
class Extension;
}

namespace views {
class AccessiblePaneView;
class ExternalFocusTracker;
class WebView;
}

class BrowserView : public BrowserWindow,
                    public BrowserWindowTesting,
                    public TabStripModelObserver,
                    public ui::AcceleratorProvider,
                    public views::WidgetDelegate,
                    public views::WidgetObserver,
                    public views::ClientView,
                    public InfoBarContainer::Delegate,
                    public views::SingleSplitViewListener,
                    public gfx::SysColorChangeListener,
                    public LoadCompleteListener::Delegate,
                    public OmniboxPopupModelObserver {
 public:
  
  static const char kViewClassName[];

  BrowserView();
  virtual ~BrowserView();

  
  void Init(Browser* browser);

  void set_frame(BrowserFrame* frame) { frame_ = frame; }
  BrowserFrame* frame() const { return frame_; }

  
  
  
  
  
  
  static BrowserView* GetBrowserViewForNativeWindow(gfx::NativeWindow window);

  
  static BrowserView* GetBrowserViewForBrowser(const Browser* browser);

  
  Browser* browser() { return browser_.get(); }

  
  
  
  
  void InitStatusBubble();

  
  
  
  
  gfx::Rect GetToolbarBounds() const;

  
  
  
  
  
  gfx::Rect GetFindBarBoundingBox() const;

  
  
  int GetTabStripHeight() const;

  
  
  
  gfx::Point OffsetPointForToolbarBackgroundImage(
      const gfx::Point& point) const;

  
  TopContainerView* top_container() { return top_container_; }

  
  TabStrip* tabstrip() { return tabstrip_; }

  
  ToolbarView* toolbar() { return toolbar_; }

  
  BookmarkBarView* bookmark_bar() { return bookmark_bar_view_.get(); }

  
  
  
  
  
  View* find_bar_host_view() { return find_bar_host_view_; }

  
  InfoBarContainerView* infobar_container() { return infobar_container_; }

  
  FullscreenExitBubbleViews* fullscreen_exit_bubble() {
    return fullscreen_bubble_.get();
  }

  
  bool IsTabStripVisible() const;

  
  
  bool IsOffTheRecord() const;

  
  
  bool IsGuestSession() const;

  
  
  bool IsRegularOrGuestSession() const;

  
  
  int GetOTRIconResourceID() const;

  
  
  int GetGuestIconResourceID() const;

  
  bool ShouldShowAvatar() const;

  
  
  
  
  bool GetAccelerator(int cmd_id, ui::Accelerator* accelerator);

  
  bool IsAcceleratorRegistered(const ui::Accelerator& accelerator);

  
  
  
  
  
  
  content::WebContents* GetActiveWebContents() const;

  
  gfx::ImageSkia GetOTRAvatarIcon() const;

  
  
  bool IsBrowserTypeNormal() const {
    return browser_->is_type_tabbed();
  }

  
  ImmersiveModeController* immersive_mode_controller() const {
    return immersive_mode_controller_.get();
  }

  
  
  void RestoreFocus();

  void SetWindowSwitcherButton(views::Button* button);

  views::Button* window_switcher_button() {
    return window_switcher_button_;
  }

  
  
  
  void FullscreenStateChanged();

  
  
  void ToolbarSizeChanged(bool is_animating);

  
  virtual void Show() OVERRIDE;
  virtual void ShowInactive() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual void SetBounds(const gfx::Rect& bounds) OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual void Activate() OVERRIDE;
  virtual void Deactivate() OVERRIDE;
  virtual bool IsActive() const OVERRIDE;
  virtual void FlashFrame(bool flash) OVERRIDE;
  virtual bool IsAlwaysOnTop() const OVERRIDE;
  virtual void SetAlwaysOnTop(bool always_on_top) OVERRIDE;
  virtual gfx::NativeWindow GetNativeWindow() OVERRIDE;
  virtual BrowserWindowTesting* GetBrowserWindowTesting() OVERRIDE;
  virtual StatusBubble* GetStatusBubble() OVERRIDE;
  virtual void UpdateTitleBar() OVERRIDE;
  virtual void BookmarkBarStateChanged(
      BookmarkBar::AnimateChangeType change_type) OVERRIDE;
  virtual void UpdateDevTools() OVERRIDE;
  virtual void UpdateLoadingAnimations(bool should_animate) OVERRIDE;
  virtual void SetStarredState(bool is_starred) OVERRIDE;
  virtual void SetTranslateIconToggled(bool is_lit) OVERRIDE;
  virtual void OnActiveTabChanged(content::WebContents* old_contents,
                                  content::WebContents* new_contents,
                                  int index,
                                  int reason) OVERRIDE;
  virtual void ZoomChangedForActiveTab(bool can_show_bubble) OVERRIDE;
  virtual gfx::Rect GetRestoredBounds() const OVERRIDE;
  virtual ui::WindowShowState GetRestoredState() const OVERRIDE;
  virtual gfx::Rect GetBounds() const OVERRIDE;
  virtual bool IsMaximized() const OVERRIDE;
  virtual bool IsMinimized() const OVERRIDE;
  virtual void Maximize() OVERRIDE;
  virtual void Minimize() OVERRIDE;
  virtual void Restore() OVERRIDE;
  virtual void EnterFullscreen(
      const GURL& url, FullscreenExitBubbleType bubble_type) OVERRIDE;
  virtual void ExitFullscreen() OVERRIDE;
  virtual void UpdateFullscreenExitBubbleContent(
      const GURL& url,
      FullscreenExitBubbleType bubble_type) OVERRIDE;
  virtual bool ShouldHideUIForFullscreen() const OVERRIDE;
  virtual bool IsFullscreen() const OVERRIDE;
  virtual bool IsFullscreenBubbleVisible() const OVERRIDE;
#if defined(OS_WIN)
  virtual void SetMetroSnapMode(bool enable) OVERRIDE;
  virtual bool IsInMetroSnapMode() const OVERRIDE;
#endif
  virtual LocationBar* GetLocationBar() const OVERRIDE;
  virtual void SetFocusToLocationBar(bool select_all) OVERRIDE;
  virtual void UpdateReloadStopState(bool is_loading, bool force) OVERRIDE;
  virtual void UpdateToolbar(content::WebContents* contents) OVERRIDE;
  virtual void FocusToolbar() OVERRIDE;
  virtual void FocusAppMenu() OVERRIDE;
  virtual void FocusBookmarksToolbar() OVERRIDE;
  virtual void FocusInfobars() OVERRIDE;
  virtual void RotatePaneFocus(bool forwards) OVERRIDE;
  virtual void DestroyBrowser() OVERRIDE;
  virtual bool IsBookmarkBarVisible() const OVERRIDE;
  virtual bool IsBookmarkBarAnimating() const OVERRIDE;
  virtual bool IsTabStripEditable() const OVERRIDE;
  virtual bool IsToolbarVisible() const OVERRIDE;
  virtual gfx::Rect GetRootWindowResizerRect() const OVERRIDE;
  virtual void DisableInactiveFrame() OVERRIDE;
  virtual void ConfirmAddSearchProvider(TemplateURL* template_url,
                                        Profile* profile) OVERRIDE;
  virtual void ShowUpdateChromeDialog() OVERRIDE;
  virtual void ShowBookmarkBubble(const GURL& url,
                                  bool already_bookmarked) OVERRIDE;
  virtual void ShowBookmarkPrompt() OVERRIDE;
  virtual void ShowTranslateBubble(
      content::WebContents* contents,
      TranslateBubbleModel::ViewState view_state,
      TranslateErrors::Type error_type) OVERRIDE;
#if defined(ENABLE_ONE_CLICK_SIGNIN)
  virtual void ShowOneClickSigninBubble(
      OneClickSigninBubbleType type,
      const base::string16& email,
      const base::string16& error_message,
      const StartSyncCallback& start_sync_callback) OVERRIDE;
#endif
  
  void SetDownloadShelfVisible(bool visible);
  virtual bool IsDownloadShelfVisible() const OVERRIDE;
  virtual DownloadShelf* GetDownloadShelf() OVERRIDE;
  virtual void ConfirmBrowserCloseWithPendingDownloads(
      int download_count,
      Browser::DownloadClosePreventionType dialog_type,
      bool app_modal,
      const base::Callback<void(bool)>& callback) OVERRIDE;
  virtual void UserChangedTheme() OVERRIDE;
  virtual int GetExtraRenderViewHeight() const OVERRIDE;
  virtual void WebContentsFocused(content::WebContents* contents) OVERRIDE;
  virtual void ShowWebsiteSettings(Profile* profile,
                                   content::WebContents* web_contents,
                                   const GURL& url,
                                   const content::SSLStatus& ssl) OVERRIDE;
  virtual void ShowAppMenu() OVERRIDE;
  virtual bool PreHandleKeyboardEvent(
      const content::NativeWebKeyboardEvent& event,
      bool* is_keyboard_shortcut) OVERRIDE;
  virtual void HandleKeyboardEvent(
      const content::NativeWebKeyboardEvent& event) OVERRIDE;
  virtual void Cut() OVERRIDE;
  virtual void Copy() OVERRIDE;
  virtual void Paste() OVERRIDE;
  virtual WindowOpenDisposition GetDispositionForPopupBounds(
      const gfx::Rect& bounds) OVERRIDE;
  virtual FindBar* CreateFindBar() OVERRIDE;
  virtual web_modal::WebContentsModalDialogHost*
      GetWebContentsModalDialogHost() OVERRIDE;
  virtual void ShowAvatarBubble(content::WebContents* web_contents,
                                const gfx::Rect& rect) OVERRIDE;
  virtual void ShowAvatarBubbleFromAvatarButton() OVERRIDE;
  virtual void ShowPasswordGenerationBubble(
      const gfx::Rect& rect,
      const autofill::PasswordForm& form,
      autofill::PasswordGenerator* password_generator) OVERRIDE;
  virtual void OverscrollUpdate(int delta_y) OVERRIDE;
  virtual int GetRenderViewHeightInsetWithDetachedBookmarkBar() OVERRIDE;

  
  virtual BookmarkBarView* GetBookmarkBarView() const OVERRIDE;
  virtual LocationBarView* GetLocationBarView() const OVERRIDE;
  virtual views::View* GetTabContentsContainerView() const OVERRIDE;
  virtual ToolbarView* GetToolbarView() const OVERRIDE;

  
  virtual void TabDetachedAt(content::WebContents* contents,
                             int index) OVERRIDE;
  virtual void TabDeactivated(content::WebContents* contents) OVERRIDE;
  virtual void TabStripEmpty() OVERRIDE;

  
  virtual bool GetAcceleratorForCommandId(int command_id,
      ui::Accelerator* accelerator) OVERRIDE;

  
  virtual bool CanResize() const OVERRIDE;
  virtual bool CanMaximize() const OVERRIDE;
  virtual bool CanActivate() const OVERRIDE;
  virtual base::string16 GetWindowTitle() const OVERRIDE;
  virtual base::string16 GetAccessibleWindowTitle() const OVERRIDE;
  virtual views::View* GetInitiallyFocusedView() OVERRIDE;
  virtual bool ShouldShowWindowTitle() const OVERRIDE;
  virtual gfx::ImageSkia GetWindowAppIcon() OVERRIDE;
  virtual gfx::ImageSkia GetWindowIcon() OVERRIDE;
  virtual bool ShouldShowWindowIcon() const OVERRIDE;
  virtual bool ExecuteWindowsCommand(int command_id) OVERRIDE;
  virtual std::string GetWindowName() const OVERRIDE;
  virtual void SaveWindowPlacement(const gfx::Rect& bounds,
                                   ui::WindowShowState show_state) OVERRIDE;
  virtual bool GetSavedWindowPlacement(
      const views::Widget* widget,
      gfx::Rect* bounds,
      ui::WindowShowState* show_state) const OVERRIDE;
  virtual views::View* GetContentsView() OVERRIDE;
  virtual views::ClientView* CreateClientView(views::Widget* widget) OVERRIDE;
  virtual void OnWindowBeginUserBoundsChange() OVERRIDE;
  virtual void OnWidgetMove() OVERRIDE;
  virtual views::Widget* GetWidget() OVERRIDE;
  virtual const views::Widget* GetWidget() const OVERRIDE;
  virtual void GetAccessiblePanes(std::vector<View*>* panes) OVERRIDE;

  
  virtual void OnWidgetActivationChanged(views::Widget* widget,
                                         bool active) OVERRIDE;

  
  virtual bool CanClose() OVERRIDE;
  virtual int NonClientHitTest(const gfx::Point& point) OVERRIDE;
  virtual gfx::Size GetMinimumSize() OVERRIDE;

  
  virtual SkColor GetInfoBarSeparatorColor() const OVERRIDE;
  virtual void InfoBarContainerStateChanged(bool is_animating) OVERRIDE;
  virtual bool DrawInfoBarArrows(int* x) const OVERRIDE;

  
  virtual bool SplitHandleMoved(views::SingleSplitView* sender) OVERRIDE;

  
  virtual void OnSysColorChange() OVERRIDE;

  
  virtual const char* GetClassName() const OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void PaintChildren(gfx::Canvas* canvas) OVERRIDE;
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;
  virtual void ChildPreferredSizeChanged(View* child) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;

  
  virtual void OnOmniboxPopupShownOrHidden() OVERRIDE;

  
  views::SingleSplitView* GetContentsSplitForTest() { return contents_split_; }
  ContentsContainer* GetContentsContainerForTest() {
    return contents_container_;
  }
  views::WebView* GetContentsWebViewForTest() { return contents_web_view_; }

 private:
  
  
  friend class BrowserViewLayoutDelegateImpl;
  FRIEND_TEST_ALL_PREFIXES(BrowserViewTest, BrowserView);
  FRIEND_TEST_ALL_PREFIXES(BrowserViewsAccessibilityTest,
                           TestAboutChromeViewAccObj);

  enum FullscreenType {
    FOR_DESKTOP,
    FOR_METRO
  };

  
  
  void GetAccessiblePanes(std::vector<views::AccessiblePaneView*>* panes);

  
  void InitViews();

  
  void LoadingAnimationCallback();

  
  
  virtual void OnLoadCompleted() OVERRIDE;

  
  BrowserViewLayout* GetBrowserViewLayout() const;

  
  void LayoutStatusBubble();

  
  
  
  
  bool MaybeShowBookmarkBar(content::WebContents* contents);

  
  
  
  void SetBookmarkBarParent(views::View* new_parent);

  
  
  
  
  bool MaybeShowInfoBar(content::WebContents* contents);

  
  
  
  
  void UpdateDevToolsForContents(content::WebContents* web_contents);

  
  void ShowDevToolsContainer();

  
  void HideDevToolsContainer();

  
  
  void UpdateDevToolsSplitPosition();

  
  
  
  
  void UpdateUIForContents(content::WebContents* contents);

  
  
  
  
  
  
  
  
  
  void ProcessFullscreen(bool fullscreen,
                         FullscreenType fullscreen_type,
                         const GURL& url,
                         FullscreenExitBubbleType bubble_type);

  
  
  
  bool ShouldUseImmersiveFullscreenForUrl(const GURL& url) const;

  
  
  void LoadAccelerators();

  
  int GetCommandIDForAppCommandID(int app_command_id) const;

  
  void InitHangMonitor();

  
  
  
  void UpdateAcceleratorMetrics(const ui::Accelerator& accelerator,
                                int command_id);

  
  
  
  void DoCutCopyPaste(void (content::RenderWidgetHost::*method)(),
                      int command_id);

  
  
  bool DoCutCopyPasteForWebContents(
      content::WebContents* contents,
      void (content::RenderWidgetHost::*method)());

  
  
  void ActivateAppModalDialog() const;

  
  int GetMaxTopInfoBarArrowHeight();

  
  int last_focused_view_storage_id_;

  
  BrowserFrame* frame_;

  
  scoped_ptr<Browser> browser_;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  
  
  TopContainerView* top_container_;

  
  TabStrip* tabstrip_;

  
  ToolbarView* toolbar_;

  
  
  
  views::Button* window_switcher_button_;

  
  
  scoped_ptr<BookmarkBarView> bookmark_bar_view_;

  
  
  
  View* find_bar_host_view_;

  
  scoped_ptr<DownloadShelfView> download_shelf_;

  
  InfoBarContainerView* infobar_container_;

  
  views::WebView* contents_web_view_;

  
  views::WebView* devtools_container_;

  
  ContentsContainer* contents_container_;

  
  views::SingleSplitView* contents_split_;

  
  DevToolsDockSide devtools_dock_side_;

  
  
  DevToolsWindow* devtools_window_;

  
  
  
  scoped_ptr<views::ExternalFocusTracker> devtools_focus_tracker_;

  
  scoped_ptr<StatusBubbleViews> status_bubble_;

  
  std::map<ui::Accelerator, int> accelerator_table_;

  
  bool initialized_;

  
  
  
  bool in_process_fullscreen_;

  scoped_ptr<FullscreenExitBubbleViews> fullscreen_bubble_;

#if defined(OS_WIN)
  
  
  WorkerThreadTicker ticker_;

  
  
  
  HungWindowDetector hung_window_detector_;

  
  
  HungPluginAction hung_plugin_action_;

  
  scoped_ptr<LoadCompleteListener> load_complete_listener_;

  
  scoped_refptr<JumpList> jumplist_;
#endif

  
  base::RepeatingTimer<BrowserView> loading_animation_timer_;

  views::UnhandledKeyboardEventHandler unhandled_keyboard_event_handler_;

  
  base::TimeTicks last_animation_time_;

  
  
  bool force_location_bar_focus_;

  scoped_ptr<ImmersiveModeController> immersive_mode_controller_;

  scoped_ptr<ScrollEndEffectController> scroll_end_effect_controller_;

  gfx::ScopedSysColorChangeListener color_change_listener_;

  mutable base::WeakPtrFactory<BrowserView> activate_modal_dialog_factory_;

  DISALLOW_COPY_AND_ASSIGN(BrowserView);
};

#endif  
