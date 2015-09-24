// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_BROWSER_WINDOW_GTK_H_
#define CHROME_BROWSER_UI_GTK_BROWSER_WINDOW_GTK_H_

#include <gtk/gtk.h>

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/prefs/pref_member.h"
#include "base/timer/timer.h"
#include "build/build_config.h"
#include "chrome/browser/devtools/devtools_window.h"
#include "chrome/browser/extensions/extension_keybinding_registry.h"
#include "chrome/browser/infobars/infobar_container.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/host_desktop.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/ui_base_types.h"
#include "ui/base/x/active_window_watcher_x_observer.h"
#include "ui/base/x/x11_util.h"
#include "ui/gfx/rect.h"

class BookmarkBarGtk;
class Browser;
class BrowserTitlebar;
class BrowserToolbarGtk;
class DevToolsWindow;
class DownloadShelfGtk;
class ExtensionKeybindingRegistryGtk;
class FindBarGtk;
class FullscreenExitBubbleGtk;
class GlobalMenuBar;
class InfoBarContainerGtk;
class InstantOverlayControllerGtk;
class LocationBar;
class StatusBubbleGtk;
class TabContentsContainerGtk;
class TabStripGtk;

namespace autofill {
class PasswordGenerator;
}

namespace extensions {
class ActiveTabPermissionGranter;
class Extension;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

class BrowserWindowGtk
    : public BrowserWindow,
      public content::NotificationObserver,
      public TabStripModelObserver,
      public ui::ActiveWindowWatcherXObserver,
      public InfoBarContainer::Delegate,
      public extensions::ExtensionKeybindingRegistry::Delegate {
 public:
  explicit BrowserWindowGtk(Browser* browser);
  virtual ~BrowserWindowGtk();

  
  void Init();

  
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
      const GURL& url, FullscreenExitBubbleType type) OVERRIDE;
  virtual void ExitFullscreen() OVERRIDE;
  virtual void UpdateFullscreenExitBubbleContent(
      const GURL& url,
      FullscreenExitBubbleType bubble_type) OVERRIDE;
  virtual bool ShouldHideUIForFullscreen() const OVERRIDE;
  virtual bool IsFullscreen() const OVERRIDE;
  virtual bool IsFullscreenBubbleVisible() const OVERRIDE;
  virtual LocationBar* GetLocationBar() const OVERRIDE;
  virtual void SetFocusToLocationBar(bool select_all) OVERRIDE;
  virtual void UpdateReloadStopState(bool is_loading, bool force) OVERRIDE;
  virtual void UpdateToolbar(content::WebContents* contents) OVERRIDE;
  virtual void FocusToolbar() OVERRIDE;
  virtual void FocusAppMenu() OVERRIDE;
  virtual void FocusBookmarksToolbar() OVERRIDE;
  virtual void FocusInfobars() OVERRIDE;
  virtual void RotatePaneFocus(bool forwards) OVERRIDE;
  virtual bool IsBookmarkBarVisible() const OVERRIDE;
  virtual bool IsBookmarkBarAnimating() const OVERRIDE;
  virtual bool IsTabStripEditable() const OVERRIDE;
  virtual bool IsToolbarVisible() const OVERRIDE;
  virtual gfx::Rect GetRootWindowResizerRect() const OVERRIDE;
  virtual void ConfirmAddSearchProvider(TemplateURL* template_url,
                                        Profile* profile) OVERRIDE;
  virtual void ShowUpdateChromeDialog() OVERRIDE;
  virtual void ShowBookmarkBubble(const GURL& url,
                                  bool already_bookmarked) OVERRIDE;
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
  virtual int GetRenderViewHeightInsetWithDetachedBookmarkBar() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void TabDetachedAt(content::WebContents* contents,
                             int index) OVERRIDE;

  
  virtual void ActiveWindowChanged(GdkWindow* active_window) OVERRIDE;

  
  virtual SkColor GetInfoBarSeparatorColor() const OVERRIDE;
  virtual void InfoBarContainerStateChanged(bool is_animating) OVERRIDE;
  virtual bool DrawInfoBarArrows(int* x) const OVERRIDE;

  
  virtual extensions::ActiveTabPermissionGranter*
      GetActiveTabPermissionGranter() OVERRIDE;

  
  TabStripGtk* tabstrip() const { return tabstrip_.get(); }

  void OnDebouncedBoundsChanged();

  
  void UnMaximize();

  
  
  bool CanClose() const;

  
  
  
  bool ShouldDrawContentDropShadow() const;

  bool ShouldShowWindowIcon() const;

  
  void AddFindBar(FindBarGtk* findbar);

  
  
  void ResetCustomFrameCursor();

  
  static BrowserWindowGtk* GetBrowserWindowForNativeWindow(
      gfx::NativeWindow window);

  
  
