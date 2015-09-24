// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_FRAME_PANEL_CONTROLLER_H_
#define CHROME_BROWSER_CHROMEOS_FRAME_PANEL_CONTROLLER_H_
#pragma once

#include <gtk/gtk.h>

#include "third_party/cros/chromeos_wm_ipc_enums.h"
#include "ui/base/x/x11_util.h"
#include "views/controls/button/button.h"

class BrowserView;
class SkBitmap;
typedef unsigned long XID;

namespace base {
class Time;
}

namespace views {
class ImageButton;
class ImageView;
class Label;
class MouseEvent;
class Widget;
}

namespace chromeos {

class PanelController {
 public:
  enum State {
    INITIAL,
    EXPANDED,
    MINIMIZED,
  };

  
  class Delegate {
   public:
    
    virtual string16 GetPanelTitle() = 0;

    
    virtual SkBitmap GetPanelIcon() = 0;

    
    
    virtual bool CanClosePanel() = 0;

    
    virtual void ClosePanel() = 0;

    
    virtual void ActivatePanel() = 0;
  };

  PanelController(Delegate* delegate_window,
                  GtkWindow* window);
  virtual ~PanelController() {}

  
  
  void Init(bool initial_focus, const gfx::Rect& init_bounds, XID creator_xid,
            WmIpcPanelUserResizeType resize_type);

  bool TitleMousePressed(const views::MouseEvent& event);
  void TitleMouseReleased(const views::MouseEvent& event);
  void TitleMouseCaptureLost();
  bool TitleMouseDragged(const views::MouseEvent& event);
  bool PanelClientEvent(GdkEventClient* event);
  void OnFocusIn();
  void OnFocusOut();

  void UpdateTitleBar();
  void SetUrgent(bool urgent);
  void Close();

  void SetState(State state);

  bool urgent() { return urgent_; }

 private:
  class TitleContentView : public views::View,
                           public views::ButtonListener {
   public:
    explicit TitleContentView(PanelController* panelController);
    virtual ~TitleContentView();

    
    virtual void Layout() OVERRIDE;
    virtual bool OnMousePressed(const views::MouseEvent& event) OVERRIDE;
    virtual void OnMouseReleased(const views::MouseEvent& event) OVERRIDE;
    virtual void OnMouseCaptureLost() OVERRIDE;
    virtual bool OnMouseDragged(const views::MouseEvent& event) OVERRIDE;

    void OnFocusIn();
    void OnFocusOut();
    void OnClose();

    views::ImageView* title_icon() { return title_icon_; }
    views::Label* title_label() { return title_label_; }
    views::ImageButton* close_button() { return close_button_; }

    
    virtual void ButtonPressed(views::Button* sender,
                               const views::Event& event) OVERRIDE;
   private:
    views::ImageView* title_icon_;
    views::Label* title_label_;
    views::ImageButton* close_button_;
    PanelController* panel_controller_;
    DISALLOW_COPY_AND_ASSIGN(TitleContentView);
  };

  
  void OnCloseButtonPressed();

  
  static bool OnPanelClientEvent(
      GtkWidget* widget,
      GdkEventClient* event,
      PanelController* panel_controller);

  
  Delegate* delegate_;

  
  GtkWindow* panel_;
  
  XID panel_xid_;

  
  views::Widget* title_window_;
  
  GtkWidget* title_;
  
  XID title_xid_;

  
  TitleContentView* title_content_;

  
  bool expanded_;

  
  bool mouse_down_;

  
  int mouse_down_abs_x_;
  int mouse_down_abs_y_;

  
  
  int mouse_down_offset_x_;
  int mouse_down_offset_y_;

  
  
  bool dragging_;

  
  int client_event_handler_id_;

  
  bool focused_;

  
  bool urgent_;

  
  base::TimeTicks urgent_cleared_time_;

  DISALLOW_COPY_AND_ASSIGN(PanelController);
};

}  

#endif  
