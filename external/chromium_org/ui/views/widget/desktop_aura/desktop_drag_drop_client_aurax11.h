// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_DRAG_DROP_CLIENT_AURAX11_H_
#define UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_DRAG_DROP_CLIENT_AURAX11_H_

#include <set>
#include <X11/Xlib.h>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "ui/aura/client/drag_drop_client.h"
#include "ui/aura/window_observer.h"
#include "ui/base/cursor/cursor.h"
#include "ui/gfx/point.h"
#include "ui/gfx/x/x11_atom_cache.h"
#include "ui/views/views_export.h"
#include "ui/views/widget/desktop_aura/x11_whole_screen_move_loop.h"
#include "ui/views/widget/desktop_aura/x11_whole_screen_move_loop_delegate.h"

namespace aura {
namespace client {
class DragDropDelegate;
}
}

namespace gfx {
class Point;
}

namespace ui {
class DragSource;
class DropTargetEvent;
class OSExchangeData;
class OSExchangeDataProviderAuraX11;
class SelectionFormatMap;
}

namespace views {
class DesktopNativeCursorManager;

class VIEWS_EXPORT DesktopDragDropClientAuraX11
    : public aura::client::DragDropClient,
      public aura::WindowObserver,
      public X11WholeScreenMoveLoopDelegate {
 public:
  DesktopDragDropClientAuraX11(
      aura::Window* root_window,
      views::DesktopNativeCursorManager* cursor_manager,
      Display* xdisplay,
      ::Window xwindow);
  virtual ~DesktopDragDropClientAuraX11();

  
  
  
  static DesktopDragDropClientAuraX11* GetForWindow(::Window window);

  
  void OnXdndEnter(const XClientMessageEvent& event);
  void OnXdndLeave(const XClientMessageEvent& event);
  void OnXdndPosition(const XClientMessageEvent& event);
  void OnXdndStatus(const XClientMessageEvent& event);
  void OnXdndFinished(const XClientMessageEvent& event);
  void OnXdndDrop(const XClientMessageEvent& event);

  
  void OnSelectionNotify(const XSelectionEvent& xselection);

  
  virtual int StartDragAndDrop(
      const ui::OSExchangeData& data,
      aura::Window* root_window,
      aura::Window* source_window,
      const gfx::Point& root_location,
      int operation,
      ui::DragDropTypes::DragEventSource source) OVERRIDE;
  virtual void DragUpdate(aura::Window* target,
                          const ui::LocatedEvent& event) OVERRIDE;
  virtual void Drop(aura::Window* target,
                    const ui::LocatedEvent& event) OVERRIDE;
  virtual void DragCancel() OVERRIDE;
  virtual bool IsDragDropInProgress() OVERRIDE;

  
  virtual void OnWindowDestroyed(aura::Window* window) OVERRIDE;

  
  virtual void OnMouseMovement(XMotionEvent* event) OVERRIDE;
  virtual void OnMouseReleased() OVERRIDE;
  virtual void OnMoveLoopEnded() OVERRIDE;

 private:
  typedef std::map< ::Window, std::pair<gfx::Point, unsigned long> >
      NextPositionMap;

  
  
  
  void DragTranslate(const gfx::Point& root_window_location,
                     scoped_ptr<ui::OSExchangeData>* data,
                     scoped_ptr<ui::DropTargetEvent>* event,
                     aura::client::DragDropDelegate** delegate);

  
  
  void NotifyDragLeave();

  
  
  ::Atom DragOperationToAtom(int drag_operation);

  
  int AtomToDragOperation(::Atom atom);

  
  
  
  std::vector< ::Atom> GetOfferedDragOperations();

  
  
  
  ui::SelectionFormatMap GetFormatMap() const;

  
  
  
  void CompleteXdndPosition(::Window source_window,
                            const gfx::Point& screen_point);

  void SendXdndEnter(::Window dest_window);
  void SendXdndLeave(::Window dest_window);
  void SendXdndPosition(::Window dest_window,
                        const gfx::Point& screen_point,
                        unsigned long time);
  void SendXdndDrop(::Window dest_window);

  
  
  void SendXClientEvent(::Window xid, XEvent* xev);

  
  
  X11WholeScreenMoveLoop move_loop_;

  aura::Window* root_window_;

  Display* xdisplay_;
  ::Window xwindow_;

  ui::X11AtomCache atom_cache_;

  
  class X11DragContext;
  scoped_ptr<X11DragContext> target_current_context_;

  
  
  
  
  
  aura::Window* target_window_;

  
  
  
  gfx::Point target_window_location_;
  gfx::Point target_window_root_location_;

  
  
  std::set< ::Window> waiting_on_status_;

  
  
  NextPositionMap next_position_message_;

  
  ui::OSExchangeDataProviderAuraX11 const* source_provider_;
  ::Window source_current_window_;

  bool drag_drop_in_progress_;

  
  int drag_operation_;

  
  
  
  int resulting_operation_;

  
  
  std::set< ::Window> pending_drop_;

  
  
  
  
  
  
  
  
  
  std::map< ::Window, ::Atom> negotiated_operation_;

  
  gfx::NativeCursor grab_cursor_;
  gfx::NativeCursor copy_grab_cursor_;
  gfx::NativeCursor move_grab_cursor_;

  DISALLOW_COPY_AND_ASSIGN(DesktopDragDropClientAuraX11);
};

}  

#endif  
