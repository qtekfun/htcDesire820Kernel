// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SESSIONS_TAB_RESTORE_SERVICE_H_
#define CHROME_BROWSER_SESSIONS_TAB_RESTORE_SERVICE_H_
#pragma once

#include <list>
#include <set>
#include <vector>

#include "base/observer_list.h"
#include "base/time.h"
#include "chrome/browser/sessions/base_session_service.h"
#include "chrome/browser/sessions/session_id.h"
#include "chrome/browser/sessions/session_types.h"
#include "content/browser/in_process_webkit/session_storage_namespace.h"

class NavigationController;
class Profile;
class TabRestoreServiceDelegate;
class TabRestoreServiceObserver;
struct SessionWindow;

class TabRestoreService : public BaseSessionService {
 public:
  
  class TimeFactory {
   public:
    virtual ~TimeFactory();
    virtual base::Time TimeNow() = 0;
  };

  
  enum Type {
    TAB,
    WINDOW
  };

  struct Entry {
    Entry();
    explicit Entry(Type type);
    virtual ~Entry();

    
    
    SessionID::id_type id;

    
    Type type;

    
    base::Time timestamp;

    
    
    
    bool from_last_session;
  };

  
  struct Tab : public Entry {
    Tab();
    virtual ~Tab();

    bool has_browser() const { return browser_id > 0; }

    
    std::vector<TabNavigation> navigations;

    
    int current_navigation_index;

    
    
    SessionID::id_type browser_id;

    
    int tabstrip_index;

    
    bool pinned;

    
    std::string extension_app_id;

    
    scoped_refptr<SessionStorageNamespace> session_storage_namespace;
  };

  
  struct Window : public Entry {
    Window();
    virtual ~Window();

    
    std::vector<Tab> tabs;

    
    int selected_tab_index;
  };

  typedef std::list<Entry*> Entries;

  
  
  
  explicit TabRestoreService(Profile* profile,
                             TimeFactory* time_factory_ = NULL);

  
  
  void AddObserver(TabRestoreServiceObserver* observer);
  void RemoveObserver(TabRestoreServiceObserver* observer);

  
  
  void CreateHistoricalTab(NavigationController* tab, int index);

  
  
  
  void BrowserClosing(TabRestoreServiceDelegate* delegate);

  
  void BrowserClosed(TabRestoreServiceDelegate* delegate);

  
  
  void ClearEntries();

  
  
  virtual const Entries& entries() const;

  
  
  
  void RestoreMostRecentEntry(TabRestoreServiceDelegate* delegate);

  
  
  
  
  void RestoreEntryById(TabRestoreServiceDelegate* delegate,
                        SessionID::id_type id,
                        bool replace_existing_tab);

  
  
  void LoadTabsFromLastSession();

  
  static const size_t kMaxEntries;

  
  void CreateEntriesFromWindows(std::vector<SessionWindow*>* windows,
                                std::vector<Entry*>* entries);

 protected:
  virtual void Save();

  virtual ~TabRestoreService();

 private:
  
  enum LoadState {
    
    NOT_LOADED           = 1 << 0,

    
    
    LOADING              = 1 << 2,

    
    
    LOADED_LAST_TABS     = 1 << 3,

    
    
    LOADED_LAST_SESSION  = 1 << 4
  };

  
  
  void PopulateTab(Tab* tab,
                   int index,
                   TabRestoreServiceDelegate* delegate,
                   NavigationController* controller);

  
  void NotifyTabsChanged();

  
  
  
  
  void AddEntry(Entry* entry, bool prune, bool to_front);

  
  void PruneAndNotify();

  
  
  
  
  Entries::iterator GetEntryIteratorById(SessionID::id_type id);

  
  void ScheduleCommandsForWindow(const Window& window);

  
  
  void ScheduleCommandsForTab(const Tab& tab, int selected_index);

  
  SessionCommand* CreateWindowCommand(SessionID::id_type id,
                                      int selected_tab_index,
                                      int num_tabs,
                                      base::Time timestamp);

  
  SessionCommand* CreateSelectedNavigationInTabCommand(
      SessionID::id_type tab_id,
      int32 index,
      base::Time timestamp);

  
  SessionCommand* CreateRestoredEntryCommand(SessionID::id_type entry_id);

  
  
  
  
  int GetSelectedNavigationIndexToPersist(const Tab& tab);

  
  
  
  void OnGotLastSessionCommands(
      Handle handle,
      scoped_refptr<InternalGetCommandsRequest> request);

  
  void CreateEntriesFromCommands(
      scoped_refptr<InternalGetCommandsRequest> request,
      std::vector<Entry*>* loaded_entries);

  
  
  
  
  
  TabRestoreServiceDelegate* RestoreTab(const Tab& tab,
                                        TabRestoreServiceDelegate* delegate,
                                        bool replace_existing_tab);

  
  
  
  bool ValidateTab(Tab* tab);

  
  
  
  void ValidateAndDeleteEmptyEntries(std::vector<Entry*>* entries);

  
  void UpdateTabBrowserIDs(SessionID::id_type old_id,
                           SessionID::id_type new_id);

  
  
  
  void OnGotPreviousSession(Handle handle,
                            std::vector<SessionWindow*>* windows);

  
  
  bool ConvertSessionWindowToWindow(
      SessionWindow* session_window,
      Window* window);

  
  
  
  void LoadStateChanged();

  
  base::Time TimeNow() const;

  
  Entries entries_;

  
  int load_state_;

  
  
  bool restoring_;

  
  bool reached_max_;

  
  int entries_to_write_;

  // Number of entries we've written.
  int entries_written_;

  ObserverList<TabRestoreServiceObserver> observer_list_;

  
  
  
  std::set<TabRestoreServiceDelegate*> closing_delegates_;

  
  CancelableRequestConsumer load_consumer_;

  
  
  
  
  std::vector<Entry*> staging_entries_;

  TimeFactory* time_factory_;

  DISALLOW_COPY_AND_ASSIGN(TabRestoreService);
};

#endif  
