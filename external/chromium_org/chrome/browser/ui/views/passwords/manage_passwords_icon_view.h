// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_PASSWORDS_MANAGE_PASSWORDS_ICON_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_PASSWORDS_MANAGE_PASSWORDS_ICON_VIEW_H_

#include "chrome/browser/ui/passwords/manage_passwords_bubble_model.h"
#include "chrome/browser/ui/views/location_bar/location_bar_view.h"
#include "ui/views/controls/image_view.h"

class ManagePasswordsBubbleUIController;

class ManagePasswordsIconView : public views::ImageView {
 public:
  
  
  
  
  
  explicit ManagePasswordsIconView(
      LocationBarView::Delegate* location_bar_delegate);
  virtual ~ManagePasswordsIconView();

  
  
  void Update(
      ManagePasswordsBubbleUIController* manage_passwords_bubble_ui_controller);

  
  void ShowBubbleIfNeeded(
      ManagePasswordsBubbleUIController* manage_passwords_bubble_ui_controller);

  void SetTooltip(bool password_to_be_saved);

 private:
  
  virtual bool GetTooltipText(const gfx::Point& p,
                              base::string16* tooltip) const OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;

  
  LocationBarView::Delegate* location_bar_delegate_;

  DISALLOW_COPY_AND_ASSIGN(ManagePasswordsIconView);
};

#endif  
