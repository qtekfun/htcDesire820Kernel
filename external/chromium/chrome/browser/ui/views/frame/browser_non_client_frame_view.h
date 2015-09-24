// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_NON_CLIENT_FRAME_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_NON_CLIENT_FRAME_VIEW_H_
#pragma once

#include "views/window/non_client_view.h"

class BrowserFrame;
class BrowserView;

class BrowserNonClientFrameView : public views::NonClientFrameView {
 public:
  BrowserNonClientFrameView() : NonClientFrameView() {}
  virtual ~BrowserNonClientFrameView() {}

  
  virtual gfx::Rect GetBoundsForTabStrip(views::View* tabstrip) const = 0;

  
  
  
  
  virtual int GetHorizontalTabStripVerticalOffset(bool restored) const = 0;

  
  virtual void UpdateThrobber(bool running) = 0;
};

namespace browser {

BrowserNonClientFrameView* CreateBrowserNonClientFrameView(
    BrowserFrame* frame, BrowserView* browser_view);

}  

#endif  
