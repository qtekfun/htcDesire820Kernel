// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TOOLBAR_BACK_BUTTON_H_
#define CHROME_BROWSER_UI_VIEWS_TOOLBAR_BACK_BUTTON_H_

#include "chrome/browser/ui/views/toolbar/toolbar_button.h"

namespace ui {
class MenuModel;
}

namespace views {
class ButtonListener;
}

class BackButton : public ToolbarButton {
 public:
  
  
  BackButton(views::ButtonListener* listener, ui::MenuModel* model);
  virtual ~BackButton();

  void SetLeadingMargin(int margin);

 protected:
  virtual gfx::Rect GetThemePaintRect() const OVERRIDE;

 private:
  
  
  int margin_leading_;

  DISALLOW_COPY_AND_ASSIGN(BackButton);
};

#endif  
