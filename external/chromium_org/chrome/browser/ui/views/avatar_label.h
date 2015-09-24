// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_AVATAR_LABEL_H_
#define CHROME_BROWSER_UI_VIEWS_AVATAR_LABEL_H_

#include "base/compiler_specific.h"
#include "ui/views/controls/button/text_button.h"

class BrowserView;

namespace ui {
class MouseEvent;
class ThemeProvider;
}

class AvatarLabel : public views::TextButton {
 public:
  explicit AvatarLabel(BrowserView* browser_view);
  virtual ~AvatarLabel();

  
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;

  
  void UpdateLabelStyle();

 private:
  BrowserView* browser_view_;

  DISALLOW_COPY_AND_ASSIGN(AvatarLabel);
};

#endif  
