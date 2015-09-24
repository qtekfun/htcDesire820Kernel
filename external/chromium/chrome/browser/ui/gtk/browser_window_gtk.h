// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_BROWSER_WINDOW_GTK_H_
#define CHROME_BROWSER_UI_GTK_BROWSER_WINDOW_GTK_H_
#pragma once

#include <gtk/gtk.h>

#include <map>

#include "base/memory/scoped_ptr.h"
#include "base/timer.h"
#include "build/build_config.h"
#include "chrome/browser/prefs/pref_member.h"
#include "chrome/browser/tabs/tab_strip_model_observer.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/gtk/infobars/infobar_arrow_model.h"
#include "content/common/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/x/active_window_watcher_x.h"
#include "ui/base/x/x11_util.h"
#include "ui/gfx/rect.h"

class BookmarkBarGtk;
class Browser;
class BrowserTitlebar;
class BrowserToolbarGtk;
class CustomDrawButton;
class DownloadShelfGtk;
class FindBarGtk;
class FullscreenExitBubbleGtk;
class GlobalMenuBar;
class InfoBarContainerGtk;
class LocationBar;
class StatusBubbleGtk;
class TabContentsContainerGtk;
class TabStripGtk;


class BrowserWindowGtk : public BrowserWindow,
                         public NotificationObserver,
                         public TabStripModelObserver,
                         public ui::ActiveWindowWatcherX::Observer,
                         public InfoBarArrowModel::Observer {
 public:
  explicit BrowserWindowGtk(Browser* browser);
  virtual ~BrowserWindowGtk();

  virtual void Init();

  
  virtual void Show();
  virtual void ShowInactive();
  virtual void SetBounds(const gfx::Rect& bounds);
  virtual void Close();
  virtual void Activate();
  virtual void Deactivate();
  virtual bool IsActive() const;
  virtual void FlashFrame();
  virtual gfx::NativeWindow GetNativeHandle();
  virtual BrowserWindowTesting* GetBrowserWindowTesting();
  virtual StatusBubble* GetStatusBubble();
  virtual void ToolbarSizeChanged(bool is_animating);
  virtual void UpdateTitleBar();
  virtual void ShelfVisibilityChanged();
  virtual void UpdateDevTools();
  virtual void UpdateLoadingAnimations(bool should_animate);
  virtual void SetStarredState(bool is_starred);
  virtual gfx::Rect GetRestoredBounds() const;
  virtual gfx::Rect GetBounds() const;
  virtual bool IsMaximized() const;
  virtual void SetFullscreen(bool fullscreen);
  virtual bool IsFullscreen() const;
  virtual bool IsFullscreenBubbleVisible() const;
  virtual LocationBar* GetLocationBar() const;
  virtual void SetFocusToLocationBar(bool select_all);
  virtual void UpdateReloadStopState(bool is_loading, bool force);
  virtual void UpdateToolbar(TabContentsWrapper* contents,
                             bool should_restore_state);
  virtual void FocusToolbar();
  virtual void FocusAppMenu();
  virtual void FocusBookmarksToolbar();
  virtual void FocusChromeOSStatus();
  virtual void RotatePaneFocus(bool forwards);
  virtual bool IsBookmarkBarVisible() const;
  virtual bool IsBookmarkBarAnimating() const;
  virtual bool IsTabStripEditable() const;
  virtual bool IsToolbarVisible() const;
  virtual void ConfirmAddSearchProvider(const TemplateURL* template_url,
                                        Profile* profile);
  virtual void ToggleBookmarkBar();
  virtual void ShowAboutChromeDialog();
  virtual void ShowUpdateChromeDialog();
  virtual void ShowTaskManager();
  virtual void ShowBackgroundPages();
  virtual void ShowBookmarkBubble(const GURL& url, bool already_bookmarked);
  virtual bool IsDownloadShelfVisible() const;
  virtual DownloadShelf* GetDownloadShelf();
  virtual void ShowRepostFormWarningDialog(TabContents* tab_contents);
  virtual void ShowCollectedCookiesDialog(TabContents* tab_contents);
  virtual void ShowThemeInstallBubble();
  virtual void ConfirmBrowserCloseWithPendingDownloads();
  virtual void ShowHTMLDialog(HtmlDialogUIDelegate* delegate,
                              gfx::NativeWindow parent_window);
  virtual void UserChangedTheme();
  virtual int GetExtraRenderViewHeight() const;
  virtual void TabContentsFocused(TabContents* tab_contents);
  virtual void ShowPageInfo(Profile* profile,
                            const GURL& url,
                            const NavigationEntry::SSLStatus& ssl,
                            bool show_history);
  virtual void ShowAppMenu();
  virtual bool PreHandleKeyboardEvent(const NativeWebKeyboardEvent& event,
                                      bool* is_keyboard_shortcut);
  virtual void HandleKeyboardEvent(const NativeWebKeyboardEvent& event);
  virtual void ShowCreateWebAppShortcutsDialog(
      TabContentsWrapper* tab_contents);
  virtual void ShowCreateChromeAppShortcutsDialog(Profile* profile,
                                                  const Extension* app);
  virtual void Cut();
  virtual void Copy();
  virtual void Paste();
  virtual void ToggleTabStripMode() {}
  virtual void PrepareForInstant();
  virtual void ShowInstant(TabContentsWrapper* preview);
  virtual void HideInstant(bool instant_is_active);
  virtual gfx::Rect GetInstantBounds();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual void TabDetachedAt(TabContentsWrapper* contents, int index);
  virtual void TabSelectedAt(TabContentsWrapper* old_contents,
                             TabContentsWrapper* new_contents,
                             int index,
                             bool user_gesture);

  
  virtual void ActiveWindowChanged(GdkWindow* active_window);

  
  virtual void PaintStateChanged();

  
  TabStripGtk* tabstrip() const { return tabstrip_.get(); }

  void UpdateDevToolsForContents(TabContents* contents);

  void OnDebouncedBoundsChanged();

  
  
  void UnMaximize();

  
  
  bool CanClose() const;

  bool ShouldShowWindowIcon() const;

  
  
  void DisableDebounceTimerForTests(bool is_disabled);

  
  void AddFindBar(FindBarGtk* findbar);

  
  
  void ResetCustomFrameCursor();

  
  
  void SetInfoBarShowing(InfoBar* bar, bool animate);

  
  static BrowserWindowGtk* GetBrowserWindowForNativeWindow(
      gfx::NativeWindow window);

  
  
  static GtkWindow* GetBrowserWindowForXID(XID xid);

  Browser* browser() const { return browser_.get(); }

  GtkWindow* window() const { return window_; }

  BrowserToolbarGtk* GetToolbar() { return toolbar_.get(); }

  gfx::Rect bounds() const { return bounds_; }

  
  
  void BookmarkBarIsFloating(bool is_floating);

  
  
  TabContents* GetDisplayedTabContents();

  static void RegisterUserPrefs(PrefService* prefs);

  
  
  
  bool ShouldDrawContentDropShadow();

  
  
  
  void QueueToolbarRedraw();

  
  
  int GetXPositionOfLocationIcon(GtkWidget* relative_to);

 protected:
  virtual void DestroyBrowser();
  
  GtkWindow* window_;
  
  
  GtkWidget* window_container_;
  
  GtkWidget* window_vbox_;
  
  GtkWidget* render_area_vbox_;
  
  
  GtkWidget* render_area_floating_container_;
  
  GtkWidget* render_area_event_box_;
  
  GtkWidget* toolbar_border_;

  scoped_ptr<Browser> browser_;

  
  scoped_ptr<DownloadShelfGtk> download_shelf_;

 private:
  
  
  void FadeForInstant(bool animate);

  
  void CancelInstantFade();

  
  void MaybeShowBookmarkBar(bool animate);

  
  
  void SetGeometryHints();

  
  void ConnectHandlersToSignals();

  
  void InitWidgets();

  
  
  void SetBackgroundColor();

  
  void OnSizeChanged(int width, int height);

  
  void UpdateWindowShape(int width, int height);

  
  
  void ConnectAccelerators();

  
  
  
  void UpdateCustomFrame();

  
  void SaveWindowPosition();

  
  
  
  
  void SetBoundsImpl(const gfx::Rect& bounds, bool exterior, bool move);

  CHROMEGTK_CALLBACK_1(BrowserWindowGtk, gboolean, OnConfigure,
                       GdkEventConfigure*);
  CHROMEGTK_CALLBACK_1(BrowserWindowGtk, gboolean, OnWindowState,
                       GdkEventWindowState*);
  CHROMEGTK_CALLBACK_1(BrowserWindowGtk, gboolean, OnMainWindowDeleteEvent,
                       GdkEvent*);
  CHROMEGTK_CALLBACK_0(BrowserWindowGtk, void, OnMainWindowDestroy);
  
  
  CHROMEGTK_CALLBACK_1(BrowserWindowGtk, gboolean, OnCustomFrameExpose,
                       GdkEventExpose*);

  
  
  void DrawContentShadow(cairo_t* cr);

  
  void DrawPopupFrame(cairo_t* cr, GtkWidget* widget, GdkEventExpose* event);

  
  void DrawCustomFrame(cairo_t* cr, GtkWidget* widget, GdkEventExpose* event);

  
  
  
  int GetVerticalOffset();

  
  
  int GetThemeFrameResource();

  
  
  void InvalidateInfoBarBits();

  
  
  gfx::Size GetInfobarArrowSize();

  
  CHROMEGTK_CALLBACK_1(BrowserWindowGtk, void, OnLocationIconSizeAllocate,
                       GtkAllocation*);

  
  
  CHROMEGTK_CALLBACK_1(BrowserWindowGtk, gboolean, OnExposeDrawInfobarBits,
                       GdkEventExpose*);

  
  
  
  CHROMEGTK_CALLBACK_1(BrowserWindowGtk, gboolean, OnBookmarkBarExpose,
                       GdkEventExpose*);

  
  
  
  CHROMEGTK_CALLBACK_1(BrowserWindowGtk, void, OnBookmarkBarSizeAllocate,
                       GtkAllocation*);

  
  
  static gboolean OnGtkAccelerator(GtkAccelGroup* accel_group,
                                   GObject* acceleratable,
                                   guint keyval,
                                   GdkModifierType modifier,
                                   void* user_data);

  
  CHROMEGTK_CALLBACK_1(BrowserWindowGtk, gboolean, OnKeyPress, GdkEventKey*);

  
  CHROMEGTK_CALLBACK_1(BrowserWindowGtk, gboolean, OnMouseMoveEvent,
                       GdkEventMotion*);
  CHROMEGTK_CALLBACK_1(BrowserWindowGtk, gboolean, OnButtonPressEvent,
                       GdkEventButton*);

  
  static void MainWindowMapped(GtkWidget* widget);
  static void MainWindowUnMapped(GtkWidget* widget);

  
  CHROMEGTK_CALLBACK_1(BrowserWindowGtk, gboolean, OnFocusIn,
                       GdkEventFocus*);
  CHROMEGTK_CALLBACK_1(BrowserWindowGtk, gboolean, OnFocusOut,
                       GdkEventFocus*);

  
  void LoadingAnimationCallback();

  
  void ShowSupportedWindowFeatures();

  
  void HideUnsupportedWindowFeatures();

  
  
  bool IsTabStripSupported() const;
  bool IsToolbarSupported() const;
  bool IsBookmarkBarSupported() const;

  
  
  bool UsingCustomPopupFrame() const;

  
  
  
  bool GetWindowEdge(int x, int y, GdkWindowEdge* edge);

  
  bool UseCustomFrame();

  
  bool BoundsMatchMonitorSize();

  
  void PlaceBookmarkBar(bool is_floating);

  
  
  static bool GetCustomFramePrefDefault();

  NotificationRegistrar registrar_;

  
  gfx::Rect bounds_;

  
  gfx::Rect restored_bounds_;

  GdkWindowState state_;

  
  
  scoped_ptr<GlobalMenuBar> global_menu_bar_;

  
  scoped_ptr<BrowserTitlebar> titlebar_;

  
  scoped_ptr<BrowserToolbarGtk> toolbar_;

  
  
  scoped_ptr<BookmarkBarGtk> bookmark_bar_;

  
  bool bookmark_bar_is_floating_;

  
  scoped_ptr<StatusBubbleGtk> status_bubble_;

  
  
  
  scoped_ptr<TabContentsContainerGtk> contents_container_;

  
  
  scoped_ptr<TabContentsContainerGtk> devtools_container_;

  
  GtkWidget* contents_split_;

  
  scoped_ptr<TabStripGtk> tabstrip_;

  
  scoped_ptr<InfoBarContainerGtk> infobar_container_;

  
  base::RepeatingTimer<BrowserWindowGtk> loading_animation_timer_;

  
  base::OneShotTimer<BrowserWindowGtk> window_configure_debounce_timer_;

  
  
  
  BooleanPrefMember use_custom_frame_pref_;

  
  static std::map<XID, GtkWindow*> xid_map_;

  
  
  GdkCursor* frame_cursor_;

  
  
  
  bool is_active_;

  
  
  guint32 last_click_time_;
  gfx::Point last_click_position_;

  
  
  bool maximize_after_show_;

  
  
  bool suppress_window_raise_;

  
  GtkAccelGroup* accel_group_;

  scoped_ptr<FullscreenExitBubbleGtk> fullscreen_exit_bubble_;

  
  
  
  
  
  bool debounce_timer_disabled_;

  
  
  InfoBarArrowModel infobar_arrow_model_;

  DISALLOW_COPY_AND_ASSIGN(BrowserWindowGtk);
};

#endif  
