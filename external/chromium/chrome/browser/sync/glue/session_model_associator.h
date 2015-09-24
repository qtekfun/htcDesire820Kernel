// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_SESSION_MODEL_ASSOCIATOR_H_
#define CHROME_BROWSER_SYNC_GLUE_SESSION_MODEL_ASSOCIATOR_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/format_macros.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_vector.h"
#include "base/observer_list.h"
#include "base/string_util.h"
#include "base/threading/non_thread_safe.h"
#include "base/utf_string_conversions.h"
#include "chrome/browser/sessions/session_id.h"
#include "chrome/browser/sessions/session_service.h"
#include "chrome/browser/sessions/session_types.h"
#include "chrome/browser/sync/engine/syncapi.h"
#include "chrome/browser/sync/glue/foreign_session_tracker.h"
#include "chrome/browser/sync/glue/model_associator.h"
#include "chrome/browser/sync/protocol/session_specifics.pb.h"
#include "chrome/browser/sync/syncable/model_type.h"
#include "chrome/browser/ui/browser_window.h"
#include "content/browser/tab_contents/tab_contents.h"

class Profile;
class ProfileSyncService;

namespace sync_api {
class ReadNode;
class WriteNode;
class WriteTransaction;
}  

namespace sync_pb {
class SessionSpecifics;
}  

