// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_LOCATION_BAR_DECORATION_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_LOCATION_BAR_DECORATION_VIEW_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/controls/image_view.h"

class LocationBarDecorationView : public views::ImageView {
 public:
  LocationBarDecorationView();
  virtual ~LocationBarDecorationView();

  
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

 protected:
  
  
  
  virtual bool CanHandleClick() const;

  
  virtual void OnClick() = 0;

 private:
  
  
  bool could_handle_click_;

  DISALLOW_COPY_AND_ASSIGN(LocationBarDecorationView);
};

#endif  
