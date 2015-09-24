// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BROWSER_WINDOW_H_
#define CHROME_BROWSER_UI_BROWSER_WINDOW_H_

#include "base/callback_forward.h"
#include "chrome/browser/lifetime/browser_close_manager.h"
#include "chrome/browser/ui/bookmarks/bookmark_bar.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/fullscreen/fullscreen_exit_bubble_type.h"
#include "chrome/browser/ui/host_desktop.h"
#include "chrome/browser/ui/sync/one_click_signin_sync_starter.h"
#include "chrome/browser/ui/translate/translate_bubble_model.h"
#include "chrome/common/content_settings_types.h"
#include "chrome/common/translate/translate_errors.h"
#include "ui/base/base_window.h"
#include "ui/base/window_open_disposition.h"
#include "ui/gfx/native_widget_types.h"

class Browser;
class BrowserWindowTesting;
class DownloadShelf;
class FindBar;
class GURL;
class LocationBar;
class Profile;
class StatusBubble;
class TemplateURL;
#if !defined(OS_MACOSX)
class ToolbarView;
#endif

namespace autofill {
class PasswordGenerator;
struct PasswordForm;
}
namespace content {
class WebContents;
struct NativeWebKeyboardEvent;
struct SSLStatus;
}

namespace extensions {
class Extension;
}

namespace gfx {
class Rect;
class Size;
}

namespace web_modal {
class WebContentsModalDialogHost;
}

class BrowserWindow : public ui::BaseWindow {
 public:
  virtual ~BrowserWindow() {}

  
  

  
  
  
  
  
  
  
  

  
  

  
  

  
  
  virtual BrowserWindowTesting* GetBrowserWindowTesting() = 0;

  
  virtual StatusBubble* GetStatusBubble() = 0;

  
  
  virtual void UpdateTitleBar() = 0;

  
  
  virtual void BookmarkBarStateChanged(
      BookmarkBar::AnimateChangeType change_type) = 0;

  
  
  virtual void UpdateDevTools() = 0;

  
  
  
  virtual void UpdateLoadingAnimations(bool should_animate) = 0;

  
  virtual void SetStarredState(bool is_starred) = 0;

  
  virtual void SetTranslateIconToggled(bool is_lit) = 0;

  
  
  
  virtual void OnActiveTabChanged(content::WebContents* old_contents,
                                  content::WebContents* new_contents,
                                  int index,
                                  int reason) = 0;

  
  
  
  
  virtual void ZoomChangedForActiveTab(bool can_show_bubble) = 0;

  
  virtual void EnterFullscreen(const GURL& url,
                               FullscreenExitBubbleType bubble_type) = 0;
  virtual void ExitFullscreen() = 0;
  virtual void UpdateFullscreenExitBubbleContent(
      const GURL& url,
      FullscreenExitBubbleType bubble_type) = 0;

  
  
  virtual bool ShouldHideUIForFullscreen() const = 0;

  
  virtual bool IsFullscreenBubbleVisible() const = 0;

#if defined(OS_WIN)
  
  virtual void SetMetroSnapMode(bool enable) = 0;

  
  virtual bool IsInMetroSnapMode() const = 0;
#endif

  
  virtual LocationBar* GetLocationBar() const = 0;

  
  
  virtual void SetFocusToLocationBar(bool select_all) = 0;

  
  
  virtual void UpdateReloadStopState(bool is_loading, bool force) = 0;

  
  virtual void UpdateToolbar(content::WebContents* contents) = 0;

  
  virtual void FocusToolbar() = 0;

  
  
  
  virtual void FocusAppMenu() = 0;

  
  virtual void FocusBookmarksToolbar() = 0;

  
  virtual void FocusInfobars() = 0;

  
  virtual void RotatePaneFocus(bool forwards) = 0;

  
  virtual bool IsBookmarkBarVisible() const = 0;

  
  virtual bool IsBookmarkBarAnimating() const = 0;

  
  virtual bool IsTabStripEditable() const = 0;

  
  virtual bool IsToolbarVisible() const = 0;

  
  
  
  
  virtual gfx::Rect GetRootWindowResizerRect() const = 0;

  
  
  
  
  virtual void DisableInactiveFrame() {}

  
  
  virtual void ConfirmAddSearchProvider(TemplateURL* template_url,
                                        Profile* profile) = 0;

  
  virtual void ShowUpdateChromeDialog() = 0;

  
  
