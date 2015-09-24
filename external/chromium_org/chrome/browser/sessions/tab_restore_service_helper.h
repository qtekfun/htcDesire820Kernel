// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SESSIONS_TAB_RESTORE_SERVICE_HELPER_H_
#define CHROME_BROWSER_SESSIONS_TAB_RESTORE_SERVICE_HELPER_H_

#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "base/time/time.h"
#include "chrome/browser/sessions/session_id.h"
#include "chrome/browser/sessions/session_types.h"
#include "chrome/browser/sessions/tab_restore_service.h"
#include "chrome/browser/ui/host_desktop.h"

class Profile;
class TabRestoreService;
class TabRestoreServiceDelegate;
class TabRestoreServiceObserver;
class TimeFactory;

namespace content {
class NavigationController;
class WebContents;
}

class TabRestoreServiceHelper {
 public:
  typedef TabRestoreService::Entries Entries;
  typedef TabRestoreService::Entry Entry;
  typedef TabRestoreService::Tab Tab;
  typedef TabRestoreService::TimeFactory TimeFactory;
  typedef TabRestoreService::Window Window;

  
  
  class Observer {
   public:
    
    virtual void OnClearEntries();

    
    
    virtual void OnRestoreEntryById(SessionID::id_type id,
                                    Entries::const_iterator entry_iterator);

    
    virtual void OnAddEntry();

   protected:
    virtual ~Observer();
  };

  enum {
    
    kMaxEntries = 25,
  };

  
  
  
  TabRestoreServiceHelper(TabRestoreService* tab_restore_service,
                          Observer* observer,
                          Profile* profile,
                          TimeFactory* time_factory);

  ~TabRestoreServiceHelper();

  
  void AddObserver(TabRestoreServiceObserver* observer);
  void RemoveObserver(TabRestoreServiceObserver* observer);
  void CreateHistoricalTab(content::WebContents* contents, int index);
  void BrowserClosing(TabRestoreServiceDelegate* delegate);
  void BrowserClosed(TabRestoreServiceDelegate* delegate);
  void ClearEntries();
  const Entries& entries() const;
  std::vector<content::WebContents*> RestoreMostRecentEntry(
      TabRestoreServiceDelegate* delegate,
      chrome::HostDesktopType host_desktop_type);
  Tab* RemoveTabEntryById(SessionID::id_type id);
  std::vector<content::WebContents*> RestoreEntryById(
      TabRestoreServiceDelegate* delegate,
      SessionID::id_type id,
      chrome::HostDesktopType host_desktop_type,
      WindowOpenDisposition disposition);

  
  void NotifyTabsChanged();

  
  void NotifyLoaded();

  
  
  
  
  void AddEntry(Entry* entry, bool prune, bool to_front);

  
  
  void PruneEntries();

  
  
  
  
  Entries::iterator GetEntryIteratorById(SessionID::id_type id);

  
  static bool ValidateEntry(Entry* entry);

 private:
  friend class PersistentTabRestoreService;

  
  
  void PopulateTab(Tab* tab,
                   int index,
                   TabRestoreServiceDelegate* delegate,
                   content::NavigationController* controller);

  
  
  
  
  
  
  
  
  TabRestoreServiceDelegate* RestoreTab(
      const Tab& tab,
      TabRestoreServiceDelegate* delegate,
      chrome::HostDesktopType host_desktop_type,
      WindowOpenDisposition disposition,
      content::WebContents** contents);

  
  
  
  static bool ValidateTab(Tab* tab);

  
  static bool ValidateWindow(Window* window);

  
  static bool IsTabInteresting(const Tab* tab);

  
  
  static bool IsWindowInteresting(const Window* window);

  
  static bool FilterEntry(Entry* entry);

  
  void UpdateTabBrowserIDs(SessionID::id_type old_id,
                           SessionID::id_type new_id);

  
  base::Time TimeNow() const;

  TabRestoreService* const tab_restore_service_;

  Observer* const observer_;

  Profile* const profile_;

  
  Entries entries_;

  
  
  bool restoring_;

  ObserverList<TabRestoreServiceObserver> observer_list_;

  
  
  
  std::set<TabRestoreServiceDelegate*> closing_delegates_;

  TimeFactory* const time_factory_;

  DISALLOW_COPY_AND_ASSIGN(TabRestoreServiceHelper);
};

#endif  
