// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_CLIENT_DRAG_DROP_DELEGATE_H_
#define UI_AURA_CLIENT_DRAG_DROP_DELEGATE_H_

#include "ui/aura/aura_export.h"

namespace ui {
class DropTargetEvent;
}

namespace aura {
class Window;
namespace client {

class AURA_EXPORT DragDropDelegate {
 public:
  
  
  
  virtual void OnDragEntered(const ui::DropTargetEvent& event) = 0;

  
  
  
  
  virtual int OnDragUpdated(const ui::DropTargetEvent& event) = 0;

  
  
  virtual void OnDragExited() = 0;

  
  
  virtual int OnPerformDrop(const ui::DropTargetEvent& event) = 0;

 protected:
  virtual ~DragDropDelegate() {}
};

AURA_EXPORT void SetDragDropDelegate(Window* window,
                                     DragDropDelegate* delegate);
AURA_EXPORT DragDropDelegate* GetDragDropDelegate(Window* window);

}  
}  

#endif  
