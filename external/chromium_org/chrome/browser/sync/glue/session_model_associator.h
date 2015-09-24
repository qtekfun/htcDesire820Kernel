// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_SESSION_MODEL_ASSOCIATOR_H_
#define CHROME_BROWSER_SYNC_GLUE_SESSION_MODEL_ASSOCIATOR_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "chrome/browser/sessions/session_id.h"
#include "chrome/browser/sessions/session_service.h"
#include "chrome/browser/sessions/session_types.h"
#include "chrome/browser/sync/glue/favicon_cache.h"
#include "chrome/browser/sync/glue/model_associator.h"
#include "chrome/browser/sync/glue/synced_session_tracker.h"
#include "chrome/browser/sync/glue/tab_node_pool.h"
#include "chrome/browser/sync/open_tabs_ui_delegate.h"
#include "sync/internal_api/public/base/model_type.h"

class PrefServiceSyncable;
class Profile;
class ProfileSyncService;

namespace syncer {
class ReadNode;
class WriteTransaction;
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
class DeviceInfo;
class SyncedTabDelegate;
class SyncedWindowDelegate;

class SessionModelAssociator
    : public AssociatorInterface,
      public OpenTabsUIDelegate,
      public base::SupportsWeakPtr<SessionModelAssociator>,
      public base::NonThreadSafe {
 public:
  
  SessionModelAssociator(ProfileSyncService* sync_service,
                         DataTypeErrorHandler* error_handler);
  SessionModelAssociator(ProfileSyncService* sync_service,
                         bool setup_for_test);
  virtual ~SessionModelAssociator();

  
  
  
  virtual bool SyncModelHasUserCreatedNodes(bool* has_nodes) OVERRIDE;

  
  virtual void AbortAssociation() OVERRIDE {
    
  }

  
  virtual bool CryptoReadyIfNecessary() OVERRIDE;

  
  
  
  virtual int64 GetSyncIdFromSessionTag(const std::string& tag);

  
  
  
  
  
  
  
  
  
  
  bool AssociateWindows(bool reload_tabs, syncer::SyncError* error);

  
  
  
  
  bool AssociateTabs(const std::vector<SyncedTabDelegate*>& tabs,
                     syncer::SyncError* error);

  
  
  
  
  
  
  bool AssociateTab(SyncedTabDelegate* const tab, syncer::SyncError* error);

  
  
  
  
  virtual syncer::SyncError AssociateModels(
      syncer::SyncMergeResult* local_merge_result,
      syncer::SyncMergeResult* syncer_merge_result) OVERRIDE;

  
  
  virtual syncer::SyncError DisassociateModels() OVERRIDE;

  
  
  const std::string& GetCurrentMachineTag() const {
    DCHECK(!current_machine_tag_.empty());
    return current_machine_tag_;
  }

  
  void AssociateForeignSpecifics(const sync_pb::SessionSpecifics& specifics,
                                 const base::Time& modification_time);

  
  
  
  bool DisassociateForeignSession(const std::string& foreign_session_tag);

  
  
  
  
  void AttemptSessionsDataRefresh() const;

  
  
  bool GetLocalSession(const SyncedSession* * local_session);

  
  
  
  void DeleteStaleSessions();

  
  
  void SetStaleSessionThreshold(size_t stale_session_threshold_days);

  
  
  
  bool ShouldSyncTab(const SyncedTabDelegate& tab) const;

  
  
  void FaviconsUpdated(const std::set<GURL>& urls);

  
  static syncer::ModelType model_type() { return syncer::SESSIONS; }

  
  
  
  void BlockUntilLocalChangeForTest(base::TimeDelta timeout);

  
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

  void SetCurrentMachineTagForTesting(const std::string& machine_tag) {
    current_machine_tag_ = machine_tag;
  }

  
  scoped_ptr<browser_sync::DeviceInfo> GetDeviceInfoForSessionTag(
      const std::string& session_tag);

  FaviconCache* GetFaviconCache();

 private:
  friend class SyncSessionModelAssociatorTest;
  FRIEND_TEST_ALL_PREFIXES(ProfileSyncServiceSessionTest, WriteSessionToNode);
  FRIEND_TEST_ALL_PREFIXES(ProfileSyncServiceSessionTest,
                           WriteFilledSessionToNode);
  FRIEND_TEST_ALL_PREFIXES(ProfileSyncServiceSessionTest,
                           WriteForeignSessionToNode);
  FRIEND_TEST_ALL_PREFIXES(ProfileSyncServiceSessionTest, TabNodePoolEmpty);
  FRIEND_TEST_ALL_PREFIXES(ProfileSyncServiceSessionTest, TabNodePoolNonEmpty);
  FRIEND_TEST_ALL_PREFIXES(ProfileSyncServiceSessionTest, ValidTabs);
  FRIEND_TEST_ALL_PREFIXES(ProfileSyncServiceSessionTest, ExistingTabs);
  FRIEND_TEST_ALL_PREFIXES(ProfileSyncServiceSessionTest, MissingLocalTabNode);
  FRIEND_TEST_ALL_PREFIXES(ProfileSyncServiceSessionTest,
                           TabPoolFreeNodeLimits);
  FRIEND_TEST_ALL_PREFIXES(ProfileSyncServiceSessionTest,
                           TabNodePoolDeleteUnassociatedNodes);
  FRIEND_TEST_ALL_PREFIXES(SyncSessionModelAssociatorTest,
                           PopulateSessionHeader);
  FRIEND_TEST_ALL_PREFIXES(SyncSessionModelAssociatorTest,
                           PopulateSessionWindow);
  FRIEND_TEST_ALL_PREFIXES(SyncSessionModelAssociatorTest, PopulateSessionTab);
  FRIEND_TEST_ALL_PREFIXES(SyncSessionModelAssociatorTest,
                           TabNodePool);

  
  
  
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
    DISALLOW_COPY_AND_ASSIGN(TabLink);

    
    const int tab_node_id_;

    
    const SyncedTabDelegate* tab_;

    
    GURL url_;
  };

  
  typedef std::map<SessionID::id_type, linked_ptr<TabLink> > TabLinksMap;

  
  static bool ShouldSyncWindow(const SyncedWindowDelegate* window);

  
  void InitializeCurrentMachineTag(syncer::WriteTransaction* trans);

  
  
  
  bool UpdateSyncModelDataFromClient(syncer::SyncError* error);

  
  
  
  bool UpdateAssociationsFromSyncModel(const syncer::ReadNode& root,
                                       syncer::WriteTransaction* trans,
                                       syncer::SyncError* error);

  
  static GURL GetCurrentVirtualURL(const SyncedTabDelegate& tab_delegate);

  
  static GURL GetCurrentFaviconURL(const SyncedTabDelegate& tab_delegate);

  
  
  
  
