// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_ACCESSIBILITY_AX_TREE_SERIALIZER_H_
#define UI_ACCESSIBILITY_AX_TREE_SERIALIZER_H_

#include <set>

#include "base/containers/hash_tables.h"
#include "base/logging.h"
#include "base/stl_util.h"
#include "ui/accessibility/ax_tree_source.h"
#include "ui/accessibility/ax_tree_update.h"

namespace ui {

struct ClientTreeNode;

template<class AXSourceNode>
class AXTreeSerializer {
 public:
  explicit AXTreeSerializer(AXTreeSource<AXSourceNode>* tree);
  ~AXTreeSerializer();

  
  
  
  void Reset();

  
  void SerializeChanges(const AXSourceNode* node,
                        AXTreeUpdate* out_update);

  
  
  
  
  
  
  void ChangeTreeSourceForTesting(AXTreeSource<AXSourceNode>* new_tree);

 private:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  const AXSourceNode* LeastCommonAncestor(const AXSourceNode* node,
                                          ClientTreeNode* client_node);

  
  
  
  
  const AXSourceNode* LeastCommonAncestor(const AXSourceNode* node);

  
  
  
  
  bool AnyDescendantWasReparented(const AXSourceNode* node,
                                  const AXSourceNode** lca);

  ClientTreeNode* ClientTreeNodeById(int32 id);

  
  
