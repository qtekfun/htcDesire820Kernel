// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SESSIONS_SESSION_SERVICE_H_
#define CHROME_BROWSER_SESSIONS_SESSION_SERVICE_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_vector.h"
#include "base/time/time.h"
#include "chrome/browser/defaults.h"
#include "chrome/browser/sessions/base_session_service.h"
#include "chrome/browser/sessions/session_id.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/browser_list_observer.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/ui_base_types.h"

class Profile;
class SessionCommand;
struct SessionTab;
struct SessionWindow;

namespace content {
class NavigationEntry;
class WebContents;
}


// flushed to SessionBackend and written to a file. Every so often
class SessionService : public BaseSessionService,
                       public BrowserContextKeyedService,
                       public content::NotificationObserver,
                       public chrome::BrowserListObserver {
  friend class SessionServiceTestHelper;
 public:
  
  enum AppType {
    TYPE_APP,
    TYPE_NORMAL
  };

  
  explicit SessionService(Profile* profile);
  
  explicit SessionService(const base::FilePath& save_path);

  virtual ~SessionService();

  
  
  
  
  bool ShouldNewWindowStartSession();

  
  
  
  
  
  bool RestoreIfNecessary(const std::vector<GURL>& urls_to_open);

  
  
  void ResetFromCurrentBrowsers();

  
  
  
  void MoveCurrentSessionToLastSession();

  
  void SetTabWindow(const SessionID& window_id,
                    const SessionID& tab_id);

  
  void SetWindowBounds(const SessionID& window_id,
                       const gfx::Rect& bounds,
                       ui::WindowShowState show_state);

  
  void SetTabIndexInWindow(const SessionID& window_id,
                           const SessionID& tab_id,
                           int new_index);

  
  void SetPinnedState(const SessionID& window_id,
                      const SessionID& tab_id,
                      bool is_pinned);

  
  
  
  
  
  void TabClosed(const SessionID& window_id,
                 const SessionID& tab_id,
                 bool closed_by_user_gesture);

  
  void WindowOpened(Browser* browser);

  
  void WindowClosing(const SessionID& window_id);

  
  void WindowClosed(const SessionID& window_id);

  
  void TabInserted(content::WebContents* contents);

  
  void TabClosing(content::WebContents* contents);

  
  
  
  void SetWindowType(const SessionID& window_id,
                     Browser::Type type,
                     AppType app_type);

  
  void SetWindowAppName(const SessionID& window_id,
                        const std::string& app_name);

  
  
  void TabNavigationPathPrunedFromBack(const SessionID& window_id,
                                       const SessionID& tab_id,
                                       int count);

  
  
  void TabNavigationPathPrunedFromFront(const SessionID& window_id,
                                        const SessionID& tab_id,
                                        int count);

  
  void UpdateTabNavigation(
      const SessionID& window_id,
      const SessionID& tab_id,
      const sessions::SerializedNavigationEntry& navigation);

  
  
  void TabRestored(content::WebContents* tab, bool pinned);

  
  
  void SetSelectedNavigationIndex(const SessionID& window_id,
                                  const SessionID& tab_id,
                                  int index);

  
  void SetSelectedTabInWindow(const SessionID& window_id, int index);

  
  void SetTabUserAgentOverride(const SessionID& window_id,
                               const SessionID& tab_id,
                               const std::string& user_agent_override);

  
  
  typedef base::Callback<void(ScopedVector<SessionWindow>, SessionID::id_type)>
      SessionCallback;

  
  
  
  CancelableTaskTracker::TaskId GetLastSession(const SessionCallback& callback,
                                               CancelableTaskTracker* tracker);

  
  
  virtual void Save() OVERRIDE;

 private:
  
  FRIEND_TEST_ALL_PREFIXES(SessionServiceTest, RestoreActivation1);
  FRIEND_TEST_ALL_PREFIXES(SessionServiceTest, RestoreActivation2);
  FRIEND_TEST_ALL_PREFIXES(NoStartupWindowTest, DontInitSessionServiceForApps);

  typedef std::map<SessionID::id_type, std::pair<int, int> > IdToRange;
  typedef std::map<SessionID::id_type, SessionTab*> IdToSessionTab;
  typedef std::map<SessionID::id_type, SessionWindow*> IdToSessionWindow;


  
  // specific enumeration _values_ are written into the session database and
  
  
  enum WindowType {
    TYPE_TABBED = 0,
    TYPE_POPUP = 1
  };

  void Init();

  
  
  bool processed_any_commands();

  
  
  bool RestoreIfNecessary(const std::vector<GURL>& urls_to_open,
                          Browser* browser);

  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void OnBrowserAdded(Browser* browser) OVERRIDE {}
  virtual void OnBrowserRemoved(Browser* browser) OVERRIDE {}
  virtual void OnBrowserSetLastActive(Browser* browser) OVERRIDE;

  
  void SetTabExtensionAppID(const SessionID& window_id,
                            const SessionID& tab_id,
                            const std::string& extension_app_id);

  
  
  SessionCommand* CreateSetSelectedTabInWindow(const SessionID& window_id,
                                               int index);

  SessionCommand* CreateSetTabWindowCommand(const SessionID& window_id,
                                            const SessionID& tab_id);

  SessionCommand* CreateSetWindowBoundsCommand(const SessionID& window_id,
                                               const gfx::Rect& bounds,
                                               ui::WindowShowState show_state);

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

  SessionCommand* CreateSessionStorageAssociatedCommand(
      const SessionID& tab_id,
      const std::string& session_storage_persistent_id);

  SessionCommand* CreateSetActiveWindowCommand(const SessionID& window_id);

  
  void OnGotSessionCommands(const SessionCallback& callback,
                            ScopedVector<SessionCommand> commands);

  
  
  
  
  
  void RestoreSessionFromCommands(const std::vector<SessionCommand*>& commands,
                                  std::vector<SessionWindow*>* valid_windows,
                                  SessionID::id_type* active_window_id);

  
  
  void UpdateSelectedTabIndex(std::vector<SessionWindow*>* windows);

  
  
  SessionWindow* GetWindow(SessionID::id_type window_id,
                           IdToSessionWindow* windows);

  
  
  SessionTab* GetTab(SessionID::id_type tab_id,
                     IdToSessionTab* tabs);

  
  
  
  
  
  std::vector<sessions::SerializedNavigationEntry>::iterator
  FindClosestNavigationWithIndex(
      std::vector<sessions::SerializedNavigationEntry>* navigations,
      int index);

  
  
  
  
  
  
  
  void SortTabsBasedOnVisualOrderAndPrune(
      std::map<int, SessionWindow*>* windows,
      std::vector<SessionWindow*>* valid_windows);

  
  
  void AddTabsToWindows(std::map<int, SessionTab*>* tabs,
                        std::map<int, SessionWindow*>* windows);

  
  
  
  
  
  
  
  bool CreateTabsAndWindows(const std::vector<SessionCommand*>& data,
                            std::map<int, SessionTab*>* tabs,
                            std::map<int, SessionWindow*>* windows,
                            SessionID::id_type* active_window_id);

  
  
  
  
  // indices that were written.
  void BuildCommandsForTab(
      const SessionID& window_id,
      content::WebContents* tab,
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

  
  
  virtual void ScheduleCommand(SessionCommand* command) OVERRIDE;

  
  void CommitPendingCloses();

  
  
  bool IsOnlyOneTabLeft() const;

  
  
  
  
  bool HasOpenTrackableBrowsers(const SessionID& window_id) const;

  
  bool ShouldTrackChangesToWindow(const SessionID& window_id) const;

  
  bool ShouldTrackBrowser(Browser* browser) const;

  
  static bool should_track_changes_for_browser_type(
      Browser::Type type,
      AppType app_type);

  
  
  bool should_record_close_as_pending() const {
    
    
    
    
    return !has_open_trackable_browsers_ &&
        (!browser_defaults::kBrowserAliveWithNoWindows ||
         force_browser_not_alive_with_no_windows_ ||
         chrome::GetTotalBrowserCount() > 1);
  }

  
  
  
  
  
  void RecordSessionUpdateHistogramData(int type,
    base::TimeTicks* last_updated_time);

  
  void RecordUpdatedTabClosed(base::TimeDelta delta, bool use_long_period);
  void RecordUpdatedNavListPruned(base::TimeDelta delta, bool use_long_period);
  void RecordUpdatedNavEntryCommit(base::TimeDelta delta, bool use_long_period);
  void RecordUpdatedSaveTime(base::TimeDelta delta, bool use_long_period);
  void RecordUpdatedSessionNavigationOrTab(base::TimeDelta delta,
                                           bool use_long_period);

  
  void MaybeDeleteSessionOnlyData();

  
  
  static WindowType WindowTypeForBrowserType(Browser::Type type);
  static Browser::Type BrowserTypeForWindowType(WindowType type);

  content::NotificationRegistrar registrar_;

  
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

  
  
  bool force_browser_not_alive_with_no_windows_;

  DISALLOW_COPY_AND_ASSIGN(SessionService);
};

#endif  
