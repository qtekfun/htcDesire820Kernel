// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_ACCESSIBILITY_AX_TREE_SOURCE_H_
#define UI_ACCESSIBILITY_AX_TREE_SOURCE_H_

#include "ui/accessibility/ax_node_data.h"

namespace ui {

template<class AXNodeSource>
class AX_EXPORT AXTreeSource {
 public:
  virtual ~AXTreeSource() {}
  virtual AXNodeSource* GetRoot() const = 0;
  virtual AXNodeSource* GetFromId(int32 id) const = 0;
  virtual int32 GetId(const AXNodeSource* node) const = 0;
  virtual int GetChildCount(const AXNodeSource* node) const = 0;
  virtual AXNodeSource* GetChildAtIndex(const AXNodeSource* node, int index)
      const = 0;
  virtual AXNodeSource* GetParent(const AXNodeSource* node) const = 0;

  
  virtual void SerializeNode(
      const AXNodeSource* node, AXNodeData* out_data) const = 0;

 protected:
  AXTreeSource() {}
};

}  

#endif  