  virtual void ShowBookmarkBubble(const GURL& url, bool already_bookmarked) = 0;

  
  
  virtual void ShowBookmarkPrompt() {}

  
  virtual void ShowTranslateBubble(
      content::WebContents* contents,
      TranslateBubbleModel::ViewState view_state,
      TranslateErrors::Type error_type) = 0;

#if defined(ENABLE_ONE_CLICK_SIGNIN)
  enum OneClickSigninBubbleType {
    ONE_CLICK_SIGNIN_BUBBLE_TYPE_BUBBLE,
    ONE_CLICK_SIGNIN_BUBBLE_TYPE_MODAL_DIALOG,
    ONE_CLICK_SIGNIN_BUBBLE_TYPE_SAML_MODAL_DIALOG
  };

  
  
  
  typedef base::Callback<void(OneClickSigninSyncStarter::StartSyncMode)>
      StartSyncCallback;

  
  
  virtual void ShowOneClickSigninBubble(
      OneClickSigninBubbleType type,
      const base::string16& email,
      const base::string16& error_message,
      const StartSyncCallback& start_sync_callback) = 0;
#endif

  
  virtual bool IsDownloadShelfVisible() const = 0;

  
  virtual DownloadShelf* GetDownloadShelf() = 0;

  
  
  
  virtual void ConfirmBrowserCloseWithPendingDownloads(
      int download_count,
      Browser::DownloadClosePreventionType dialog_type,
      bool app_modal,
      const base::Callback<void(bool)>& callback) = 0;

  
  
  virtual void UserChangedTheme() = 0;

  
  
  
  
  virtual int GetExtraRenderViewHeight() const = 0;

  
  
  virtual void WebContentsFocused(content::WebContents* contents) = 0;

  
  
  
  
  virtual void ShowWebsiteSettings(Profile* profile,
                                   content::WebContents* web_contents,
                                   const GURL& url,
                                   const content::SSLStatus& ssl) = 0;

  
  virtual void ShowAppMenu() = 0;

  
  
  
  
  
  virtual bool PreHandleKeyboardEvent(
      const content::NativeWebKeyboardEvent& event,
      bool* is_keyboard_shortcut) = 0;

  
  
  virtual void HandleKeyboardEvent(
      const content::NativeWebKeyboardEvent& event) = 0;

  
  virtual void Cut() = 0;
  virtual void Copy() = 0;
  virtual void Paste() = 0;

#if defined(OS_MACOSX)
  
  virtual void OpenTabpose() = 0;

  
  
  
  
  
  virtual void EnterFullscreenWithChrome() = 0;
  virtual bool IsFullscreenWithChrome() = 0;
  virtual bool IsFullscreenWithoutChrome() = 0;
#endif

  
  virtual WindowOpenDisposition GetDispositionForPopupBounds(
      const gfx::Rect& bounds) = 0;

  
  virtual FindBar* CreateFindBar() = 0;

  
  
  
  virtual web_modal::WebContentsModalDialogHost*
      GetWebContentsModalDialogHost() = 0;

  
  
  
  
  virtual void UpdatePreferredSize(content::WebContents* web_contents,
                                   const gfx::Size& pref_size) {}

  
  virtual void ResizeDueToAutoResize(content::WebContents* web_contents,
                                     const gfx::Size& new_size) {}

  
  static BrowserWindow* CreateBrowserWindow(Browser* browser);

  
  
  
  static chrome::HostDesktopType AdjustHostDesktopType(
      chrome::HostDesktopType desktop_type);

  
  
  
  virtual void ShowAvatarBubble(content::WebContents* web_contents,
                                const gfx::Rect& rect) = 0;

  
  virtual void ShowAvatarBubbleFromAvatarButton() = 0;

  
  
  
  
  virtual void ShowPasswordGenerationBubble(
      const gfx::Rect& rect,
      const autofill::PasswordForm& form,
      autofill::PasswordGenerator* password_generator) = 0;

  
  
  virtual void OverscrollUpdate(int delta_y) {}

  
  
  
  virtual int GetRenderViewHeightInsetWithDetachedBookmarkBar() = 0;

 protected:
  friend class BrowserCloseManager;
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

  
  virtual ToolbarView* GetToolbarView() const = 0;
#endif

 protected:
  virtual ~BrowserWindowTesting() {}
};

#endif  
