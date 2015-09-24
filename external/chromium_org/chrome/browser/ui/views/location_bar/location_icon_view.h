// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_LOCATION_ICON_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_LOCATION_ICON_VIEW_H_

#include "chrome/browser/ui/views/location_bar/page_info_helper.h"
#include "ui/views/controls/image_view.h"

class LocationBarView;

class LocationIconView : public views::ImageView {
 public:
  explicit LocationIconView(LocationBarView* location_bar);
  virtual ~LocationIconView();

  
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;

  
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  
  void ShowTooltip(bool show);

  PageInfoHelper* page_info_helper() { return &page_info_helper_; }

 private:
  PageInfoHelper page_info_helper_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(LocationIconView);
};

#endif  