namespace browser_sync {

static const char kSessionsTag[] = "google_chrome_sessions";

class SessionModelAssociator
    : public PerDataTypeAssociatorInterface<TabContents, size_t>,
      public base::NonThreadSafe {
 public:
  
  explicit SessionModelAssociator(ProfileSyncService* sync_service);
  SessionModelAssociator(ProfileSyncService* sync_service,
                         bool setup_for_test);
  virtual ~SessionModelAssociator();

  
  
  
  virtual bool SyncModelHasUserCreatedNodes(bool* has_nodes);

  
  virtual void AbortAssociation() {
    
  }

  
  virtual bool CryptoReadyIfNecessary();

  
  
  
  virtual int64 GetSyncIdFromChromeId(const size_t& id);

  
  
  
  virtual int64 GetSyncIdFromSessionTag(const std::string& tag);

  
  virtual const TabContents* GetChromeNodeFromSyncId(int64 sync_id);

  
  virtual bool InitSyncNodeFromChromeId(const size_t& id,
                                        sync_api::BaseNode* sync_node);

  
  
  
  
  
  
  
  void ReassociateWindows(bool reload_tabs);

  
  void ReassociateTabs(const std::vector<TabContents*>& tabs);

  
  
  void ReassociateTab(const TabContents& tab);

  
  
  virtual void Associate(const TabContents* tab, int64 sync_id);

  
  
  virtual void Disassociate(int64 sync_id);

  
  
  
  
  virtual bool AssociateModels();

  
  
  
  virtual bool InitSyncNodeFromChromeId(const std::string& id,
                                        sync_api::BaseNode* sync_node);

  
  
  virtual bool DisassociateModels();

  
  
  inline const std::string& GetCurrentMachineTag() {
    DCHECK(!current_machine_tag_.empty());
    return current_machine_tag_;
  }

  
  bool AssociateForeignSpecifics(const sync_pb::SessionSpecifics& specifics,
                                 int64 modification_time);

  
  void DisassociateForeignSession(const std::string& foreign_session_tag);

  
  
  bool GetAllForeignSessions(std::vector<const ForeignSession*>* sessions);

  
  
  bool GetForeignSession(const std::string& tag,
                         std::vector<SessionWindow*>* windows);

  
  
  
  bool GetForeignTab(const std::string& tag,
                     const SessionID::id_type tab_id,
                     const SessionTab** tab);

  
  
  static bool SessionWindowHasNoTabsToSync(const SessionWindow& window);

  
  
  
  bool IsValidTab(const TabContents& tab);

  
  
  
  static bool IsValidSessionTab(const SessionTab& tab);

  
  static syncable::ModelType model_type() { return syncable::SESSIONS; }

 private:
  FRIEND_TEST_ALL_PREFIXES(ProfileSyncServiceSessionTest, WriteSessionToNode);
  FRIEND_TEST_ALL_PREFIXES(ProfileSyncServiceSessionTest,
                           WriteFilledSessionToNode);
  FRIEND_TEST_ALL_PREFIXES(ProfileSyncServiceSessionTest,
                           WriteForeignSessionToNode);
  FRIEND_TEST_ALL_PREFIXES(ProfileSyncServiceSessionTest, TabNodePoolEmpty);
  FRIEND_TEST_ALL_PREFIXES(ProfileSyncServiceSessionTest, TabNodePoolNonEmpty);
  FRIEND_TEST_ALL_PREFIXES(SessionModelAssociatorTest, PopulateSessionWindow);
  FRIEND_TEST_ALL_PREFIXES(SessionModelAssociatorTest, PopulateSessionTab);

  
  class TabLinks {
   public:
    
    
    TabLinks()
        : sync_id_(0),
          session_tab_(NULL),
          tab_(NULL) {}

    
    
    TabLinks(int64 sync_id, const TabContents* tab)
      : sync_id_(sync_id),
        session_tab_(NULL) {
      tab_ = const_cast<TabContents*>(tab);
    }
    TabLinks(int64 sync_id, const SessionTab* session_tab)
      : sync_id_(sync_id),
        tab_(NULL) {
      session_tab_ = const_cast<SessionTab*>(session_tab);
    }

    inline int64 sync_id() const { return sync_id_; }
    inline const SessionTab* session_tab() const { return session_tab_; }
    inline const TabContents* tab() const { return tab_; }
   private:
    int64 sync_id_;
    SessionTab* session_tab_;
    TabContents* tab_;
  };

  
  
  class TabNodePool {
   public:
    explicit TabNodePool(ProfileSyncService* sync_service);
    ~TabNodePool();

    
    
    
    
    
    void AddTabNode(int64 sync_id);

    
    
    
    
    
    
    
    
    
    
    
    
    int64 GetFreeTabNode();

    
    
    
    
    
    
    void FreeTabNode(int64 sync_id);

    
    inline void clear() { tab_syncid_pool_.clear(); }

    
    
    inline size_t capacity() const { return tab_syncid_pool_.size(); }

    
    inline bool empty() const { return tab_pool_fp_ == -1; }

    
    inline bool full() {
      return tab_pool_fp_ == static_cast<int64>(tab_syncid_pool_.size())-1;
    }

    inline void set_machine_tag(const std::string& machine_tag) {
      machine_tag_ = machine_tag;
    }
   private:
    
    std::vector<int64> tab_syncid_pool_;

    
    
    
    
    int64 tab_pool_fp_;

    
    
    std::string machine_tag_;

    
    ProfileSyncService* sync_service_;

    DISALLOW_COPY_AND_ASSIGN(TabNodePool);
  };

  
  typedef std::map<SessionID::id_type, TabLinks> TabLinksMap;

  
  
  
  void DeleteForeignSessions();

  
  static bool ShouldSyncWindowType(const Browser::Type& type);

  
  static inline std::string TabIdToTag(
      const std::string machine_tag,
      size_t tab_node_id) {
    return StringPrintf("%s %"PRIuS"",
        machine_tag.c_str(), tab_node_id);
  }

  
  void InitializeCurrentMachineTag(sync_api::WriteTransaction* trans);

  
  
  void UpdateSyncModelDataFromClient();

  
  
  
  bool UpdateAssociationsFromSyncModel(const sync_api::ReadNode& root,
                                       const sync_api::BaseTransaction* trans);

  
  
  bool WriteTabContentsToSyncModel(const Browser& browser,
                                   const TabContents& tab,
                                   const int64 sync_id,
                                   sync_api::WriteTransaction* trans);

  
  
  static void PopulateSessionWindowFromSpecifics(
      const std::string& foreign_session_tag,
      const sync_pb::SessionWindow& window,
      const int64 mtime,
      SessionWindow* session_window,
      ForeignSessionTracker* tracker);

  
  static void PopulateSessionTabFromSpecifics(const sync_pb::SessionTab& tab,
                                              const int64 mtime,
                                              SessionTab* session_tab);

  
  static void AppendSessionTabNavigation(
     const sync_pb::TabNavigation& navigation,
     std::vector<TabNavigation>* navigations);

  
  static void PopulateSessionSpecificsNavigation(
     const TabNavigation* navigation,
     sync_pb::TabNavigation* tab_navigation);

  
  SessionService* GetSessionService();

  
  
  void OnGotSession(int handle, std::vector<SessionWindow*>* windows);

  
  void PopulateSessionSpecificsHeader(
      const std::vector<SessionWindow*>& windows,
      sync_pb::SessionHeader* header_s);

  
  void PopulateSessionSpecificsWindow(const SessionWindow& window,
                                      sync_pb::SessionWindow* session_window);

  
  
  bool SyncLocalWindowToSyncModel(const SessionWindow& window);

  
  
  bool WriteSessionTabToSyncModel(const SessionTab& tab,
                                  const int64 sync_id,
                                  sync_api::WriteTransaction* trans);

  
  void PopulateSessionSpecificsTab(const SessionTab& tab,
                                   sync_pb::SessionTab* session_tab);

  
  std::string current_machine_tag_;

  
  TabNodePool tab_pool_;

  
  
  int64 local_session_syncid_;

  
  TabLinksMap tab_map_;

  ForeignSessionTracker foreign_session_tracker_;

  
  ProfileSyncService* sync_service_;

  
  CancelableRequestConsumer consumer_;

  
  bool setup_for_test_;

  DISALLOW_COPY_AND_ASSIGN(SessionModelAssociator);
};

}  

#endif  
