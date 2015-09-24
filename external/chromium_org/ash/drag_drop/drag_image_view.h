// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_DRAG_DROP_DRAG_IMAGE_VIEW_H_
#define ASH_DRAG_DROP_DRAG_IMAGE_VIEW_H_

#include "ui/base/dragdrop/drag_drop_types.h"
#include "ui/views/controls/image_view.h"

namespace views {
class Widget;
}

namespace ash {
namespace internal {

class DragImageView : public views::ImageView {
 public:
  
  
  
  
  
  DragImageView(gfx::NativeView context,
                ui::DragDropTypes::DragEventSource source);
  virtual ~DragImageView();

  
  
  
  
  void SetBoundsInScreen(const gfx::Rect& bounds);

  
  void SetScreenPosition(const gfx::Point& position);

  
  gfx::Rect GetBoundsInScreen() const;

  
  void SetWidgetVisible(bool visible);

  
  
  
  void SetTouchDragOperationHintOff();

  
  
  void SetTouchDragOperation(int operation);
  void SetTouchDragOperationHintPosition(const gfx::Point& position);

  
  void SetOpacity(float opacity);

 private:
  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

  scoped_ptr<views::Widget> widget_;
  gfx::Size widget_size_;

  ui::DragDropTypes::DragEventSource drag_event_source_;
  int touch_drag_operation_;
  gfx::Point touch_drag_operation_indicator_position_;

  DISALLOW_COPY_AND_ASSIGN(DragImageView);
};

}  
}  

#endif  
