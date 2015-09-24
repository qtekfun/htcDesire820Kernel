// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_TABLE_TABLE_VIEW_OBSERVER_H_
#define UI_VIEWS_CONTROLS_TABLE_TABLE_VIEW_OBSERVER_H_

#include "ui/events/keycodes/keyboard_codes.h"

namespace views {

class TableView;
class TableView2;

class TableViewObserver {
 public:
  virtual ~TableViewObserver() {}

  
  virtual void OnSelectionChanged() = 0;

  
  virtual void OnDoubleClick() {}

  
  virtual void OnMiddleClick() {}

  
  virtual void OnKeyDown(ui::KeyboardCode virtual_keycode) {}
};

}  

#endif  
