// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SESSIONS_TAB_RESTORE_SERVICE_DELEGATE_H_
#define CHROME_BROWSER_SESSIONS_TAB_RESTORE_SERVICE_DELEGATE_H_

#include <string>
#include <vector>

#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/sessions/session_id.h"
#include "chrome/browser/ui/host_desktop.h"

class Profile;

namespace content {
class NavigationController;
class SessionStorageNamespace;
class WebContents;
}

namespace sessions {
class SerializedNavigationEntry;
}

class TabRestoreServiceDelegate {
 public:
  
  virtual void ShowBrowserWindow() = 0;

  
  virtual const SessionID& GetSessionID() const = 0;

  
  virtual int GetTabCount() const = 0;

  
  virtual int GetSelectedIndex() const = 0;

  
  virtual std::string GetAppName() const = 0;

  
  virtual content::WebContents* GetWebContentsAt(int index) const = 0;
  virtual content::WebContents* GetActiveWebContents() const = 0;
  virtual bool IsTabPinned(int index) const = 0;
  virtual content::WebContents* AddRestoredTab(
      const std::vector<sessions::SerializedNavigationEntry>& navigations,
      int tab_index,
      int selected_navigation,
      const std::string& extension_app_id,
      bool select,
      bool pin,
      bool from_last_session,
      content::SessionStorageNamespace* storage_namespace,
      const std::string& user_agent_override) = 0;
  virtual content::WebContents* ReplaceRestoredTab(
      const std::vector<sessions::SerializedNavigationEntry>& navigations,
      int selected_navigation,
      bool from_last_session,
      const std::string& extension_app_id,
      content::SessionStorageNamespace* session_storage_namespace,
      const std::string& user_agent_override) = 0;
  virtual void CloseTab() = 0;

  
  static TabRestoreServiceDelegate* Create(
      Profile* profile,
      chrome::HostDesktopType host_desktop_type,
      const std::string& app_name);

  
  static TabRestoreServiceDelegate* FindDelegateForWebContents(
      const content::WebContents* contents);

  
  
  
  static TabRestoreServiceDelegate* FindDelegateWithID(
      SessionID::id_type desired_id,
      chrome::HostDesktopType host_desktop_type);

 protected:
  virtual ~TabRestoreServiceDelegate() {}
};

#endif  
