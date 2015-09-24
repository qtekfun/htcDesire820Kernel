// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_LOCATION_ICON_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_LOCATION_ICON_VIEW_H_
#pragma once

#include "chrome/browser/ui/views/location_bar/click_handler.h"
#include "views/controls/image_view.h"

class LocationBarView;
namespace views {
class MouseEvent;
}

class LocationIconView : public views::ImageView {
 public:
  explicit LocationIconView(LocationBarView* location_bar);
  virtual ~LocationIconView();

  
  virtual bool OnMousePressed(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const views::MouseEvent& event) OVERRIDE;

  
  void ShowTooltip(bool show);

 private:
  ClickHandler click_handler_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(LocationIconView);
};

#endif  
