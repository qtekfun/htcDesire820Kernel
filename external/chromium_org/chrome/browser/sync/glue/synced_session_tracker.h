// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_SYNCED_SESSION_TRACKER_H_
#define CHROME_BROWSER_SYNC_GLUE_SYNCED_SESSION_TRACKER_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_vector.h"
#include "chrome/browser/sessions/session_id.h"
#include "chrome/browser/sessions/session_types.h"
#include "chrome/browser/sync/glue/synced_session.h"
#include "chrome/browser/sync/glue/tab_node_pool.h"

namespace browser_sync {

class SyncedSessionTracker {
 public:
  SyncedSessionTracker();
  ~SyncedSessionTracker();

  
  void SetLocalSessionTag(const std::string& local_session_tag);

  
  
  
  
  bool LookupAllForeignSessions(std::vector<const SyncedSession*>* sessions)
      const;

  
  
  
  
  
  
  
  bool LookupSessionWindows(const std::string& session_tag,
                            std::vector<const SessionWindow*>* windows) const;

  
  
  
  
  
  
  bool LookupSessionTab(const std::string& session_tag,
                        SessionID::id_type tab_id,
                        const SessionTab** tab) const;

  
  
  
  SyncedSession* GetSession(const std::string& session_tag);

  
  
  bool DeleteSession(const std::string& session_tag);

  
  
  
  
  
  
  
  
  void ResetSessionTracking(const std::string& session_tag);

  
  
  
  void CleanupSession(const std::string& session_tag);

  
  
  
  
  
  void PutWindowInSession(const std::string& session_tag,
                          SessionID::id_type window_id);

  
  
  
  
  
  void PutTabInWindow(const std::string& session_tag,
                      SessionID::id_type window_id,
                      SessionID::id_type tab_id,
                      size_t tab_index);

  
  
  
  
  SessionTab* GetTab(const std::string& session_tag,
                     SessionID::id_type tab_id,
                     int tab_node_id);

  
  
  
  
  
  
  
  
  
  
  bool LookupTabNodeIds(const std::string& session_tag,
                        std::set<int>* tab_node_ids);

  
  
  void Clear();

  bool Empty() const {
    return synced_tab_map_.empty() && synced_session_map_.empty();
  }

  
  size_t num_synced_sessions() const {
    return synced_session_map_.size();
  }

  
  size_t num_synced_tabs(const std::string& session_tag) const {
    SyncedTabMap::const_iterator iter = synced_tab_map_.find(session_tag);
    if (iter != synced_tab_map_.end()) {
      return iter->second.size();
    } else {
      return 0;
    }
  }
 private:
  
  
  
  
  
  
  
  
  
  
  enum OwnedState {
    IS_OWNED,
    NOT_OWNED
  };
  struct SessionTabWrapper {
    SessionTabWrapper() : tab_ptr(NULL),
                          owned(false),
                          tab_node_id(TabNodePool::kInvalidTabNodeID) {}
    SessionTabWrapper(SessionTab* tab_ptr, OwnedState owned, int tab_node_id)
        : tab_ptr(tab_ptr),
          owned(owned == IS_OWNED),
          tab_node_id(tab_node_id) {}
    SessionTab* tab_ptr;

    
    
    
    
    bool owned;

    
    
    
    int tab_node_id;
  };
  typedef std::map<SessionID::id_type, SessionTabWrapper> IDToSessionTabMap;
  typedef std::map<std::string, IDToSessionTabMap> SyncedTabMap;

  struct SessionWindowWrapper {
    SessionWindowWrapper() : window_ptr(NULL), owned(false) {}
    SessionWindowWrapper(SessionWindow* window_ptr, OwnedState owned)
        : window_ptr(window_ptr),
          owned(owned == IS_OWNED) {}
    SessionWindow* window_ptr;
    bool owned;
  };
  typedef std::map<SessionID::id_type, SessionWindowWrapper>
      IDToSessionWindowMap;
  typedef std::map<std::string, IDToSessionWindowMap> SyncedWindowMap;

  typedef std::map<std::string, SyncedSession*> SyncedSessionMap;

  
  bool DeleteOldSessionWindowIfNecessary(SessionWindowWrapper window_wrapper);
  bool DeleteOldSessionTabIfNecessary(SessionTabWrapper tab_wrapper);

  
  SessionTab* GetTabImpl(const std::string& session_tag,
                         SessionID::id_type tab_id,
                         int tab_node_id);

  
  
  
  SyncedTabMap synced_tab_map_;

  
  
  
  SyncedWindowMap synced_window_map_;

  
  
  
  SyncedSessionMap synced_session_map_;

  
  
  
  
  std::set<SessionTab*> unmapped_tabs_;

  
  
  std::string local_session_tag_;

  DISALLOW_COPY_AND_ASSIGN(SyncedSessionTracker);
};

}  

#endif  
