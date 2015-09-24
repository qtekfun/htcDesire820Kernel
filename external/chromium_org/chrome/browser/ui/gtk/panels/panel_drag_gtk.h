// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_PANELS_PANEL_DRAG_GTK_H_
#define CHROME_BROWSER_UI_GTK_PANELS_PANEL_DRAG_GTK_H_

#include <gtk/gtk.h>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/base/gtk/gtk_signal.h"

class Panel;
class PanelDragDelegate;

class PanelDragGtk {
 public:
  explicit PanelDragGtk(Panel* panel);
  ~PanelDragGtk();

  GtkWidget* widget() const { return drag_widget_; }

  
  
  
  
  
  void InitialWindowEdgeMousePress(GdkEventButton* event, GdkCursor* cursor,
                                   GdkWindowEdge& edge);

  
  
  
  
  
  void InitialTitlebarMousePress(GdkEventButton* event,
                                 GtkWidget* titlebar_widget);

 private:
  friend class GtkNativePanelTesting;

  enum DragState {
    NOT_DRAGGING,
    DRAG_CAN_START,  
    DRAG_IN_PROGRESS,  
    DRAG_ENDED_WAITING_FOR_MOUSE_RELEASE
  };

  
  CHROMEGTK_CALLBACK_1(PanelDragGtk, gboolean, OnMouseMoveEvent,
                       GdkEventMotion*);
  CHROMEGTK_CALLBACK_1(PanelDragGtk, gboolean, OnButtonPressEvent,
                       GdkEventButton*);
  CHROMEGTK_CALLBACK_1(PanelDragGtk, gboolean, OnButtonReleaseEvent,
                       GdkEventButton*);
  CHROMEGTK_CALLBACK_1(PanelDragGtk, gboolean, OnKeyPressEvent,
                       GdkEventKey*);
  CHROMEGTK_CALLBACK_1(PanelDragGtk, gboolean, OnKeyReleaseEvent,
                       GdkEventKey*);
  CHROMEGTK_CALLBACK_1(PanelDragGtk, gboolean, OnGrabBrokenEvent,
                       GdkEventGrabBroken*);

  
  
  void AssertCleanState();

  void GrabPointerAndKeyboard(GdkEventButton* event,
                              GdkCursor* cursor);
  void ReleasePointerAndKeyboardGrab();

  
  
  
  
  
  void EndDrag(bool canceled);

  
  Panel* panel_;

  
  GtkWidget* drag_widget_;

  DragState drag_state_;

  
  GdkEvent* initial_mouse_down_;

  
  
  
  GtkWidget* click_handler_;

  
  PanelDragDelegate* drag_delegate_;

  DISALLOW_COPY_AND_ASSIGN(PanelDragGtk);
};

#endif  