  static GtkWindow* GetBrowserWindowForXID(XID xid);

  Browser* browser() const { return browser_.get(); }

  GtkWindow* window() const { return window_; }

  BrowserTitlebar* titlebar() const { return titlebar_.get(); }

  GtkWidget* titlebar_widget() const;

  BrowserToolbarGtk* GetToolbar() { return toolbar_.get(); }

  gfx::Rect bounds() const { return bounds_; }

  
  content::WebContents* GetDisplayedTab();

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  
  
  void QueueToolbarRedraw();

  
  
  int GetXPositionOfLocationIcon(GtkWidget* relative_to);

  
  void MaybeShowBookmarkBar(bool animate);

 protected:
  virtual void DestroyBrowser() OVERRIDE;

  
  
  
  bool GetWindowEdge(int x, int y, GdkWindowEdge* edge);

  
  
  
  GdkRegion* GetWindowShape(int width, int height) const;

  
  void SaveWindowPosition();

  
  
  void SetGeometryHints();

  
  bool UseCustomFrame() const;

  
  void InvalidateWindow();

  
  GtkWindow* window_;
  
  bool window_has_shown_;
  
  
  
  GtkWidget* window_container_;
  
  
  GtkWidget* window_vbox_;
  
  
  GtkWidget* render_area_vbox_;
  
  
  GtkWidget* render_area_floating_container_;
  
  GtkWidget* render_area_event_box_;
  
  GtkWidget* toolbar_border_;

  scoped_ptr<Browser> browser_;

 private:
  
  void ConnectHandlersToSignals();

  
  void InitWidgets();

  
  
  void SetBackgroundColor();

  
  void UpdateWindowShape(int width, int height);

  
  
  void ConnectAccelerators();

  
  
  bool UsingCustomPopupFrame() const;

  
  void DrawCustomFrame(cairo_t* cr, GtkWidget* widget, GdkEventExpose* event);

  
  void DrawPopupFrame(cairo_t* cr, GtkWidget* widget, GdkEventExpose* event);

  
  void DrawCustomFrameBorder(GtkWidget* widget);

  
  
  
  void UpdateCustomFrame();

  
  
  
  
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

  
  
  
  int GetVerticalOffset();

  
  
  int GetThemeFrameResource();

  
  
  void InvalidateInfoBarBits();

  
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

  
  void PlaceBookmarkBar(bool is_floating);

  
  bool DrawFrameAsActive() const;

  
  
  
  
  void UpdateDevToolsForContents(content::WebContents* contents);

  
  void ShowDevToolsContainer();

  
  void HideDevToolsContainer();

  
  
  void UpdateDevToolsSplitPosition();

  
  void OnUseCustomChromeFrameChanged();

  
  
  static bool GetCustomFramePrefDefault();

  
  gfx::Rect bounds_;

  
  
  gfx::Rect configure_bounds_;

  
  gfx::Rect restored_bounds_;

  GdkWindowState state_;

  
  
  scoped_ptr<GlobalMenuBar> global_menu_bar_;

  
  scoped_ptr<BrowserTitlebar> titlebar_;

  
  scoped_ptr<BrowserToolbarGtk> toolbar_;

  
  
  scoped_ptr<BookmarkBarGtk> bookmark_bar_;

  
  scoped_ptr<DownloadShelfGtk> download_shelf_;

  
  scoped_ptr<StatusBubbleGtk> status_bubble_;

  
  
  
  scoped_ptr<TabContentsContainerGtk> contents_container_;

  
  
  scoped_ptr<TabContentsContainerGtk> devtools_container_;

  
  
  
  scoped_ptr<ExtensionKeybindingRegistryGtk> extension_keybinding_registry_;

  DevToolsDockSide devtools_dock_side_;

  
  
  DevToolsWindow* devtools_window_;

  
  
  GtkWidget* contents_hsplit_;

  
  
  GtkWidget* contents_vsplit_;

  
  scoped_ptr<TabStripGtk> tabstrip_;

  
  scoped_ptr<InfoBarContainerGtk> infobar_container_;

  
  base::RepeatingTimer<BrowserWindowGtk> loading_animation_timer_;

  
  base::OneShotTimer<BrowserWindowGtk> window_configure_debounce_timer_;

  
  
  
  BooleanPrefMember use_custom_frame_pref_;

  
  
  GdkCursor* frame_cursor_;

  
  
  
  bool is_active_;

  
  
  
  ui::WindowShowState show_state_after_show_;

  
  
  bool suppress_window_raise_;

  
  GtkAccelGroup* accel_group_;

  
  
  
  
  bool is_fullscreen_;

  scoped_ptr<FullscreenExitBubbleGtk> fullscreen_exit_bubble_;

  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(BrowserWindowGtk);
};

#endif  
