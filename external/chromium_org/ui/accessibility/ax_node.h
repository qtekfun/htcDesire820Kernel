// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_ACCESSIBILITY_AX_NODE_H_
#define UI_ACCESSIBILITY_AX_NODE_H_

#include "ui/accessibility/ax_node_data.h"

namespace ui {

class AX_EXPORT AXNode {
 public:
  
  
  
  AXNode(AXNode* parent, int32 id, int32 index_in_parent);
  virtual ~AXNode();

  
  int32 id() const { return data_.id; }
  AXNode* parent() const { return parent_; }
  int child_count() const { return static_cast<int>(children_.size()); }
  const AXNodeData& data() const { return data_; }
  const std::vector<AXNode*>& children() const { return children_; }

  
  AXNode* ChildAtIndex(int index) const { return children_[index]; }

  
  
  virtual void SetData(const AXNodeData& src);

  
  void SetIndexInParent(int index_in_parent);

  
  
  virtual void SwapChildren(std::vector<AXNode*>& children);

  
  
  
  
  
  virtual void Destroy();

 private:
  int index_in_parent_;
  AXNode* parent_;
  std::vector<AXNode*> children_;
  AXNodeData data_;
};


}  

#endif  
