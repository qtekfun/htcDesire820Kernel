// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BROWSER_WINDOW_H_
#define CHROME_BROWSER_UI_BROWSER_WINDOW_H_
#pragma once

#include "chrome/common/content_settings_types.h"
#include "content/browser/tab_contents/navigation_entry.h"
#include "ui/gfx/native_widget_types.h"

class Browser;
class BrowserWindowTesting;
class DownloadShelf;
class FindBar;
class GURL;
class HtmlDialogUIDelegate;
class LocationBar;
class Panel;
class Profile;
class StatusBubble;
class TabContents;
class TabContentsWrapper;
class TemplateURL;
class TemplateURLModel;
#if !defined(OS_MACOSX)
class ToolbarView;
#endif
struct NativeWebKeyboardEvent;

namespace gfx {
class Rect;
}

namespace views {
class Window;
}

class Extension;

class BrowserWindow {
 public:
  virtual ~BrowserWindow() {}

  
  virtual void Show() = 0;

  
  
  virtual void ShowInactive() = 0;

  
  virtual void SetBounds(const gfx::Rect& bounds) = 0;

  
  
  
  
  
  
  
  virtual void Close() = 0;

  
  
  virtual void Activate() = 0;

  
  
  virtual void Deactivate() = 0;

  
  virtual bool IsActive() const = 0;

  
  virtual void FlashFrame() = 0;

  
  
  virtual gfx::NativeWindow GetNativeHandle() = 0;

  
  
  virtual BrowserWindowTesting* GetBrowserWindowTesting() = 0;

  
  virtual StatusBubble* GetStatusBubble() = 0;

  
  
  
  
  virtual void ToolbarSizeChanged(bool is_animating) = 0;

  
  
  virtual void UpdateTitleBar() = 0;

  
  
  
  
  
  
  virtual void ShelfVisibilityChanged() = 0;

  
  
  virtual void UpdateDevTools() = 0;

  
  
  
  virtual void UpdateLoadingAnimations(bool should_animate) = 0;

  
  virtual void SetStarredState(bool is_starred) = 0;

  
  
  virtual gfx::Rect GetRestoredBounds() const = 0;

  
  
  
  virtual gfx::Rect GetBounds() const = 0;

  
  
  virtual bool IsMaximized() const = 0;

  
  virtual void SetFullscreen(bool fullscreen) = 0;
  virtual bool IsFullscreen() const = 0;

  
  virtual bool IsFullscreenBubbleVisible() const = 0;

  
  virtual LocationBar* GetLocationBar() const = 0;

  
  
  virtual void SetFocusToLocationBar(bool select_all) = 0;

  
  
  virtual void UpdateReloadStopState(bool is_loading, bool force) = 0;

  
  virtual void UpdateToolbar(TabContentsWrapper* contents,
                             bool should_restore_state) = 0;

  
  virtual void FocusToolbar() = 0;

  
  
  
  virtual void FocusAppMenu() = 0;

  
  virtual void FocusBookmarksToolbar() = 0;

  
  virtual void FocusChromeOSStatus() = 0;

  
  virtual void RotatePaneFocus(bool forwards) = 0;

  
  virtual bool IsBookmarkBarVisible() const = 0;

  
  virtual bool IsBookmarkBarAnimating() const = 0;

  
  virtual bool IsTabStripEditable() const = 0;

  
  virtual bool IsToolbarVisible() const = 0;

  
  
  
  
  virtual void DisableInactiveFrame() {}

  
  
  virtual void ConfirmSetDefaultSearchProvider(
      TabContents* tab_contents,
      TemplateURL* template_url,
      TemplateURLModel* template_url_model) {
    
    
  }

  
  
  virtual void ConfirmAddSearchProvider(const TemplateURL* template_url,
                                        Profile* profile) = 0;

  
  virtual void ToggleBookmarkBar() = 0;

  
  virtual void ShowAboutChromeDialog() = 0;

  
  virtual void ShowUpdateChromeDialog() = 0;

  
  virtual void ShowTaskManager() = 0;

  
  virtual void ShowBackgroundPages() = 0;

  
  
  virtual void ShowBookmarkBubble(const GURL& url, bool already_bookmarked) = 0;

  
  virtual bool IsDownloadShelfVisible() const = 0;

  
  virtual DownloadShelf* GetDownloadShelf() = 0;

  
  virtual void ShowRepostFormWarningDialog(TabContents* tab_contents) = 0;

  
  virtual void ShowCollectedCookiesDialog(TabContents* tab_contents) = 0;

  
  virtual void ShowThemeInstallBubble() = 0;

  
  
  
  
  virtual void ConfirmBrowserCloseWithPendingDownloads() = 0;

  
  
  virtual void ShowHTMLDialog(HtmlDialogUIDelegate* delegate,
                              gfx::NativeWindow parent_window) = 0;

  
  
  virtual void UserChangedTheme() = 0;

  
  
  
  
  virtual int GetExtraRenderViewHeight() const = 0;

  
  
  virtual void TabContentsFocused(TabContents* tab_contents) = 0;

  
  
  
  
  virtual void ShowPageInfo(Profile* profile,
                            const GURL& url,
                            const NavigationEntry::SSLStatus& ssl,
                            bool show_history) = 0;

  
  virtual void ShowAppMenu() = 0;

  
  
  
  
  
  virtual bool PreHandleKeyboardEvent(const NativeWebKeyboardEvent& event,
                                      bool* is_keyboard_shortcut) = 0;

  
  
  virtual void HandleKeyboardEvent(const NativeWebKeyboardEvent& event) = 0;

  
  virtual void ShowCreateWebAppShortcutsDialog(
      TabContentsWrapper* tab_contents) = 0;

  
  virtual void ShowCreateChromeAppShortcutsDialog(Profile* profile,
                                                  const Extension* app) = 0;

  
  virtual void Cut() = 0;
  virtual void Copy() = 0;
  virtual void Paste() = 0;

  
  virtual void ToggleTabStripMode() = 0;

#if defined(OS_MACOSX)
  
  virtual void OpenTabpose() = 0;
#endif

  
  virtual void PrepareForInstant() = 0;

  
  virtual void ShowInstant(TabContentsWrapper* preview) = 0;

  
  
  virtual void HideInstant(bool instant_is_active) = 0;

  
  
  
  virtual gfx::Rect GetInstantBounds() = 0;

#if defined(OS_CHROMEOS)
  
  virtual void ShowKeyboardOverlay(gfx::NativeWindow owning_window) = 0;
#endif

  
  static BrowserWindow* CreateBrowserWindow(Browser* browser);

  
  static FindBar* CreateFindBar(Browser* browser_window);

 protected:
  friend class BrowserList;
  friend class BrowserView;
  virtual void DestroyBrowser() = 0;
};

#if defined(OS_WIN) || defined(TOOLKIT_VIEWS)
class BookmarkBarView;
class LocationBarView;

namespace views {
class View;
}
#endif  

class BrowserWindowTesting {
 public:
#if defined(OS_WIN) || defined(TOOLKIT_VIEWS)
  
  virtual BookmarkBarView* GetBookmarkBarView() const = 0;

  
  virtual LocationBarView* GetLocationBarView() const = 0;

  
  virtual views::View* GetTabContentsContainerView() const = 0;

  
  virtual views::View* GetSidebarContainerView() const = 0;

  
  virtual ToolbarView* GetToolbarView() const = 0;
#endif

 protected:
  virtual ~BrowserWindowTesting() {}
};

#endif  
