// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_CLICK_HANDLER_H_
#define CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_CLICK_HANDLER_H_
#pragma once

#include "base/basictypes.h"

class LocationBarView;

namespace views {
class MouseEvent;
class View;
}

class ClickHandler {
 public:
  ClickHandler(const views::View* owner, LocationBarView* location_bar);

  void OnMouseReleased(const views::MouseEvent& event);

 private:
  const views::View* owner_;
  LocationBarView* location_bar_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(ClickHandler);
};

#endif  

