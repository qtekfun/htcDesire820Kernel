// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_PRIORITY_FOREST_H_
#define NET_SPDY_SPDY_PRIORITY_FOREST_H_

#include <map>
#include <set>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/rand_util.h"

namespace net {

template <typename NodeId, typename Priority>
class SpdyPriorityForest {
 public:
  SpdyPriorityForest();
  ~SpdyPriorityForest();

  
  int num_nodes() const;

  
  bool NodeExists(NodeId node_id) const;

  
  
  bool AddRootNode(NodeId node_id, Priority priority);

  
  
  
  bool AddNonRootNode(NodeId node_id, NodeId parent_id, bool unordered);

  
  
  bool RemoveNode(NodeId node_id);

  
  
  Priority GetPriority(NodeId node_id) const;

  
  
  NodeId GetParent(NodeId node_id) const;

  
  
  
  bool IsNodeUnordered(NodeId node_id) const;

  
  
  NodeId GetChild(NodeId node_id) const;

  
  
  
  bool SetPriority(NodeId node_id, Priority priority);

  
  
  
  
  
  bool SetParent(NodeId node_id, NodeId parent_id, bool unordered);

  
  
  bool IsMarkedReadyToRead(NodeId node_id) const;
  
  
  bool MarkReadyToRead(NodeId node_id);
  bool MarkNoLongerReadyToRead(NodeId node_id);
  
  
  NodeId NextNodeToRead();

  
  
  bool IsMarkedReadyToWrite(NodeId node_id) const;
  
  
  bool MarkReadyToWrite(NodeId node_id);
  bool MarkNoLongerReadyToWrite(NodeId node_id);
  
  
  NodeId NextNodeToWrite();

  
  
  bool ValidateInvariantsForTests() const;

 private:
  enum NodeType { ROOT_NODE, NONROOT_ORDERED, NONROOT_UNORDERED };
  struct Node {
    Node() : type(ROOT_NODE), flags(0), child() {
      depends_on.priority = Priority();
    }
    NodeType type;
    unsigned int flags;  
    union {
      Priority priority;  
      NodeId parent_id;  
    } depends_on;
    NodeId child;  
  };

  typedef base::hash_map<NodeId, Node> NodeMap;

  
  
  static const unsigned int kReadyToRead = (1 << 0);
  
  static const unsigned int kReadyToWrite = (1 << 1);

  
  bool IsMarked(NodeId node_id, unsigned int flag) const;
  
  bool Mark(NodeId node_id, unsigned int flag);
  
  bool Unmark(NodeId node_id, unsigned int flag);
  
  NodeId FirstMarkedNode(unsigned int flag);
  
  const Node* FindNode(NodeId node_id) const;

  NodeMap all_nodes_;  

