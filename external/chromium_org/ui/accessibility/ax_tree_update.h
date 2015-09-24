// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_ACCESSIBILITY_AX_TREE_UPDATE_H_
#define UI_ACCESSIBILITY_AX_TREE_UPDATE_H_

#include <vector>

#include "ui/accessibility/ax_node_data.h"

namespace ui {

struct AX_EXPORT AXTreeUpdate {
  AXTreeUpdate();
  ~AXTreeUpdate();

  
  
  
  
  
  int node_id_to_clear;

  
  std::vector<AXNodeData> nodes;

  
};

}  

#endif  
