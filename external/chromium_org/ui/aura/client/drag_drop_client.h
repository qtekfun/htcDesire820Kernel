// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_CLIENT_DRAG_DROP_CLIENT_H_
#define UI_AURA_CLIENT_DRAG_DROP_CLIENT_H_

#include "ui/aura/aura_export.h"
#include "ui/base/dragdrop/drag_drop_types.h"
#include "ui/gfx/native_widget_types.h"

namespace gfx {
class Point;
}

namespace ui {
class LocatedEvent;
class OSExchangeData;
}

namespace aura {
class Window;
namespace client {

class AURA_EXPORT DragDropClient {
 public:
  virtual ~DragDropClient() {}

  
  
  
  virtual int StartDragAndDrop(const ui::OSExchangeData& data,
                               aura::Window* root_window,
                               aura::Window* source_window,
                               const gfx::Point& root_location,
                               int operation,
                               ui::DragDropTypes::DragEventSource source) = 0;

  
  virtual void DragUpdate(aura::Window* target,
                          const ui::LocatedEvent& event) = 0;

  
  virtual void Drop(aura::Window* target,
                    const ui::LocatedEvent& event) = 0;

  
  virtual void DragCancel() = 0;

  
  virtual bool IsDragDropInProgress() = 0;
};

AURA_EXPORT void SetDragDropClient(Window* root_window,
                                   DragDropClient* client);
AURA_EXPORT DragDropClient* GetDragDropClient(Window* root_window);

}  
}  

#endif  
