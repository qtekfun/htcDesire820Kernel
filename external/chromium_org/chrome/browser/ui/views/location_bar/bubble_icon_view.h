// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_BUBBLE_ICON_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_BUBBLE_ICON_VIEW_H_

#include "ui/views/controls/image_view.h"

class CommandUpdater;

class BubbleIconView : public views::ImageView {
 protected:
  enum ExecuteSource {
    EXECUTE_SOURCE_MOUSE,
    EXECUTE_SOURCE_KEYBOARD,
    EXECUTE_SOURCE_GESTURE,
  };

  explicit BubbleIconView(CommandUpdater* command_updater, int command_id);
  virtual ~BubbleIconView();

  
  virtual bool IsBubbleShowing() const = 0;

  
  virtual void OnExecuting(ExecuteSource execute_source) = 0;

  
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual bool GetTooltipText(const gfx::Point& p, base::string16* tooltip)
      const OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;

  
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

 private:
  
  CommandUpdater* command_updater_;

  
  const int command_id_;

  
  
  
  bool suppress_mouse_released_action_;

  DISALLOW_COPY_AND_ASSIGN(BubbleIconView);
};

#endif  
