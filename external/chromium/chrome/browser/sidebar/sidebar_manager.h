// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SIDEBAR_SIDEBAR_MANAGER_H_
#define CHROME_BROWSER_SIDEBAR_SIDEBAR_MANAGER_H_

#include <map>
#include <string>

#include "base/memory/ref_counted.h"
#include "base/string16.h"
#include "chrome/browser/sidebar/sidebar_container.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class GURL;
class PrefService;
class Profile;
class SidebarContainer;
class SkBitmap;
class TabContents;

class SidebarManager : public NotificationObserver,
                       public base::RefCounted<SidebarManager>,
                       private SidebarContainer::Delegate {
 public:
  
  static SidebarManager* GetInstance();

  
  static bool IsSidebarAllowed();

  SidebarManager();

  
  
  SidebarContainer* GetActiveSidebarContainerFor(TabContents* tab);

  
  
  SidebarContainer* GetSidebarContainerFor(TabContents* tab,
                                           const std::string& content_id);

  
  TabContents* GetSidebarTabContents(TabContents* tab,
                                     const std::string& content_id);

  
  void NotifyStateChanges(TabContents* was_active_sidebar_contents,
                          TabContents* active_sidebar_contents);

  

  
  
  void ShowSidebar(TabContents* tab, const std::string& content_id);

  
  void ExpandSidebar(TabContents* tab, const std::string& content_id);

  
  
  void CollapseSidebar(TabContents* tab, const std::string& content_id);

  
  
  void HideSidebar(TabContents* tab, const std::string& content_id);

  
  void NavigateSidebar(TabContents* tab,
                       const std::string& content_id,
                       const GURL& url);

  
  void SetSidebarBadgeText(TabContents* tab,
                           const std::string& content_id,
                           const string16& badge_text);

  
  void SetSidebarIcon(TabContents* tab,
                      const std::string& content_id,
                      const SkBitmap& bitmap);

  
  void SetSidebarTitle(TabContents* tab,
                       const std::string& content_id,
                       const string16& title);

 private:
  friend class base::RefCounted<SidebarManager>;

  virtual ~SidebarManager();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual void UpdateSidebar(SidebarContainer* host);

  
  void HideAllSidebars(TabContents* tab);

  
  SidebarContainer* FindSidebarContainerFor(TabContents* sidebar_contents);

  
  
  void RegisterSidebarContainerFor(TabContents* tab,
                                   SidebarContainer* container);

  
  void UnregisterSidebarContainerFor(TabContents* tab,
                                     const std::string& content_id);

  
  void BindSidebarHost(TabContents* tab, SidebarContainer* sidebar_host);

  
  void UnbindSidebarHost(TabContents* tab, SidebarContainer* sidebar_host);

  NotificationRegistrar registrar_;

  
  
  typedef std::map<std::string, SidebarContainer*> ContentIdToSidebarHostMap;

  
  
  
  
  
  struct SidebarStateForTab;
  typedef std::map<TabContents*, SidebarStateForTab> TabToSidebarHostMap;
  TabToSidebarHostMap tab_to_sidebar_host_;

  typedef std::map<SidebarContainer*, TabContents*> SidebarHostToTabMap;
  SidebarHostToTabMap sidebar_host_to_tab_;

  DISALLOW_COPY_AND_ASSIGN(SidebarManager);
};

#endif  

