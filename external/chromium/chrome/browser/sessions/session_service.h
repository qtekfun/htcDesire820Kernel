// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SESSIONS_SESSION_SERVICE_H_
#define CHROME_BROWSER_SESSIONS_SESSION_SERVICE_H_
#pragma once

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/time.h"
#include "chrome/browser/defaults.h"
#include "chrome/browser/sessions/base_session_service.h"
#include "chrome/browser/sessions/session_id.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_list.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class NavigationController;
class NavigationEntry;
class Profile;
class SessionCommand;
struct SessionTab;
struct SessionWindow;


// flushed to SessionBackend and written to a file. Every so often
class SessionService : public BaseSessionService,
                       public NotificationObserver {
  friend class SessionServiceTestHelper;
 public:
  
  explicit SessionService(Profile* profile);
  
  explicit SessionService(const FilePath& save_path);

  
  
  
  
  
  bool RestoreIfNecessary(const std::vector<GURL>& urls_to_open);

  
  
  void ResetFromCurrentBrowsers();

  
  
  
  void MoveCurrentSessionToLastSession();

  
  void SetTabWindow(const SessionID& window_id,
                    const SessionID& tab_id);

  
  void SetWindowBounds(const SessionID& window_id,
                       const gfx::Rect& bounds,
                       bool is_maximized);

  
  void SetTabIndexInWindow(const SessionID& window_id,
                           const SessionID& tab_id,
                           int new_index);

  
  void SetPinnedState(const SessionID& window_id,
                      const SessionID& tab_id,
                      bool is_pinned);

  
  
  
  
  
  void TabClosed(const SessionID& window_id,
                 const SessionID& tab_id,
                 bool closed_by_user_gesture);

  
  void WindowClosing(const SessionID& window_id);

  
  void WindowClosed(const SessionID& window_id);

  
  
  
  void SetWindowType(const SessionID& window_id, Browser::Type type);

  
  
  void TabNavigationPathPrunedFromBack(const SessionID& window_id,
                                       const SessionID& tab_id,
                                       int count);

  
  
  void TabNavigationPathPrunedFromFront(const SessionID& window_id,
                                        const SessionID& tab_id,
                                        int count);

  
  void UpdateTabNavigation(const SessionID& window_id,
                           const SessionID& tab_id,
                           int index,
                           const NavigationEntry& entry);

  
  
  void TabRestored(NavigationController* controller,
                   bool pinned);

  
  
  void SetSelectedNavigationIndex(const SessionID& window_id,
                                  const SessionID& tab_id,
                                  int index);

  
  void SetSelectedTabInWindow(const SessionID& window_id, int index);

  
  
  
  
  
  
  typedef Callback2<Handle, std::vector<SessionWindow*>*>::Type
      SessionCallback;

  
  
  
  
  
  
  
  Handle GetLastSession(CancelableRequestConsumerBase* consumer,
                        SessionCallback* callback);

  
  
  
  
  
  
  
  Handle GetCurrentSession(CancelableRequestConsumerBase* consumer,
                           SessionCallback* callback);

  
  
  virtual void Save();

 private:
  typedef std::map<SessionID::id_type, std::pair<int, int> > IdToRange;
  typedef std::map<SessionID::id_type, SessionTab*> IdToSessionTab;
  typedef std::map<SessionID::id_type, SessionWindow*> IdToSessionWindow;


  virtual ~SessionService();

  
  // specific enumeration _values_ are written into the session database and
  
  enum WindowType {
    TYPE_NORMAL = 0,
    TYPE_POPUP = 1,
    TYPE_APP = 2,
    TYPE_APP_POPUP = TYPE_APP + TYPE_POPUP,
    TYPE_DEVTOOLS = TYPE_APP + 4,
    TYPE_APP_PANEL = TYPE_APP + 8
  };

  void Init();

  
  
  bool RestoreIfNecessary(const std::vector<GURL>& urls_to_open,
                          Browser* browser);

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  void SetTabExtensionAppID(const SessionID& window_id,
                            const SessionID& tab_id,
                            const std::string& extension_app_id);

  
  
  SessionCommand* CreateSetSelectedTabInWindow(const SessionID& window_id,
                                               int index);

  SessionCommand* CreateSetTabWindowCommand(const SessionID& window_id,
                                            const SessionID& tab_id);

  SessionCommand* CreateSetWindowBoundsCommand(const SessionID& window_id,
                                               const gfx::Rect& bounds,
                                               bool is_maximized);

  SessionCommand* CreateSetTabIndexInWindowCommand(const SessionID& tab_id,
                                                   int new_index);

  SessionCommand* CreateTabClosedCommand(SessionID::id_type tab_id);

  SessionCommand* CreateWindowClosedCommand(SessionID::id_type tab_id);

  SessionCommand* CreateSetSelectedNavigationIndexCommand(
      const SessionID& tab_id,
      int index);

  SessionCommand* CreateSetWindowTypeCommand(const SessionID& window_id,
                                             WindowType type);

  SessionCommand* CreatePinnedStateCommand(const SessionID& tab_id,
                                           bool is_pinned);

  
  
  void OnGotSessionCommands(
      Handle handle,
      scoped_refptr<InternalGetCommandsRequest> request);

  
  
  
  
  
  
  void RestoreSessionFromCommands(const std::vector<SessionCommand*>& commands,
                                  std::vector<SessionWindow*>* valid_windows);

  
  
  void UpdateSelectedTabIndex(std::vector<SessionWindow*>* windows);

  
  
  SessionWindow* GetWindow(SessionID::id_type window_id,
                           IdToSessionWindow* windows);

  
  
  SessionTab* GetTab(SessionID::id_type tab_id,
                     IdToSessionTab* tabs);

  
  
  
  
  
  std::vector<TabNavigation>::iterator FindClosestNavigationWithIndex(
      std::vector<TabNavigation>* navigations,
      int index);

  
  
  
  
  
  
  
  void SortTabsBasedOnVisualOrderAndPrune(
      std::map<int, SessionWindow*>* windows,
      std::vector<SessionWindow*>* valid_windows);

  
  
  void AddTabsToWindows(std::map<int, SessionTab*>* tabs,
                        std::map<int, SessionWindow*>* windows);

  
  
  
  
  
  
  bool CreateTabsAndWindows(const std::vector<SessionCommand*>& data,
                            std::map<int, SessionTab*>* tabs,
                            std::map<int, SessionWindow*>* windows);

  
  
  
  
  // indices that were written.
  void BuildCommandsForTab(
      const SessionID& window_id,
      NavigationController* controller,
      int index_in_window,
      bool is_pinned,
      std::vector<SessionCommand*>* commands,
      IdToRange* tab_to_available_range);

  
  
  
  void BuildCommandsForBrowser(
      Browser* browser,
      std::vector<SessionCommand*>* commands,
      IdToRange* tab_to_available_range,
      std::set<SessionID::id_type>* windows_to_track);

  
  
  
  
  void BuildCommandsFromBrowsers(
      std::vector<SessionCommand*>* commands,
      IdToRange* tab_to_available_range,
      std::set<SessionID::id_type>* windows_to_track);

  
  
  void ScheduleReset();

  
  
  
  bool ReplacePendingCommand(SessionCommand* command);

  
  
  virtual void ScheduleCommand(SessionCommand* command);

  
  void CommitPendingCloses();

  
  
  bool IsOnlyOneTabLeft();

  
  
  
  
  bool HasOpenTrackableBrowsers(const SessionID& window_id);

  
  bool ShouldTrackChangesToWindow(const SessionID& window_id);

  
  static bool should_track_changes_for_browser_type(Browser::Type type) {
    return type == Browser::TYPE_NORMAL ||
        (type == Browser::TYPE_POPUP && browser_defaults::kRestorePopups);
  }

  
  
  bool should_record_close_as_pending() const {
    
    
    
    
    return !has_open_trackable_browsers_ &&
        (!browser_defaults::kBrowserAliveWithNoWindows ||
         BrowserList::size() > 1);
  }

  
  
  
  
  
  void RecordSessionUpdateHistogramData(NotificationType type,
    base::TimeTicks* last_updated_time);

  
  void RecordUpdatedTabClosed(base::TimeDelta delta, bool use_long_period);
  void RecordUpdatedNavListPruned(base::TimeDelta delta, bool use_long_period);
  void RecordUpdatedNavEntryCommit(base::TimeDelta delta, bool use_long_period);
  void RecordUpdatedSaveTime(base::TimeDelta delta, bool use_long_period);
  void RecordUpdatedSessionNavigationOrTab(base::TimeDelta delta,
                                           bool use_long_period);

  
  
  static WindowType WindowTypeForBrowserType(Browser::Type type);
  static Browser::Type BrowserTypeForWindowType(WindowType type);

  NotificationRegistrar registrar_;

  
  // been written to disk.
  
  
  // written.
  IdToRange tab_to_available_range_;

  
  
  
  
  typedef std::set<SessionID::id_type> PendingWindowCloseIDs;
  PendingWindowCloseIDs pending_window_close_ids_;

  
  
  typedef std::set<SessionID::id_type> PendingTabCloseIDs;
  PendingTabCloseIDs pending_tab_close_ids_;

  
  
  typedef std::set<SessionID::id_type> WindowClosingIDs;
  WindowClosingIDs window_closing_ids_;

  
  
  typedef std::set<SessionID::id_type> WindowsTracking;
  WindowsTracking windows_tracking_;

  
  bool has_open_trackable_browsers_;

  
  
  
  
  bool move_on_new_browser_;

  
  base::TimeTicks last_updated_tab_closed_time_;
  base::TimeTicks last_updated_nav_list_pruned_time_;
  base::TimeTicks last_updated_nav_entry_commit_time_;
  base::TimeTicks last_updated_save_time_;

  
  const base::TimeDelta save_delay_in_millis_;
  const base::TimeDelta save_delay_in_mins_;
  const base::TimeDelta save_delay_in_hrs_;

  DISALLOW_COPY_AND_ASSIGN(SessionService);
};

#endif  
