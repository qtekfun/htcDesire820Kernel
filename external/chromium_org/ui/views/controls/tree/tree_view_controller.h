// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_TREE_TREE_VIEW_CONTROLLER_H_
#define UI_VIEWS_CONTROLS_TREE_TREE_VIEW_CONTROLLER_H_

#include "ui/events/keycodes/keyboard_codes.h"
#include "ui/views/views_export.h"

namespace ui {
class TreeModelNode;
}

namespace views {

class TreeView;


class VIEWS_EXPORT TreeViewController {
 public:
  
  
  virtual void OnTreeViewSelectionChanged(TreeView* tree_view) = 0;

  
  
  virtual bool CanEdit(TreeView* tree_view, ui::TreeModelNode* node);

 protected:
  virtual ~TreeViewController();
};

}  

#endif  
