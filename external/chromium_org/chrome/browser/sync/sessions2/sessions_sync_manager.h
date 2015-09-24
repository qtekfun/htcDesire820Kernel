// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_SESSIONS2_SESSIONS_SYNC_MANAGER_H_
#define CHROME_BROWSER_SYNC_SESSIONS2_SESSIONS_SYNC_MANAGER_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "chrome/browser/sessions/session_id.h"
#include "chrome/browser/sessions/session_types.h"
#include "chrome/browser/sync/glue/device_info.h"
#include "chrome/browser/sync/glue/favicon_cache.h"
#include "chrome/browser/sync/glue/synced_session.h"
#include "chrome/browser/sync/glue/synced_session_tracker.h"
#include "chrome/browser/sync/open_tabs_ui_delegate.h"
#include "chrome/browser/sync/sessions2/tab_node_pool2.h"
#include "chrome/browser/sync/sync_prefs.h"
#include "sync/api/syncable_service.h"

class Profile;

namespace syncer {
class SyncErrorFactory;
}

namespace sync_pb {
class SessionHeader;
class SessionSpecifics;
class SessionTab;
class SessionWindow;
class TabNavigation;
}  

namespace browser_sync {

class DataTypeErrorHandler;
class SyncedTabDelegate;
class SyncedWindowDelegate;

class LocalSessionEventHandler {
 public:
  
  
  virtual void OnLocalTabModified(SyncedTabDelegate* modified_tab) = 0;

  
  
  
  
  
  virtual void OnFaviconPageUrlsUpdated(
      const std::set<GURL>& updated_page_urls) = 0;
};

class LocalSessionEventRouter {
 public:
  virtual ~LocalSessionEventRouter();
  virtual void StartRoutingTo(LocalSessionEventHandler* handler) = 0;
  virtual void Stop() = 0;
};

class SessionsSyncManager : public syncer::SyncableService,
                            public OpenTabsUIDelegate,
                            public LocalSessionEventHandler {
 public:
  
  class SyncInternalApiDelegate {
   public:
    virtual ~SyncInternalApiDelegate() {}

    
    
    virtual scoped_ptr<DeviceInfo> GetLocalDeviceInfo() const = 0;

    
    virtual std::string GetLocalSyncCacheGUID() const = 0;
  };

  SessionsSyncManager(Profile* profile,
                      SyncInternalApiDelegate* delegate,
                      scoped_ptr<LocalSessionEventRouter> router);
  virtual ~SessionsSyncManager();

  
  virtual syncer::SyncMergeResult MergeDataAndStartSyncing(
      syncer::ModelType type,
      const syncer::SyncDataList& initial_sync_data,
      scoped_ptr<syncer::SyncChangeProcessor> sync_processor,
      scoped_ptr<syncer::SyncErrorFactory> error_handler) OVERRIDE;
  virtual void StopSyncing(syncer::ModelType type) OVERRIDE;
  virtual syncer::SyncDataList GetAllSyncData(
      syncer::ModelType type) const OVERRIDE;
  virtual syncer::SyncError ProcessSyncChanges(
      const tracked_objects::Location& from_here,
      const syncer::SyncChangeList& change_list) OVERRIDE;

  
  virtual bool GetSyncedFaviconForPageURL(
      const std::string& pageurl,
      scoped_refptr<base::RefCountedMemory>* favicon_png) const OVERRIDE;
  virtual bool GetAllForeignSessions(
      std::vector<const SyncedSession*>* sessions) OVERRIDE;
  virtual bool GetForeignSession(
      const std::string& tag,
      std::vector<const SessionWindow*>* windows) OVERRIDE;
  virtual bool GetForeignTab(const std::string& tag,
                             const SessionID::id_type tab_id,
                             const SessionTab** tab) OVERRIDE;
  virtual void DeleteForeignSession(const std::string& tag) OVERRIDE;

  
  virtual void OnLocalTabModified(SyncedTabDelegate* modified_tab) OVERRIDE;
  virtual void OnFaviconPageUrlsUpdated(
      const std::set<GURL>& updated_favicon_page_urls) OVERRIDE;

  
  
  const std::string& current_machine_tag() const {
    DCHECK(!current_machine_tag_.empty());
    return current_machine_tag_;
  }

  
  static GURL GetCurrentVirtualURL(const SyncedTabDelegate& tab_delegate);

  
  static GURL GetCurrentFaviconURL(const SyncedTabDelegate& tab_delegate);

  FaviconCache* GetFaviconCache();

  
  
  
  
  void DoGarbageCollection();

 private:
  
  
  
  class TabLink {
   public:
    TabLink(int tab_node_id, const SyncedTabDelegate* tab)
      : tab_node_id_(tab_node_id),
        tab_(tab) {}

    void set_tab(const SyncedTabDelegate* tab) { tab_ = tab; }
    void set_url(const GURL& url) { url_ = url; }

    int tab_node_id() const { return tab_node_id_; }
    const SyncedTabDelegate* tab() const { return tab_; }
    const GURL& url() const { return url_; }

   private:
    
    const int tab_node_id_;

    
    const SyncedTabDelegate* tab_;

    
    GURL url_;

    DISALLOW_COPY_AND_ASSIGN(TabLink);
  };

  
  typedef std::map<SessionID::id_type, linked_ptr<TabLink> > TabLinksMap;

  FRIEND_TEST_ALL_PREFIXES(SessionsSyncManagerTest, PopulateSessionHeader);
  FRIEND_TEST_ALL_PREFIXES(SessionsSyncManagerTest, PopulateSessionWindow);
  FRIEND_TEST_ALL_PREFIXES(SessionsSyncManagerTest, ValidTabs);
  FRIEND_TEST_ALL_PREFIXES(SessionsSyncManagerTest, SetSessionTabFromDelegate);
  FRIEND_TEST_ALL_PREFIXES(SessionsSyncManagerTest, BlockedNavigations);
  FRIEND_TEST_ALL_PREFIXES(SessionsSyncManagerTest, DeleteForeignSession);
  FRIEND_TEST_ALL_PREFIXES(SessionsSyncManagerTest,
                           SaveUnassociatedNodesForReassociation);
  FRIEND_TEST_ALL_PREFIXES(SessionsSyncManagerTest, MergeDeletesCorruptNode);
  FRIEND_TEST_ALL_PREFIXES(SessionsSyncManagerTest,
                           MergeLocalSessionExistingTabs);
  FRIEND_TEST_ALL_PREFIXES(SessionsSyncManagerTest,
                           CheckPrerenderedWebContentsSwap);
  FRIEND_TEST_ALL_PREFIXES(SessionsSyncManagerTest,
                           AssociateWindowsDontReloadTabs);

  void InitializeCurrentMachineTag();

  
  
  void UpdateTrackerWithForeignSession(
      const sync_pb::SessionSpecifics& specifics,
      const base::Time& modification_time);

  
  
  bool InitFromSyncModel(const syncer::SyncDataList& sync_data,
                         syncer::SyncChangeList* new_changes);

  
  
  
  syncer::SyncChange TombstoneTab(const sync_pb::SessionSpecifics& tab);

  
  
  void RefreshFaviconVisitTimesFromForeignTab(
      const sync_pb::SessionTab& tab, const base::Time& modification_time);

  
  
  
  
  bool DisassociateForeignSession(const std::string& foreign_session_tag);

  
  
  
  
  void DeleteForeignSessionInternal(const std::string& tag,
                                    syncer::SyncChangeList* change_output);

  
  
  static void PopulateSessionHeaderFromSpecifics(
      const sync_pb::SessionHeader& header_specifics,
      base::Time mtime,
      SyncedSession* session_header);

  
  
  void BuildSyncedSessionFromSpecifics(
      const std::string& session_tag,
      const sync_pb::SessionWindow& specifics,
      base::Time mtime,
      SessionWindow* session_window);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  enum ReloadTabsOption {
    RELOAD_TABS,
    DONT_RELOAD_TABS
  };
  void AssociateWindows(ReloadTabsOption option,
                        syncer::SyncChangeList* change_output);

  
  
  
  
  void AssociateTab(SyncedTabDelegate* const tab,
                    syncer::SyncChangeList* change_output);

  
  
  
  bool ShouldSyncTab(const SyncedTabDelegate& tab) const;
  static bool ShouldSyncWindow(const SyncedWindowDelegate* window);

  
  static void SetSessionTabFromDelegate(
      const SyncedTabDelegate& tab_delegate,
      base::Time mtime,
      SessionTab* session_tab);

  
  void LocalTabDelegateToSpecifics(const SyncedTabDelegate& tab_delegate,
                                   sync_pb::SessionSpecifics* specifics);

  
  
  
  
  
  
  
  
  void UpdateTabIdIfNecessary(const SyncedTabDelegate& tab_delegate,
                              SessionID::id_type new_tab_id,
                              syncer::SyncChangeList* change_output);

  
  TabLinksMap local_tab_map_;

  SyncedSessionTracker session_tracker_;
  FaviconCache favicon_cache_;

  
  TabNodePool2 local_tab_pool_;

  SyncPrefs sync_prefs_;

  const Profile* const profile_;

  scoped_ptr<syncer::SyncErrorFactory> error_handler_;
  scoped_ptr<syncer::SyncChangeProcessor> sync_processor_;

  const SyncInternalApiDelegate* const delegate_;

  
  std::string current_machine_tag_;

  
  std::string current_session_name_;

  
  
  int local_session_header_node_id_;

  
  
  size_t stale_session_threshold_days_;

  scoped_ptr<LocalSessionEventRouter> local_event_router_;

  DISALLOW_COPY_AND_ASSIGN(SessionsSyncManager);
};

}  

#endif  
