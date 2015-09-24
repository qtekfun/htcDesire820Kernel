// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_VIEW_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/timer.h"
#include "build/build_config.h"
#include "chrome/browser/tabs/tab_strip_model_observer.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/views/frame/browser_bubble_host.h"
#include "chrome/browser/ui/views/frame/browser_frame.h"
#include "chrome/browser/ui/views/infobars/infobar_container.h"
#include "chrome/browser/ui/views/tab_contents/tab_contents_container.h"
#include "chrome/browser/ui/views/tabs/abstract_tab_strip_view.h"
#include "chrome/browser/ui/views/unhandled_keyboard_event_handler.h"
#include "content/common/notification_registrar.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/gfx/native_widget_types.h"
#include "views/controls/single_split_view.h"
#include "views/window/client_view.h"
#include "views/window/window_delegate.h"

#if defined(OS_WIN)
#include "chrome/browser/hang_monitor/hung_plugin_action.h"
#include "chrome/browser/hang_monitor/hung_window_detector.h"
#include "views/controls/menu/native_menu_win.h"
#endif


class AccessiblePaneView;
class BookmarkBarView;
class Browser;
class BrowserBubble;
class BrowserViewLayout;
class ContentsContainer;
class DownloadShelfView;
class EncodingMenuModel;
class FullscreenExitBubble;
class HtmlDialogUIDelegate;
class InfoBarContainerView;
class LocationBarView;
class SideTabStrip;
class StatusBubbleViews;
class TabContentsContainer;
class TabStripModel;
class ToolbarView;
class ZoomMenuModel;
class Extension;

#if defined(OS_WIN)
class AeroPeekManager;
class JumpList;
#endif

namespace views {
class ExternalFocusTracker;
class Menu;
}

