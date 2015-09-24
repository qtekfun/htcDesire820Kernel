// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TABS_ABSTRACT_TAB_STRIP_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_TABS_ABSTRACT_TAB_STRIP_VIEW_H_
#pragma once

#include "views/view.h"

class AbstractTabStripView : public views::View {
 public:
  virtual ~AbstractTabStripView() {}

  
  
  
  virtual bool IsTabStripEditable() const = 0;

  
  
  virtual bool IsTabStripCloseable() const = 0;

  
  
  virtual void UpdateLoadingAnimations() = 0;

  
  
  virtual bool IsPositionInWindowCaption(const gfx::Point& point) = 0;

  
  virtual void SetBackgroundOffset(const gfx::Point& offset) = 0;
};

#endif  

