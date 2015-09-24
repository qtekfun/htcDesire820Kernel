// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_FOREIGN_SESSION_TRACKER_H_
#define CHROME_BROWSER_SYNC_GLUE_FOREIGN_SESSION_TRACKER_H_
#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_vector.h"
#include "chrome/browser/sessions/session_id.h"
#include "chrome/browser/sessions/session_types.h"

namespace browser_sync {

class ForeignSessionTracker {
 public:
  ForeignSessionTracker();
  ~ForeignSessionTracker();

  
  
  bool LookupAllForeignSessions(std::vector<const ForeignSession*>* sessions);

  
  
  
  
  
  
  bool LookupSessionWindows(const std::string& foreign_session_tag,
                            std::vector<SessionWindow*>* windows);

  
  
  
  
  
  
  bool LookupSessionTab(const std::string& foreign_session_tag,
                        SessionID::id_type tab_id,
                        const SessionTab** tab);

  
  
  ForeignSession* GetForeignSession(const std::string& foreign_session_tag);

  
  
  
  bool DeleteForeignSession(const std::string& foreign_session_tag);

  
  
  
  
  
  SessionTab* GetSessionTab(const std::string& foreign_session_tag,
                            SessionID::id_type tab_id,
                            bool has_window);

  
  
  void clear();

  inline bool empty() {
    return foreign_tab_map_.empty() && foreign_session_map_.empty();
  }

  inline size_t num_foreign_sessions() {
    return foreign_session_map_.size();
  }

  inline size_t num_foreign_tabs(const std::string& foreign_session_tag) {
    if (foreign_tab_map_.find(foreign_session_tag) != foreign_tab_map_.end()) {
      return foreign_tab_map_[foreign_session_tag]->size();
    } else {
      return 0;
    }
  }
 private:
  
  typedef std::map<SessionID::id_type, SessionTab*> IDToSessionTabMap;
  typedef std::map<std::string, IDToSessionTabMap*> ForeignTabMap;
  typedef std::map<std::string, ForeignSession*> ForeignSessionMap;

  
  ForeignTabMap foreign_tab_map_;

  
  ForeignSessionMap foreign_session_map_;

  
  
  
  std::set<SessionTab*> unmapped_tabs_;

  DISALLOW_COPY_AND_ASSIGN(ForeignSessionTracker);
};

}  

#endif  
