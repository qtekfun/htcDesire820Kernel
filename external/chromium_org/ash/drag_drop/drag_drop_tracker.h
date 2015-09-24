// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_DRAG_DROP_DRAG_DROP_TRACKER_H_
#define ASH_DRAG_DROP_DRAG_DROP_TRACKER_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/events/event.h"

namespace aura {
class Window;
class WindowDelegate;
}

namespace ash {
namespace internal {

class ASH_EXPORT DragDropTracker {
 public:
  DragDropTracker(aura::Window* context_root,
                  aura::WindowDelegate* delegate);
  ~DragDropTracker();

  aura::Window* capture_window() { return capture_window_.get(); }

  
  
  
  void TakeCapture();

  
  
  aura::Window* GetTarget(const ui::LocatedEvent& event);

  
  
  
  ui::LocatedEvent* ConvertEvent(aura::Window* target,
                                 const ui::LocatedEvent& event);

 private:
  
  scoped_ptr<aura::Window> capture_window_;

  DISALLOW_COPY_AND_ASSIGN(DragDropTracker);
};

}  
}  

#endif  