  bool WriteTabContentsToSyncModel(TabLink* tab_link,
                                   syncer::SyncError* error);

  
  static void SetSessionTabFromDelegate(
      const SyncedTabDelegate& tab_delegate,
      base::Time mtime,
      SessionTab* session_tab);

  
  
  static void PopulateSessionHeaderFromSpecifics(
    const sync_pb::SessionHeader& header_specifics,
    base::Time mtime,
    SyncedSession* session_header);

  
  
  static void PopulateSessionWindowFromSpecifics(
      const std::string& foreign_session_tag,
      const sync_pb::SessionWindow& window,
      base::Time mtime,
      SessionWindow* session_window,
      SyncedSessionTracker* tracker);

  
  
  void LoadForeignTabFavicon(const sync_pb::SessionTab& tab);

  
  
  bool IsValidTab(const SyncedTabDelegate& tab) const;

  
  
  
  
  bool TabHasValidEntry(const SyncedTabDelegate& tab) const;

  
  
  void UpdateTabIdIfNecessary(int tab_node_id,
                              SessionID::id_type new_tab_id);

  
  void QuitLoopForSubtleTesting();

  
  std::string current_machine_tag_;

  
  std::string current_session_name_;

  
  TabNodePool local_tab_pool_;

  
  
  int64 local_session_syncid_;

  
  TabLinksMap local_tab_map_;

  SyncedSessionTracker synced_session_tracker_;

  
  ProfileSyncService* sync_service_;

  
  
  size_t stale_session_threshold_days_;

  
  bool setup_for_test_;

  
  
  bool waiting_for_change_;

  
  Profile* const profile_;

  DataTypeErrorHandler* error_handler_;

  
  FaviconCache favicon_cache_;

  base::WeakPtrFactory<SessionModelAssociator> test_weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(SessionModelAssociator);
};

}  

#endif  
