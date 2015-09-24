// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_TAB_NODE_POOL_H_
#define CHROME_BROWSER_SYNC_GLUE_TAB_NODE_POOL_H_

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "chrome/browser/sessions/session_id.h"

class ProfileSyncService;

namespace browser_sync {


class TabNodePool {
 public:
  explicit TabNodePool(ProfileSyncService* sync_service);
  ~TabNodePool();
  enum InvalidTab {
    kInvalidTabID = -1
  };

  
  
  static const size_t kFreeNodesLowWatermark;

  
  static const size_t kFreeNodesHighWatermark;

  static const int kInvalidTabNodeID;

  
  static std::string TabIdToTag(const std::string machine_tag,
                                int tab_node_id);

  
  
  
  
  
  
  int GetFreeTabNode();

  
  void FreeTabNode(int tab_node_id);

  
  
  
  
  void AssociateTabNode(int tab_node_id, SessionID::id_type tab_id);

  
  
  
  
  void AddTabNode(int tab_node_id);

  
  
  SessionID::id_type GetTabIdFromTabNodeId(int tab_node_id) const;

  
  
  void ReassociateTabNode(int tab_node_id, SessionID::id_type tab_id);

  
  bool IsUnassociatedTabNode(int tab_node_id);

  
  void DeleteUnassociatedTabNodes();

  
  void Clear();

  
  
  size_t Capacity() const;

  
  bool Empty() const;

  
  bool Full();

  void SetMachineTag(const std::string& machine_tag);

 private:
  friend class SyncTabNodePoolTest;
  typedef std::map<int, SessionID::id_type> TabNodeIDToTabIDMap;

  
  void FreeTabNodeInternal(int tab_node_id);

  
  
  
  
  TabNodeIDToTabIDMap nodeid_tabid_map_;

  
  std::set<int> free_nodes_pool_;

  
  
  
  std::set<int> unassociated_nodes_;

  
  
  int max_used_tab_node_id_;

  
  
  std::string machine_tag_;

  
  ProfileSyncService* sync_service_;

  DISALLOW_COPY_AND_ASSIGN(TabNodePool);
};

}  

#endif  
