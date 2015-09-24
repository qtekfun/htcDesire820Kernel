// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_THEME_BACKGROUND_H_
#define CHROME_BROWSER_UI_VIEWS_THEME_BACKGROUND_H_
#pragma once

#include "views/background.h"

class BrowserView;

namespace views {
class View;
};

class ThemeBackground : public views::Background {
 public:
  explicit ThemeBackground(BrowserView* browser);
  virtual ~ThemeBackground() {}

  
  virtual void Paint(gfx::Canvas* canvas, views::View* view) const;

 private:
  BrowserView* browser_view_;

  DISALLOW_COPY_AND_ASSIGN(ThemeBackground);
};

#endif  
