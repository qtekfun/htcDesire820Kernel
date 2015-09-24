// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_DRAG_CONTROLLER_H_
#define UI_VIEWS_DRAG_CONTROLLER_H_

#include "ui/views/views_export.h"

namespace gfx {
class Point;
}

namespace ui {
class OSExchangeData;
}

namespace views {
class View;

class VIEWS_EXPORT DragController {
 public:
  
  virtual void WriteDragDataForView(View* sender,
                                    const gfx::Point& press_pt,
                                    ui::OSExchangeData* data) = 0;

  
  
  virtual int GetDragOperationsForView(View* sender,
                                       const gfx::Point& p) = 0;

  
  
  
  virtual bool CanStartDragForView(View* sender,
                                   const gfx::Point& press_pt,
                                   const gfx::Point& p) = 0;

 protected:
  virtual ~DragController() {}
};

}  

#endif  
