// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_ASH_MULTI_USER_MULTI_USER_WINDOW_MANAGER_CHROMEOS_H_
#define CHROME_BROWSER_UI_ASH_MULTI_USER_MULTI_USER_WINDOW_MANAGER_CHROMEOS_H_

#include <map>
#include <string>

#include "ash/session_state_observer.h"
#include "ash/wm/window_state_observer.h"
#include "base/compiler_specific.h"
#include "chrome/browser/ui/ash/multi_user/multi_user_window_manager.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/aura/window_observer.h"

class Browser;
class Profile;

namespace aura {
class Window;
class WindowObserver;
}

namespace chrome {

class AppObserver;

// overwritten to match new ways of doing this.
class MultiUserWindowManagerChromeOS : public MultiUserWindowManager,
                                       public ash::SessionStateObserver,
                                       public aura::WindowObserver,
                                       public content::NotificationObserver,
                                       public ash::wm::WindowStateObserver {
 public:
  
  explicit MultiUserWindowManagerChromeOS(const std::string& active_user_id);
  virtual ~MultiUserWindowManagerChromeOS();

  
  virtual void SetWindowOwner(
      aura::Window* window, const std::string& user_id) OVERRIDE;
  virtual const std::string& GetWindowOwner(aura::Window* window) OVERRIDE;
  virtual void ShowWindowForUser(
      aura::Window* window, const std::string& user_id) OVERRIDE;
  virtual bool AreWindowsSharedAmongUsers() OVERRIDE;
  virtual bool IsWindowOnDesktopOfUser(aura::Window* window,
                                       const std::string& user_id) OVERRIDE;
  virtual const std::string& GetUserPresentingWindow(
      aura::Window* window) OVERRIDE;
  virtual void AddUser(Profile* profile) OVERRIDE;

  
  virtual void ActiveUserChanged(const std::string& user_id) OVERRIDE;

  
  virtual void OnWindowDestroyed(aura::Window* window) OVERRIDE;
  virtual void OnWindowVisibilityChanging(aura::Window* window,
                                          bool visible) OVERRIDE;
  virtual void OnWindowVisibilityChanged(aura::Window* window,
                                         bool visible) OVERRIDE;
  virtual void OnAddTransientChild(aura::Window* window,
                                   aura::Window* transient) OVERRIDE;
  virtual void OnRemoveTransientChild(aura::Window* window,
                                      aura::Window* transient) OVERRIDE;

  
  virtual void OnWindowShowTypeChanged(
      ash::wm::WindowState* state,
      ash::wm::WindowShowType old_type) OVERRIDE;

  
  virtual void Observe(int type,
               const content::NotificationSource& source,
               const content::NotificationDetails& details) OVERRIDE;

 private:
  class WindowEntry {
   public:
    explicit WindowEntry(const std::string& user_id)
        : owner_(user_id),
          show_for_user_(user_id),
          show_(true) {}
    virtual ~WindowEntry() {}

    
    const std::string& owner() const { return owner_; }

    
    const std::string& show_for_user() const { return show_for_user_; }

    
    bool show() const { return show_; }

    
    
    void set_show_for_user(const std::string& user_id) {
      show_for_user_ = user_id.empty() ? owner_ : user_id;
    }

    
    void set_show(bool show) { show_ = show; }

   private:
    
    const std::string owner_;

    
    std::string show_for_user_;

    
    bool show_;

    DISALLOW_COPY_AND_ASSIGN(WindowEntry);
  };

  typedef std::map<aura::Window*, WindowEntry*> WindowToEntryMap;
  typedef std::map<std::string, AppObserver*> UserIDToShellWindowObserver;
  typedef std::map<aura::Window*, bool> TransientWindowToVisibility;

  
  void AddBrowserWindow(Browser* browser);

  
  
  
  
  
  void SetWindowVisibility(aura::Window* window, bool visible);

  
  
  void ShowWithTransientChildrenRecursive(aura::Window* window);

  
  
  aura::Window* GetOwningWindowInTransientChain(aura::Window* window);

  
  
  
  void AddTransientOwnerRecursive(aura::Window* window,
                                  aura::Window* owning_parent);

  
  
  void RemoveTransientOwnerRecursive(aura::Window* window);

  
  
  WindowToEntryMap window_to_entry_;

  
  UserIDToShellWindowObserver user_id_to_app_observer_;

  
  TransientWindowToVisibility transient_window_to_visibility_;

  
  
  
  std::string current_user_id_;

  
  content::NotificationRegistrar registrar_;

  
  bool suppress_visibility_changes_;

  
  
  static MultiProfileMode multi_user_mode_;

  DISALLOW_COPY_AND_ASSIGN(MultiUserWindowManagerChromeOS);
};

}  

#endif  