  void DeleteClientSubtree(ClientTreeNode* client_node);

  
  void SerializeChangedNodes(const AXSourceNode* node,
                             AXTreeUpdate* out_update);

  
  AXTreeSource<AXSourceNode>* tree_;

  
  ClientTreeNode* client_root_;

  
  base::hash_map<int32, ClientTreeNode*> client_id_map_;
};

struct AX_EXPORT ClientTreeNode {
  ClientTreeNode();
  virtual ~ClientTreeNode();
  int32 id;
  ClientTreeNode* parent;
  std::vector<ClientTreeNode*> children;
};

template<class AXSourceNode>
AXTreeSerializer<AXSourceNode>::AXTreeSerializer(
    AXTreeSource<AXSourceNode>* tree)
    : tree_(tree),
      client_root_(NULL) {
}

template<class AXSourceNode>
AXTreeSerializer<AXSourceNode>::~AXTreeSerializer() {
  Reset();
}

template<class AXSourceNode>
void AXTreeSerializer<AXSourceNode>::Reset() {
  if (client_root_) {
    DeleteClientSubtree(client_root_);
    client_root_ = NULL;
  }
}

template<class AXSourceNode>
void AXTreeSerializer<AXSourceNode>::ChangeTreeSourceForTesting(
    AXTreeSource<AXSourceNode>* new_tree) {
  tree_ = new_tree;
}

template<class AXSourceNode>
const AXSourceNode* AXTreeSerializer<AXSourceNode>::LeastCommonAncestor(
    const AXSourceNode* node, ClientTreeNode* client_node) {
  if (node == NULL || client_node == NULL)
    return NULL;

  std::vector<const AXSourceNode*> ancestors;
  while (node) {
    ancestors.push_back(node);
    node = tree_->GetParent(node);
  }

  std::vector<ClientTreeNode*> client_ancestors;
  while (client_node) {
    client_ancestors.push_back(client_node);
    client_node = client_node->parent;
  }

  
  
  
  const AXSourceNode* lca = NULL;
  int source_index = static_cast<int>(ancestors.size() - 1);
  int client_index = static_cast<int>(client_ancestors.size() - 1);
  while (source_index >= 0 && client_index >= 0) {
    if (tree_->GetId(ancestors[source_index]) !=
            client_ancestors[client_index]->id) {
      return lca;
    }
    lca = ancestors[source_index];
    source_index--;
    client_index--;
  }
  return lca;
}

template<class AXSourceNode>
const AXSourceNode* AXTreeSerializer<AXSourceNode>::LeastCommonAncestor(
    const AXSourceNode* node) {
  
  
  ClientTreeNode* client_node = ClientTreeNodeById(tree_->GetId(node));
  while (node && !client_node) {
    node = tree_->GetParent(node);
    if (node)
      client_node = ClientTreeNodeById(tree_->GetId(node));
  }
  return LeastCommonAncestor(node, client_node);
}

template<class AXSourceNode>
bool AXTreeSerializer<AXSourceNode>::AnyDescendantWasReparented(
    const AXSourceNode* node, const AXSourceNode** lca) {
  bool result = false;
  int id = tree_->GetId(node);
  int child_count = tree_->GetChildCount(node);
  for (int i = 0; i < child_count; ++i) {
    const AXSourceNode* child = tree_->GetChildAtIndex(node, i);
    int child_id = tree_->GetId(child);
    ClientTreeNode* client_child = ClientTreeNodeById(child_id);
    if (client_child) {
      if (!client_child->parent) {
        
        
        *lca = NULL;
        return true;
      } else if (client_child->parent->id != id) {
        
        
        *lca = LeastCommonAncestor(*lca, client_child);
        result = true;
      } else {
        
        
        
        continue;
      }
    }

    
    if (AnyDescendantWasReparented(child, lca))
      result = true;
  }
  return result;
}

template<class AXSourceNode>
ClientTreeNode* AXTreeSerializer<AXSourceNode>::ClientTreeNodeById(int32 id) {
  base::hash_map<int32, ClientTreeNode*>::iterator iter =
      client_id_map_.find(id);
  if (iter != client_id_map_.end())
    return iter->second;
  else
    return NULL;
}

template<class AXSourceNode>
void AXTreeSerializer<AXSourceNode>::SerializeChanges(
    const AXSourceNode* node,
    AXTreeUpdate* out_update) {
  
  
  const AXSourceNode* lca = LeastCommonAncestor(node);

  if (client_root_) {
    
    
    bool need_delete = (lca != node);
    if (lca) {
      
      
      
      if (AnyDescendantWasReparented(lca, &lca))
        need_delete = true;
    }

    if (lca == NULL) {
      
      
      out_update->node_id_to_clear = client_root_->id;
      DeleteClientSubtree(client_root_);
      client_id_map_.erase(client_root_->id);
      client_root_ = NULL;
    } else if (need_delete) {
      
      
      
      out_update->node_id_to_clear = tree_->GetId(lca);
      ClientTreeNode* client_lca = ClientTreeNodeById(tree_->GetId(lca));
      CHECK(client_lca);
      for (size_t i = 0; i < client_lca->children.size(); ++i) {
        client_id_map_.erase(client_lca->children[i]->id);
        DeleteClientSubtree(client_lca->children[i]);
      }
      client_lca->children.clear();
    }
  }

  
  if (!lca)
    lca = tree_->GetRoot();
  SerializeChangedNodes(lca, out_update);
}

template<class AXSourceNode>
void AXTreeSerializer<AXSourceNode>::DeleteClientSubtree(
    ClientTreeNode* client_node) {
  for (size_t i = 0; i < client_node->children.size(); ++i) {
    client_id_map_.erase(client_node->children[i]->id);
    DeleteClientSubtree(client_node->children[i]);
  }
  client_node->children.clear();
}

template<class AXSourceNode>
void AXTreeSerializer<AXSourceNode>::SerializeChangedNodes(
    const AXSourceNode* node,
    AXTreeUpdate* out_update) {
  
  
  
  
  
  
  

  
  
  
  
  int id = tree_->GetId(node);
  ClientTreeNode* client_node = ClientTreeNodeById(id);
  if (!client_node) {
    if (client_root_) {
      client_id_map_.erase(client_root_->id);
      DeleteClientSubtree(client_root_);
    }
    client_root_ = new ClientTreeNode();
    client_node = client_root_;
    client_node->id = id;
    client_node->parent = NULL;
    client_id_map_[client_node->id] = client_node;
  }

  
  
  
  base::hash_set<int32> new_child_ids;
  int child_count = tree_->GetChildCount(node);
  for (int i = 0; i < child_count; ++i) {
    AXSourceNode* child = tree_->GetChildAtIndex(node, i);
    int new_child_id = tree_->GetId(child);
    new_child_ids.insert(new_child_id);

    
    
    ClientTreeNode* client_child = client_id_map_[new_child_id];
    CHECK(!client_child || client_child->parent == client_node);
  }

  
  
  
  
  
  
  base::hash_map<int32, ClientTreeNode*> client_child_id_map;
  std::vector<ClientTreeNode*> old_children;
  old_children.swap(client_node->children);
  for (size_t i = 0; i < old_children.size(); ++i) {
    ClientTreeNode* old_child = old_children[i];
    int old_child_id = old_child->id;
    if (new_child_ids.find(old_child_id) == new_child_ids.end()) {
      client_id_map_.erase(old_child_id);
      DeleteClientSubtree(old_child);
    } else {
      client_child_id_map[old_child_id] = old_child;
    }
  }

  
  
  out_update->nodes.push_back(AXNodeData());
  AXNodeData* serialized_node = &out_update->nodes.back();
  tree_->SerializeNode(node, serialized_node);
  if (serialized_node->id == client_root_->id)
    serialized_node->role = AX_ROLE_ROOT_WEB_AREA;
  serialized_node->child_ids.clear();

  
  
  
  std::vector<AXSourceNode*> children_to_serialize;
  client_node->children.reserve(child_count);
  for (int i = 0; i < child_count; ++i) {
    AXSourceNode* child = tree_->GetChildAtIndex(node, i);
    int child_id = tree_->GetId(child);

    
    
    if (new_child_ids.find(child_id) == new_child_ids.end())
      continue;

    new_child_ids.erase(child_id);
    serialized_node->child_ids.push_back(child_id);
    if (client_child_id_map.find(child_id) != client_child_id_map.end()) {
      ClientTreeNode* reused_child = client_child_id_map[child_id];
      client_node->children.push_back(reused_child);
    } else {
      ClientTreeNode* new_child = new ClientTreeNode();
      new_child->id = child_id;
      new_child->parent = client_node;
      client_node->children.push_back(new_child);
      client_id_map_[child_id] = new_child;
      children_to_serialize.push_back(child);
    }
  }

  
  for (size_t i = 0; i < children_to_serialize.size(); ++i)
    SerializeChangedNodes(children_to_serialize[i], out_update);
}

}  

#endif  
