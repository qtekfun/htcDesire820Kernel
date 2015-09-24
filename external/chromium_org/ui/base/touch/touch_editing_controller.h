// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_TOUCH_TOUCH_EDITING_CONTROLLER_H_
#define UI_BASE_TOUCH_TOUCH_EDITING_CONTROLLER_H_

#include "ui/base/models/simple_menu_model.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"

namespace ui {

class UI_EXPORT TouchEditable : public ui::SimpleMenuModel::Delegate {
 public:
  
  
  
  virtual void SelectRect(const gfx::Point& start, const gfx::Point& end) = 0;

  
  virtual void MoveCaretTo(const gfx::Point& point) = 0;

  
  
  
  
  
  
  
  
  
  virtual void GetSelectionEndPoints(gfx::Rect* p1, gfx::Rect* p2) = 0;

  
  virtual gfx::Rect GetBounds() = 0;

  
  virtual gfx::NativeView GetNativeView() = 0;

  
  virtual void ConvertPointToScreen(gfx::Point* point) = 0;
  virtual void ConvertPointFromScreen(gfx::Point* point) = 0;

  
  
  virtual bool DrawsHandles() = 0;

  
  virtual void OpenContextMenu(const gfx::Point& anchor) = 0;

 protected:
  virtual ~TouchEditable() {}
};

class UI_EXPORT TouchSelectionController {
 public:
  virtual ~TouchSelectionController() {}

  
  static TouchSelectionController* create(
      TouchEditable* client_view);

  
  virtual void SelectionChanged() = 0;

  
  virtual bool IsHandleDragInProgress() = 0;
};

class UI_EXPORT TouchSelectionControllerFactory {
 public:
  static void SetInstance(TouchSelectionControllerFactory* instance);

  virtual TouchSelectionController* create(TouchEditable* client_view) = 0;

 protected:
  virtual ~TouchSelectionControllerFactory() {}
};

}  

#endif  
