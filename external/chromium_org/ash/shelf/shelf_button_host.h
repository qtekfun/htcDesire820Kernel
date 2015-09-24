// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELF_SHELF_BUTTON_HOST_H_
#define ASH_SHELF_SHELF_BUTTON_HOST_H_

#include "ash/ash_export.h"
#include "base/strings/string16.h"

namespace ui {
class LocatedEvent;
}

namespace views {
class View;
}

namespace ash {
namespace internal {

class ASH_EXPORT ShelfButtonHost {
 public:
  enum Pointer {
    NONE,
    DRAG_AND_DROP,
    MOUSE,
    TOUCH,
  };

  
  virtual void PointerPressedOnButton(views::View* view,
                                      Pointer pointer,
                                      const ui::LocatedEvent& event) = 0;

  
  virtual void PointerDraggedOnButton(views::View* view,
                                      Pointer pointer,
                                      const ui::LocatedEvent& event) = 0;

  
  virtual void PointerReleasedOnButton(views::View* view,
                                       Pointer pointer,
                                       bool canceled) = 0;

  
  virtual void MouseMovedOverButton(views::View* view) = 0;

  
  virtual void MouseEnteredButton(views::View* view) = 0;

  
  virtual void MouseExitedButton(views::View* view) = 0;

  
  virtual base::string16 GetAccessibleName(const views::View* view) = 0;

 protected:
  virtual ~ShelfButtonHost() {}
};

}  
}  

#endif  
