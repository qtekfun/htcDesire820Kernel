// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_PANELS_PANEL_SCROLLER_CONTAINER_H_
#define CHROME_BROWSER_CHROMEOS_PANELS_PANEL_SCROLLER_CONTAINER_H_
#pragma once

#include "base/basictypes.h"
#include "views/view.h"

class PanelScroller;

class PanelScrollerContainer : public views::View {
 public:
  PanelScrollerContainer(PanelScroller* scroller, views::View* contents);
  virtual ~PanelScrollerContainer();

  int HeaderSize() const;

  
  virtual gfx::Size GetPreferredSize();
  virtual void Layout();
  virtual void OnPaint(gfx::Canvas* canvas);

 private:
  
  PanelScroller* scroller_;

  views::View* contents_;

  DISALLOW_COPY_AND_ASSIGN(PanelScrollerContainer);
};

#endif  