class BrowserView : public BrowserBubbleHost,
                    public BrowserWindow,
                    public BrowserWindowTesting,
                    public NotificationObserver,
                    public TabStripModelObserver,
                    public ui::SimpleMenuModel::Delegate,
                    public views::WindowDelegate,
                    public views::ClientView,
                    public InfoBarContainer::Delegate,
                    public views::SingleSplitView::Observer {
 public:
  
  static const char kViewClassName[];

  explicit BrowserView(Browser* browser);
  virtual ~BrowserView();

  void set_frame(BrowserFrame* frame) { frame_ = frame; }
  BrowserFrame* frame() const { return frame_; }

  
  
  
  static BrowserView* GetBrowserViewForNativeWindow(gfx::NativeWindow window);

  
  Browser* browser() const { return browser_.get(); }

  
  
  
  
  virtual gfx::Rect GetToolbarBounds() const;

  
  
  gfx::Rect GetClientAreaBounds() const;

  
  
  
  
  
  gfx::Rect GetFindBarBoundingBox() const;

  
  
  virtual int GetTabStripHeight() const;

  
  
  
  gfx::Point OffsetPointForToolbarBackgroundImage(
      const gfx::Point& point) const;

  
  int GetSidebarWidth() const;

  
  AbstractTabStripView* tabstrip() const { return tabstrip_; }

  
  ToolbarView* toolbar() const { return toolbar_; }

  
  virtual bool IsTabStripVisible() const;

  
  bool UseVerticalTabs() const;

  
  
  bool IsOffTheRecord() const;

  
  
  virtual bool ShouldShowOffTheRecordAvatar() const;

  
  virtual bool AcceleratorPressed(const views::Accelerator& accelerator);

  
  
  
  
  bool GetAccelerator(int cmd_id, ui::Accelerator* accelerator);

  
  
  
  bool ActivateAppModalDialog() const;

  
  
  
  
  
  
  TabContents* GetSelectedTabContents() const;
  TabContentsWrapper* GetSelectedTabContentsWrapper() const;

  
  SkBitmap GetOTRAvatarIcon();

#if defined(OS_WIN)
  
  
  void PrepareToRunSystemMenu(HMENU menu);
#endif

  
  
  bool IsBrowserTypeNormal() const {
    return browser_->type() == Browser::TYPE_NORMAL;
  }

  
  
  bool IsBrowserTypePanel() const {
    return browser_->type() == Browser::TYPE_APP_PANEL;
  }

  
  
  bool IsBrowserTypePopup() const {
    return (browser_->type() & Browser::TYPE_POPUP) != 0;
  }

  
  static void RegisterBrowserViewPrefs(PrefService* prefs);

  
  
  bool IsPositionInWindowCaption(const gfx::Point& point);

  
  virtual bool IsFullscreenBubbleVisible() const;

  
  
  void FullScreenStateChanged();

  
  
  void RestoreFocus();

  
  virtual void Show() OVERRIDE;
  virtual void ShowInactive() OVERRIDE;
  virtual void SetBounds(const gfx::Rect& bounds) OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual void Activate() OVERRIDE;
  virtual void Deactivate() OVERRIDE;
  virtual bool IsActive() const OVERRIDE;
  virtual void FlashFrame() OVERRIDE;
  virtual gfx::NativeWindow GetNativeHandle() OVERRIDE;
  virtual BrowserWindowTesting* GetBrowserWindowTesting() OVERRIDE;
  virtual StatusBubble* GetStatusBubble() OVERRIDE;
  virtual void ToolbarSizeChanged(bool is_animating) OVERRIDE;
  virtual void UpdateTitleBar() OVERRIDE;
  virtual void ShelfVisibilityChanged() OVERRIDE;
  virtual void UpdateDevTools() OVERRIDE;
  virtual void UpdateLoadingAnimations(bool should_animate) OVERRIDE;
  virtual void SetStarredState(bool is_starred) OVERRIDE;
  virtual gfx::Rect GetRestoredBounds() const OVERRIDE;
  virtual gfx::Rect GetBounds() const OVERRIDE;
  virtual bool IsMaximized() const OVERRIDE;
  virtual void SetFullscreen(bool fullscreen) OVERRIDE;
  virtual bool IsFullscreen() const OVERRIDE;
  virtual LocationBar* GetLocationBar() const OVERRIDE;
  virtual void SetFocusToLocationBar(bool select_all) OVERRIDE;
  virtual void UpdateReloadStopState(bool is_loading, bool force) OVERRIDE;
  virtual void UpdateToolbar(TabContentsWrapper* contents,
                             bool should_restore_state) OVERRIDE;
  virtual void FocusToolbar() OVERRIDE;
  virtual void FocusAppMenu() OVERRIDE;
  virtual void FocusBookmarksToolbar() OVERRIDE;
  virtual void FocusChromeOSStatus() OVERRIDE {}
  virtual void RotatePaneFocus(bool forwards) OVERRIDE;
  virtual void DestroyBrowser() OVERRIDE;
  virtual bool IsBookmarkBarVisible() const OVERRIDE;
  virtual bool IsBookmarkBarAnimating() const OVERRIDE;
  virtual bool IsTabStripEditable() const OVERRIDE;
  virtual bool IsToolbarVisible() const OVERRIDE;
  virtual void DisableInactiveFrame() OVERRIDE;
  virtual void ConfirmSetDefaultSearchProvider(
      TabContents* tab_contents,
      TemplateURL* template_url,
      TemplateURLModel* template_url_model) OVERRIDE;
  virtual void ConfirmAddSearchProvider(const TemplateURL* template_url,
                                        Profile* profile) OVERRIDE;
  virtual void ToggleBookmarkBar() OVERRIDE;
  virtual void ShowAboutChromeDialog() OVERRIDE;
  virtual void ShowUpdateChromeDialog() OVERRIDE;
  virtual void ShowTaskManager() OVERRIDE;
  virtual void ShowBackgroundPages() OVERRIDE;
  virtual void ShowBookmarkBubble(const GURL& url, bool already_bookmarked)
      OVERRIDE;
  
  void SetDownloadShelfVisible(bool visible);
  virtual bool IsDownloadShelfVisible() const OVERRIDE;
  virtual DownloadShelf* GetDownloadShelf() OVERRIDE;
  virtual void ShowRepostFormWarningDialog(TabContents* tab_contents) OVERRIDE;
  virtual void ShowCollectedCookiesDialog(TabContents* tab_contents) OVERRIDE;
  virtual void ShowThemeInstallBubble() OVERRIDE;
  virtual void ConfirmBrowserCloseWithPendingDownloads() OVERRIDE;
  virtual void ShowHTMLDialog(HtmlDialogUIDelegate* delegate,
                              gfx::NativeWindow parent_window) OVERRIDE;
  virtual void UserChangedTheme() OVERRIDE;
  virtual int GetExtraRenderViewHeight() const OVERRIDE;
  virtual void TabContentsFocused(TabContents* source) OVERRIDE;
  virtual void ShowPageInfo(Profile* profile,
                            const GURL& url,
                            const NavigationEntry::SSLStatus& ssl,
                            bool show_history) OVERRIDE;
  virtual void ShowAppMenu() OVERRIDE;
  virtual bool PreHandleKeyboardEvent(const NativeWebKeyboardEvent& event,
                                      bool* is_keyboard_shortcut) OVERRIDE;
  virtual void HandleKeyboardEvent(const NativeWebKeyboardEvent& event)
      OVERRIDE;
  virtual void ShowCreateWebAppShortcutsDialog(TabContentsWrapper* tab_contents)
      OVERRIDE;
  virtual void ShowCreateChromeAppShortcutsDialog(
      Profile*, const Extension* app) OVERRIDE;
  virtual void Cut() OVERRIDE;
  virtual void Copy() OVERRIDE;
  virtual void Paste() OVERRIDE;
  virtual void ToggleTabStripMode() OVERRIDE;
  virtual void PrepareForInstant() OVERRIDE;
  virtual void ShowInstant(TabContentsWrapper* preview) OVERRIDE;
  virtual void HideInstant(bool instant_is_active) OVERRIDE;
  virtual gfx::Rect GetInstantBounds() OVERRIDE;
#if defined(OS_CHROMEOS)
  virtual void ShowKeyboardOverlay(gfx::NativeWindow owning_window) OVERRIDE;
#endif

  
  virtual BookmarkBarView* GetBookmarkBarView() const OVERRIDE;
  virtual LocationBarView* GetLocationBarView() const OVERRIDE;
  virtual views::View* GetTabContentsContainerView() const OVERRIDE;
  virtual views::View* GetSidebarContainerView() const OVERRIDE;
  virtual ToolbarView* GetToolbarView() const OVERRIDE;

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  
  virtual void TabDetachedAt(TabContentsWrapper* contents, int index) OVERRIDE;
  virtual void TabDeselected(TabContentsWrapper* contents) OVERRIDE;
  virtual void TabSelectedAt(TabContentsWrapper* old_contents,
                             TabContentsWrapper* new_contents,
                             int index,
                             bool user_gesture) OVERRIDE;
  virtual void TabReplacedAt(TabStripModel* tab_strip_model,
                             TabContentsWrapper* old_contents,
                             TabContentsWrapper* new_contents,
                             int index) OVERRIDE;
  virtual void TabStripEmpty() OVERRIDE;

  
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id, ui::Accelerator* accelerator) OVERRIDE;
  virtual bool IsItemForCommandIdDynamic(int command_id) const OVERRIDE;
  virtual string16 GetLabelForCommandId(int command_id) const OVERRIDE;
  virtual void ExecuteCommand(int command_id) OVERRIDE;

  
  virtual bool CanResize() const OVERRIDE;
  virtual bool CanMaximize() const OVERRIDE;
  virtual bool CanActivate() const OVERRIDE;
  virtual bool IsModal() const OVERRIDE;
  virtual std::wstring GetWindowTitle() const OVERRIDE;
  virtual std::wstring GetAccessibleWindowTitle() const OVERRIDE;
  virtual views::View* GetInitiallyFocusedView() OVERRIDE;
  virtual bool ShouldShowWindowTitle() const OVERRIDE;
  virtual SkBitmap GetWindowAppIcon() OVERRIDE;
  virtual SkBitmap GetWindowIcon() OVERRIDE;
  virtual bool ShouldShowWindowIcon() const OVERRIDE;
  virtual bool ExecuteWindowsCommand(int command_id) OVERRIDE;
  virtual std::wstring GetWindowName() const OVERRIDE;
  virtual void SaveWindowPlacement(const gfx::Rect& bounds,
                                   bool maximized) OVERRIDE;
  virtual bool GetSavedWindowBounds(gfx::Rect* bounds) const OVERRIDE;
  virtual bool GetSavedMaximizedState(bool* maximized) const OVERRIDE;
  virtual views::View* GetContentsView() OVERRIDE;
  virtual views::ClientView* CreateClientView(views::Window* window) OVERRIDE;
  virtual void OnWindowActivationChanged(bool active) OVERRIDE;
  virtual void OnWindowBeginUserBoundsChange() OVERRIDE;
  virtual void OnWidgetMove() OVERRIDE;

  
  virtual bool CanClose() OVERRIDE;
  virtual int NonClientHitTest(const gfx::Point& point) OVERRIDE;
  virtual gfx::Size GetMinimumSize() OVERRIDE;

  
  virtual SkColor GetInfoBarSeparatorColor() const OVERRIDE;
  virtual void InfoBarContainerStateChanged(bool is_animating) OVERRIDE;
  virtual bool DrawInfoBarArrows(int* x) const OVERRIDE;

  
  virtual bool SplitHandleMoved(views::SingleSplitView* view) OVERRIDE;

 protected:
  
  
  
  virtual void GetAccessiblePanes(
      std::vector<AccessiblePaneView*>* panes);

  
  void SaveFocusedView();

  int last_focused_view_storage_id() const {
    return last_focused_view_storage_id_;
  }

  
  virtual std::string GetClassName() const OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void PaintChildren(gfx::Canvas* canvas) OVERRIDE;
  virtual void ViewHierarchyChanged(bool is_add,
                                    views::View* parent,
                                    views::View* child) OVERRIDE;
  virtual void ChildPreferredSizeChanged(View* child) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  
  
  virtual views::LayoutManager* CreateLayoutManager() const;

  
  
  
  virtual void InitTabStrip(TabStripModel* tab_strip_model);

  
  
  
  virtual ToolbarView* CreateToolbar() const;

  
  virtual void Init();

  
  virtual void LoadingAnimationCallback();

 private:
  friend class BrowserViewLayout;
  FRIEND_TEST_ALL_PREFIXES(BrowserViewsAccessibilityTest,
                           TestAboutChromeViewAccObj);

