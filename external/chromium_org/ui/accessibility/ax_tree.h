// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_ACCESSIBILITY_AX_TREE_H_
#define UI_ACCESSIBILITY_AX_TREE_H_

#include <set>

#include "base/containers/hash_tables.h"
#include "ui/accessibility/ax_export.h"
#include "ui/accessibility/ax_tree.h"
#include "ui/accessibility/ax_tree_update.h"

namespace ui {

class AXNode;

class AX_EXPORT AXTree {
 public:
  AXTree();
  explicit AXTree(const AXTreeUpdate& initial_state);
  virtual ~AXTree();

  virtual AXNode* GetRoot() const;
  virtual AXNode* GetFromId(int32 id) const;

  
  
  
  virtual bool Unserialize(const AXTreeUpdate& update);

  
  
  const std::string& error() { return error_; }

 protected:
  
  virtual AXNode* CreateNode(AXNode* parent, int32 id, int32 index_in_parent);

  
  
  
  
  
  virtual bool UpdateNode(const AXNodeData& src,
                          std::set<AXNode*>* pending_nodes);

  
  virtual void OnRootChanged();

 private:
  
  AXNode* CreateAndInitializeNode(
      AXNode* parent, int32 id, int32 index_in_parent);

  
  
  void DestroyNodeAndSubtree(AXNode* node);

  
  
  
  bool DeleteOldChildren(AXNode* node,
                         const std::vector<int32> new_child_ids);

  
  
  
  
  
  
  bool CreateNewChildVector(AXNode* node,
                            const std::vector<int32> new_child_ids,
                            std::vector<AXNode*>* new_children,
                            std::set<AXNode*>* pending_nodes);

  AXNode* root_;
  base::hash_map<int32, AXNode*> id_map_;
  std::string error_;
};

}  

#endif  
