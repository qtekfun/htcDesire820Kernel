// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_FULLSCREEN_FULLSCREEN_CONTROLLER_H_
#define CHROME_BROWSER_UI_FULLSCREEN_FULLSCREEN_CONTROLLER_H_

#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/ui/fullscreen/fullscreen_exit_bubble_type.h"
#include "chrome/common/content_settings.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class Browser;
class BrowserWindow;
class GURL;
class Profile;

namespace content {
class WebContents;
}


class FullscreenController : public content::NotificationObserver {
 public:
  explicit FullscreenController(Browser* browser);
  virtual ~FullscreenController();

  

  
  
  bool IsFullscreenForBrowser() const;

  void ToggleFullscreenMode();

  

  
  
  
  bool IsFullscreenForTabOrPending() const;
  bool IsFullscreenForTabOrPending(
      const content::WebContents* web_contents) const;
  
  
  bool IsFullscreenCausedByTab() const;

  void ToggleFullscreenModeForTab(content::WebContents* web_contents,
                                  bool enter_fullscreen);

  
  
  
  void ToggleFullscreenModeWithExtension(const GURL& extension_url);

  

  
  bool IsInMetroSnapMode();

#if defined(OS_WIN)
  
  
  void SetMetroSnapMode(bool enable);
#endif

#if defined(OS_MACOSX)
  void ToggleFullscreenWithChrome();
#endif

  

  bool IsMouseLockRequested() const;
  bool IsMouseLocked() const;

  void RequestToLockMouse(content::WebContents* web_contents,
                          bool user_gesture,
                          bool last_unlocked_by_target);

  

  
  void OnTabDeactivated(content::WebContents* web_contents);

  
  void OnTabClosing(content::WebContents* web_contents);

  
  void WindowFullscreenStateChanged();

  
  bool HandleUserPressedEscape();

  
  void ExitTabOrBrowserFullscreenToPreviousState();
  void OnAcceptFullscreenPermission();
  void OnDenyFullscreenPermission();

  
  void LostMouseLock();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  

  GURL GetFullscreenExitBubbleURL() const;
  FullscreenExitBubbleType GetFullscreenExitBubbleType() const;

 private:
  friend class FullscreenControllerTest;

  enum MouseLockState {
    MOUSELOCK_NOT_REQUESTED,
    
    
    MOUSELOCK_REQUESTED,
    
    MOUSELOCK_ACCEPTED,
    
    MOUSELOCK_ACCEPTED_SILENTLY
  };

  enum FullscreenInternalOption {
    BROWSER,
#if defined(OS_MACOSX)
    BROWSER_WITH_CHROME,
#endif
    TAB
  };

  void UpdateNotificationRegistrations();

  
  void PostFullscreenChangeNotification(bool is_fullscreen);
  
  void NotifyFullscreenChange(bool is_fullscreen);
  
  
  void NotifyTabOfExitIfNecessary();
  void NotifyMouseLockChange();

  void ToggleFullscreenModeInternal(FullscreenInternalOption option);
  void EnterFullscreenModeInternal(FullscreenInternalOption option);
  void ExitFullscreenModeInternal();
  void SetFullscreenedTab(content::WebContents* tab);
  void SetMouseLockTab(content::WebContents* tab);

  
  void ExitTabFullscreenOrMouseLockIfNecessary();
  void UpdateFullscreenExitBubbleContent();

  ContentSetting GetFullscreenSetting(const GURL& url) const;
  ContentSetting GetMouseLockSetting(const GURL& url) const;

  bool IsPrivilegedFullscreenForTab() const;
  void SetPrivilegedFullscreenForTesting(bool is_privileged);
  void UnlockMouse();

  Browser* const browser_;
  BrowserWindow* const window_;
  Profile* const profile_;

  
  
  
  content::WebContents* fullscreened_tab_;

  
  GURL extension_caused_fullscreen_;

  enum PriorFullscreenState {
    STATE_INVALID,
    STATE_NORMAL,
    STATE_BROWSER_FULLSCREEN_NO_CHROME,
#if defined(OS_MACOSX)
    STATE_BROWSER_FULLSCREEN_WITH_CHROME,
#endif
  };
  
  
  PriorFullscreenState state_prior_to_tab_fullscreen_;
  
  
  bool tab_fullscreen_accepted_;

  
  bool toggled_into_fullscreen_;

  
  
  content::WebContents* mouse_lock_tab_;

  MouseLockState mouse_lock_state_;

  content::NotificationRegistrar registrar_;

  
  
  bool reentrant_window_state_change_call_check_;

  
  
  bool is_privileged_fullscreen_for_testing_;

  base::WeakPtrFactory<FullscreenController> ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(FullscreenController);
};

#endif  
