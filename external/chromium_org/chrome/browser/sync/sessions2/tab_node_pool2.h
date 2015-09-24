// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_SESSIONS2_TAB_NODE_POOL2_H_
#define CHROME_BROWSER_SYNC_SESSIONS2_TAB_NODE_POOL2_H_

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "chrome/browser/sessions/session_id.h"
#include "sync/api/sync_change_processor.h"

namespace syncer {
class SyncChangeProcessor;
}

namespace browser_sync {


class TabNodePool2 {
 public:
   TabNodePool2();
  ~TabNodePool2();
  enum InvalidTab {
    kInvalidTabID = -1
  };

  
  
  static const size_t kFreeNodesLowWatermark;

  
  static const size_t kFreeNodesHighWatermark;

  static const int kInvalidTabNodeID;

  
  static std::string TabIdToTag(const std::string machine_tag,
                                int tab_node_id);

  
  
  
  
  
  
  
  
  
  
  
  int GetFreeTabNode(syncer::SyncChangeList* change_output);

  
  
  
  
  
  
  void FreeTabNode(int tab_node_id, syncer::SyncChangeList* change_output);

  
  
  
  
  void AssociateTabNode(int tab_node_id, SessionID::id_type tab_id);

  
  
  
  
  void AddTabNode(int tab_node_id);

  
  
  SessionID::id_type GetTabIdFromTabNodeId(int tab_node_id) const;

  
  
  void ReassociateTabNode(int tab_node_id, SessionID::id_type tab_id);

  
  bool IsUnassociatedTabNode(int tab_node_id);

  
  
  
  
  void DeleteUnassociatedTabNodes(syncer::SyncChangeList* change_output);

  
  void Clear();

  
  
  size_t Capacity() const;

  
  bool Empty() const;

  
  bool Full();

  void SetMachineTag(const std::string& machine_tag);

 private:
  friend class SyncTabNodePool2Test;
  typedef std::map<int, SessionID::id_type> TabNodeIDToTabIDMap;

  
  
  
  
  void FreeTabNodeInternal(int tab_node_id,
                           syncer::SyncChangeList* change_output);

  
  
  
  
  TabNodeIDToTabIDMap nodeid_tabid_map_;

  
  std::set<int> free_nodes_pool_;

  
  
  
  std::set<int> unassociated_nodes_;

  
  
  int max_used_tab_node_id_;

  
  
  std::string machine_tag_;

  DISALLOW_COPY_AND_ASSIGN(TabNodePool2);
};

}  

#endif  
