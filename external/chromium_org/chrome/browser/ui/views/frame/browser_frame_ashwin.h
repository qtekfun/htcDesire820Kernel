// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_FRAME_ASHWIN_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_FRAME_ASHWIN_H_

#include "chrome/browser/ui/views/frame/browser_frame_ash.h"

class BrowserFrameAshWin : public BrowserFrameAsh {
 public:
  BrowserFrameAshWin(BrowserFrame* browser_frame, BrowserView* browser_view);

 protected:
  virtual ~BrowserFrameAshWin();

  
  virtual void OnWindowFocused(aura::Window* gained_focus,
                               aura::Window* lost_focus) OVERRIDE;
 private:
  DISALLOW_COPY_AND_ASSIGN(BrowserFrameAshWin);
};

#endif  