#if defined(OS_WIN)
  
  void InitSystemMenu();
#endif

  
  BrowserViewLayout* GetBrowserViewLayout() const;

  
  void LayoutStatusBubble();

  
  
  
  
  bool MaybeShowBookmarkBar(TabContentsWrapper* contents);

  
  
  
  
  bool MaybeShowInfoBar(TabContentsWrapper* contents);

  
  void UpdateSidebar();
  
  
  void UpdateSidebarForContents(TabContentsWrapper* tab_contents);

  
  void UpdateDevToolsForContents(TabContentsWrapper* tab_contents);

  
  
  
  
  void UpdateUIForContents(TabContentsWrapper* contents);

  
  
  
  
  
  bool UpdateChildViewAndLayout(views::View* new_view, views::View** old_view);

  
  
  
  
  
  void ProcessFullscreen(bool fullscreen);

  
  
  void LoadAccelerators();

#if defined(OS_WIN)
  
  void BuildSystemMenuForBrowserWindow();
  void BuildSystemMenuForAppOrPopupWindow(bool is_app);
#endif

  
  int GetCommandIDForAppCommandID(int app_command_id) const;

  
  void InitHangMonitor();

  
  
  
  void UpdateAcceleratorMetrics(const views::Accelerator& accelerator,
                                int command_id);

  
  
  
  
  void ProcessTabSelected(TabContentsWrapper* new_contents,
                          bool change_tab_contents);

  
  gfx::Size GetResizeCornerSize() const;

  
  views::Window* DoShowAboutChromeDialog();

  
  void SetToolbar(ToolbarView* toolbar);

  
  int last_focused_view_storage_id_;

  
  BrowserFrame* frame_;

  
  scoped_ptr<Browser> browser_;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  
  
  views::View* active_bookmark_bar_;

  
  AbstractTabStripView* tabstrip_;

  
  ToolbarView* toolbar_;

  
  scoped_ptr<BookmarkBarView> bookmark_bar_view_;

  
  scoped_ptr<DownloadShelfView> download_shelf_;

  
  InfoBarContainerView* infobar_container_;

  
  TabContentsContainer* sidebar_container_;

  
  views::SingleSplitView* sidebar_split_;

  
  TabContentsContainer* contents_container_;

  
  TabContentsContainer* devtools_container_;

  
  TabContentsContainer* preview_container_;

  
  ContentsContainer* contents_;

  
  views::SingleSplitView* contents_split_;

  
  
  
  scoped_ptr<views::ExternalFocusTracker> devtools_focus_tracker_;

  
  scoped_ptr<StatusBubbleViews> status_bubble_;

  
  std::map<views::Accelerator, int> accelerator_table_;

  
  bool initialized_;

  
  
  bool ignore_layout_;

  scoped_ptr<FullscreenExitBubble> fullscreen_bubble_;

#if defined(OS_WIN)
  
  scoped_ptr<views::SystemMenuModel> system_menu_contents_;
  scoped_ptr<ZoomMenuModel> zoom_menu_contents_;
  scoped_ptr<EncodingMenuModel> encoding_menu_contents_;
  
  scoped_ptr<views::NativeMenuWin> system_menu_;

  
  
  WorkerThreadTicker ticker_;

  
  
  
  HungWindowDetector hung_window_detector_;

  
  
  HungPluginAction hung_plugin_action_;

  
  scoped_ptr<JumpList> jumplist_;

  
  scoped_ptr<AeroPeekManager> aeropeek_manager_;
#endif

  
  base::RepeatingTimer<BrowserView> loading_animation_timer_;

  UnhandledKeyboardEventHandler unhandled_keyboard_event_handler_;

  NotificationRegistrar registrar_;

  
  base::TimeTicks last_animation_time_;

  DISALLOW_COPY_AND_ASSIGN(BrowserView);
};

#endif  
