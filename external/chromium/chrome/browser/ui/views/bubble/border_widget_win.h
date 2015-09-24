// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_BUBBLE_BORDER_WIDGET_WIN_H_
#define CHROME_BROWSER_UI_VIEWS_BUBBLE_BORDER_WIDGET_WIN_H_
#pragma once

#include "chrome/browser/ui/views/bubble/bubble_border.h"
#include "views/widget/widget_win.h"

class BorderContents;

class BorderWidgetWin : public views::WidgetWin {
 public:
  BorderWidgetWin();
  virtual ~BorderWidgetWin() { }

  
  void Init(BorderContents* border_contents, HWND owner);

  
  
  
  
  
  
  virtual gfx::Rect SizeAndGetBounds(const gfx::Rect& position_relative_to,
                                     BubbleBorder::ArrowLocation arrow_location,
                                     const gfx::Size& contents_size);

  
  BorderContents* border_contents() { return border_contents_; }

 protected:
  BorderContents* border_contents_;

 private:
  
  virtual LRESULT OnMouseActivate(UINT message,
                                  WPARAM w_param,
                                  LPARAM l_param) OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(BorderWidgetWin);
};

#endif  