  DISALLOW_COPY_AND_ASSIGN(SpdyPriorityForest);
};

template <typename NodeId, typename Priority>
SpdyPriorityForest<NodeId, Priority>::SpdyPriorityForest() {}

template <typename NodeId, typename Priority>
SpdyPriorityForest<NodeId, Priority>::~SpdyPriorityForest() {}

template <typename NodeId, typename Priority>
int SpdyPriorityForest<NodeId, Priority>::num_nodes() const {
  return all_nodes_.size();
}

template <typename NodeId, typename Priority>
bool SpdyPriorityForest<NodeId, Priority>::NodeExists(NodeId node_id) const {
  return all_nodes_.count(node_id) != 0;
}

template <typename NodeId, typename Priority>
bool SpdyPriorityForest<NodeId, Priority>::AddRootNode(
    NodeId node_id, Priority priority) {
  if (NodeExists(node_id)) {
    return false;
  }
  Node* new_node = &all_nodes_[node_id];
  new_node->type = ROOT_NODE;
  new_node->depends_on.priority = priority;
  return true;
}

template <typename NodeId, typename Priority>
bool SpdyPriorityForest<NodeId, Priority>::AddNonRootNode(
    NodeId node_id, NodeId parent_id, bool unordered) {
  if (NodeExists(node_id) || !NodeExists(parent_id)) {
    return false;
  }

  Node* parent = &all_nodes_[parent_id];
  if (parent->child != NodeId()) {
    return false;
  }

  Node* new_node = &all_nodes_[node_id];
  new_node->type = (unordered ? NONROOT_UNORDERED : NONROOT_ORDERED);
  new_node->depends_on.parent_id = parent_id;
  parent->child = node_id;
  return true;
}

template <typename NodeId, typename Priority>
bool SpdyPriorityForest<NodeId, Priority>::RemoveNode(NodeId node_id) {
  if (!NodeExists(node_id)) {
    return false;
  }
  const Node& node = all_nodes_[node_id];

  
  
  if (node.type != ROOT_NODE) {
    DCHECK(NodeExists(node.depends_on.parent_id));
    Node* parent = &all_nodes_[node.depends_on.parent_id];
    DCHECK_EQ(node_id, parent->child);
    parent->child = node.child;
  }

  
  if (node.child != NodeId()) {
    DCHECK(NodeExists(node.child));
    Node* child = &all_nodes_[node.child];
    DCHECK_NE(ROOT_NODE, child->type);
    DCHECK_EQ(node_id, child->depends_on.parent_id);
    
    
    child->depends_on = node.depends_on;
    
    
    
    if (node.type == ROOT_NODE) {
      child->type = ROOT_NODE;
    } else if (node.type == NONROOT_ORDERED) {
      child->type = NONROOT_ORDERED;
    }
  }

  
  all_nodes_.erase(node_id);
  return true;
}

template <typename NodeId, typename Priority>
Priority SpdyPriorityForest<NodeId, Priority>::GetPriority(
    NodeId node_id) const {
  const Node* node = FindNode(node_id);
  if (node != NULL && node->type == ROOT_NODE) {
    return node->depends_on.priority;
  } else {
    return Priority();
  }
}

template <typename NodeId, typename Priority>
NodeId SpdyPriorityForest<NodeId, Priority>::GetParent(NodeId node_id) const {
  const Node* node = FindNode(node_id);
  if (node != NULL && node->type != ROOT_NODE) {
    return node->depends_on.parent_id;
  } else {
    return NodeId();
  }
}

template <typename NodeId, typename Priority>
bool SpdyPriorityForest<NodeId, Priority>::IsNodeUnordered(
    NodeId node_id) const {
  const Node* node = FindNode(node_id);
  return node != NULL && node->type == NONROOT_UNORDERED;
}

template <typename NodeId, typename Priority>
NodeId SpdyPriorityForest<NodeId, Priority>::GetChild(NodeId node_id) const {
  const Node* node = FindNode(node_id);
  if (node != NULL) {
    return node->child;
  } else {
    return NodeId();
  }
}

template <typename NodeId, typename Priority>
bool SpdyPriorityForest<NodeId, Priority>::SetPriority(
    NodeId node_id, Priority priority) {
  if (!NodeExists(node_id)) {
    return false;
  }

  Node* node = &all_nodes_[node_id];
  
  if (node->type != ROOT_NODE) {
    DCHECK(NodeExists(node->depends_on.parent_id));
    Node* parent = &all_nodes_[node->depends_on.parent_id];
    parent->child = NodeId();
    node->type = ROOT_NODE;
  }

  node->depends_on.priority = priority;
  return true;
}

template <typename NodeId, typename Priority>
bool SpdyPriorityForest<NodeId, Priority>::SetParent(
    NodeId node_id, NodeId parent_id, bool unordered) {
  if (!NodeExists(node_id) || !NodeExists(parent_id)) {
    return false;
  }

  Node* node = &all_nodes_[node_id];
  Node* new_parent = &all_nodes_[parent_id];
  
  
  if (new_parent->child == node_id) {
    node->type = (unordered ? NONROOT_UNORDERED : NONROOT_ORDERED);
    return true;
  }
  
  if (new_parent->child != NodeId()) {
    return false;
  }

  
  if (node_id == parent_id) return false;
  Node* last = node;
  NodeId last_id = node_id;
  while (last->child != NodeId()) {
    if (last->child == parent_id) return false;
    last_id = last->child;
    DCHECK(NodeExists(last_id));
    last = &all_nodes_[last_id];
  }

  
  
  if (node->type != ROOT_NODE) {
    const NodeId old_parent_id = node->depends_on.parent_id;
    DCHECK(NodeExists(old_parent_id));
    DCHECK(old_parent_id != parent_id);
    Node* old_parent = &all_nodes_[old_parent_id];
    DCHECK_EQ(node_id, old_parent->child);
    old_parent->child = NodeId();
  }

  
  node->type = (unordered ? NONROOT_UNORDERED : NONROOT_ORDERED);
  node->depends_on.parent_id = parent_id;
  new_parent->child = node_id;
  return true;
}

template <typename NodeId, typename Priority>
bool SpdyPriorityForest<NodeId, Priority>::IsMarkedReadyToRead(
    NodeId node_id) const {
  return IsMarked(node_id, kReadyToRead);
}

template <typename NodeId, typename Priority>
bool SpdyPriorityForest<NodeId, Priority>::MarkReadyToRead(NodeId node_id) {
  return Mark(node_id, kReadyToRead);
}

template <typename NodeId, typename Priority>
bool SpdyPriorityForest<NodeId, Priority>::MarkNoLongerReadyToRead(
    NodeId node_id) {
  return Unmark(node_id, kReadyToRead);
}

template <typename NodeId, typename Priority>
NodeId SpdyPriorityForest<NodeId, Priority>::NextNodeToRead() {
  return FirstMarkedNode(kReadyToRead);
}

template <typename NodeId, typename Priority>
bool SpdyPriorityForest<NodeId, Priority>::IsMarkedReadyToWrite(
    NodeId node_id) const {
  return IsMarked(node_id, kReadyToWrite);
}

template <typename NodeId, typename Priority>
bool SpdyPriorityForest<NodeId, Priority>::MarkReadyToWrite(NodeId node_id) {
  return Mark(node_id, kReadyToWrite);
}

template <typename NodeId, typename Priority>
bool SpdyPriorityForest<NodeId, Priority>::MarkNoLongerReadyToWrite(
    NodeId node_id) {
  return Unmark(node_id, kReadyToWrite);
}

template <typename NodeId, typename Priority>
NodeId SpdyPriorityForest<NodeId, Priority>::NextNodeToWrite() {
  return FirstMarkedNode(kReadyToWrite);
}

template <typename NodeId, typename Priority>
bool SpdyPriorityForest<NodeId, Priority>::IsMarked(
    NodeId node_id, unsigned int flag) const {
  const Node* node = FindNode(node_id);
  return node != NULL && (node->flags & flag) != 0;
}

template <typename NodeId, typename Priority>
bool SpdyPriorityForest<NodeId, Priority>::Mark(
    NodeId node_id, unsigned int flag) {
  if (!NodeExists(node_id)) {
    return false;
  }
  all_nodes_[node_id].flags |= flag;
  return true;
}

template <typename NodeId, typename Priority>
bool SpdyPriorityForest<NodeId, Priority>::Unmark(
    NodeId node_id, unsigned int flag) {
  if (!NodeExists(node_id)) {
    return false;
  }
  all_nodes_[node_id].flags &= ~flag;
  return true;
}

template <typename NodeId, typename Priority>
NodeId SpdyPriorityForest<NodeId, Priority>::FirstMarkedNode(
    unsigned int flag) {
  

  
  uint64 total_weight = 0;
  std::map<uint64, NodeId> roots;  
  for (typename NodeMap::const_iterator iter = all_nodes_.begin();
       iter != all_nodes_.end(); ++iter) {
    const NodeId root_id = iter->first;
    const Node& root = iter->second;
    if (root.type == ROOT_NODE) {
      
      for (const Node* node = &root; ; node = &all_nodes_[node->child]) {
        if ((node->flags & flag) != 0) {
          total_weight += static_cast<uint64>(root.depends_on.priority);
          roots[total_weight] = root_id;
          break;
        }
        if (node->child == NodeId()) {
          break;
        }
        DCHECK(NodeExists(node->child));
      }
    }
  }

  
  if (total_weight == 0) {
    DCHECK(roots.empty());
    return NodeId();
  } else {
    DCHECK(!roots.empty());
  }

  
  typename std::map<uint64, NodeId>::const_iterator root_iter =
      roots.upper_bound(base::RandGenerator(total_weight));
  DCHECK(root_iter != roots.end());
  const NodeId root_id = root_iter->second;

  
  NodeId node_id = root_id;
  while (true) {
    DCHECK(NodeExists(node_id));
    Node* node = &all_nodes_[node_id];
    if ((node->flags & flag) != 0) {
      
      
      std::vector<NodeId> group;
      group.push_back(node_id);
      for (Node* next = node; next->child != NodeId();) {
        DCHECK(NodeExists(next->child));
        Node *child = &all_nodes_[next->child];
        DCHECK_NE(ROOT_NODE, child->type);
        if (child->type != NONROOT_UNORDERED) {
          break;
        }
        if ((child->flags & flag) != 0) {
          group.push_back(next->child);
        }
        next = child;
      }
      return group[base::RandGenerator(group.size())];
    }
    node_id = node->child;
  }
}

template <typename NodeId, typename Priority>
const typename SpdyPriorityForest<NodeId, Priority>::Node*
SpdyPriorityForest<NodeId, Priority>::FindNode(NodeId node_id) const {
  typename NodeMap::const_iterator iter = all_nodes_.find(node_id);
  if (iter == all_nodes_.end()) {
    return NULL;
  }
  return &iter->second;
}

template <typename NodeId, typename Priority>
bool SpdyPriorityForest<NodeId, Priority>::ValidateInvariantsForTests() const {
  for (typename NodeMap::const_iterator iter = all_nodes_.begin();
       iter != all_nodes_.end(); ++iter) {
    const NodeId node_id = iter->first;
    const Node& node = iter->second;
    if (node.type != ROOT_NODE &&
        (!NodeExists(node.depends_on.parent_id) ||
         GetChild(node.depends_on.parent_id) != node_id)) {
      return false;
    }
    if (node.child != NodeId()) {
      if (!NodeExists(node.child) || node_id != GetParent(node.child)) {
        return false;
      }
    }

    NodeId child_id = node.child;
    int count = 0;
    while (child_id != NodeId()) {
      if (count > num_nodes() || node_id == child_id) {
        return false;
      }
      child_id = GetChild(child_id);
      ++count;
    }
  }
  return true;
}

}  

#endif  
