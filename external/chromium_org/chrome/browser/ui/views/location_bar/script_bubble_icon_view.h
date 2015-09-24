// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_SCRIPT_BUBBLE_ICON_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_SCRIPT_BUBBLE_ICON_VIEW_H_

#include "chrome/browser/ui/views/location_bar/location_bar_view.h"
#include "ui/views/controls/image_view.h"

class ScriptBubbleIconView : public views::ImageView {
 public:
  explicit ScriptBubbleIconView(
      LocationBarView::Delegate* location_bar_delegate);
  virtual ~ScriptBubbleIconView();

  
  void SetScriptCount(size_t script_count);

 private:
  
  virtual void Layout() OVERRIDE;

  
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;

  
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  
  void ShowScriptBubble(views::View* anchor_view,
                        content::WebContents* web_contents);

  
  LocationBarView::Delegate* location_bar_delegate_;

  
  size_t script_count_;

  DISALLOW_COPY_AND_ASSIGN(ScriptBubbleIconView);
};

#endif  
