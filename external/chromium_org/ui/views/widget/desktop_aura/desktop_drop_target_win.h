// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTIOP_DROP_TARGET_WIN_H_
#define UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTIOP_DROP_TARGET_WIN_H_

#include "base/memory/scoped_ptr.h"
#include "ui/aura/window_observer.h"
#include "ui/base/dragdrop/drop_target_win.h"

namespace aura {
namespace client {
class DragDropDelegate;
}
}

namespace ui {
class DropTargetEvent;
class OSExchangeData;
}

namespace views {

class DesktopDropTargetWin : public ui::DropTargetWin,
                             public aura::WindowObserver {
 public:
  DesktopDropTargetWin(aura::Window* root_window, HWND window);
  virtual ~DesktopDropTargetWin();

 private:
  
  virtual DWORD OnDragEnter(IDataObject* data_object,
                            DWORD key_state,
                            POINT position,
                            DWORD effect) OVERRIDE;
  virtual DWORD OnDragOver(IDataObject* data_object,
                           DWORD key_state,
                           POINT position,
                           DWORD effect) OVERRIDE;
  virtual void OnDragLeave(IDataObject* data_object) OVERRIDE;
  virtual DWORD OnDrop(IDataObject* data_object,
                       DWORD key_state,
                       POINT position,
                       DWORD effect) OVERRIDE;

  
  virtual void OnWindowDestroyed(aura::Window* window) OVERRIDE;

  
  
  void Translate(IDataObject* data_object,
                 DWORD key_state,
                 POINT cursor_position,
                 DWORD effect,
                 scoped_ptr<ui::OSExchangeData>* data,
                 scoped_ptr<ui::DropTargetEvent>* event,
                 aura::client::DragDropDelegate** delegate);

  void NotifyDragLeave();

  
  aura::Window* root_window_;

  
  
  
  
  
  aura::Window* target_window_;

  DISALLOW_COPY_AND_ASSIGN(DesktopDropTargetWin);
};

}  

#endif  
