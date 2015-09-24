// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTEXT_MENU_CONTROLLER_H_
#define UI_VIEWS_CONTEXT_MENU_CONTROLLER_H_

#include "ui/base/ui_base_types.h"
#include "ui/views/views_export.h"

namespace gfx {
class Point;
}

namespace views {
class View;

class VIEWS_EXPORT ContextMenuController {
 public:
  
  
  virtual void ShowContextMenuForView(View* source,
                                      const gfx::Point& point,
                                      ui::MenuSourceType source_type) = 0;

 protected:
  virtual ~ContextMenuController() {}
};

}  

#endif  
