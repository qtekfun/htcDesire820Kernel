// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_SETTING_LEVEL_BUBBLE_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_SETTING_LEVEL_BUBBLE_VIEW_H_
#pragma once

#include "views/view.h"

namespace views {
class ProgressBar;
}  

class SkBitmap;

namespace chromeos {

class SettingLevelBubbleView : public views::View {
 public:
  SettingLevelBubbleView();

  
  
  
  void Init(SkBitmap* icon, int level_percent);

  
  void SetIcon(SkBitmap* icon);

  
  void Update(int level_percent);

  
  virtual void OnPaint(gfx::Canvas* canvas);
  virtual void Layout();
  virtual gfx::Size GetPreferredSize();

 private:
  views::ProgressBar* progress_bar_;
  SkBitmap* icon_;  

  DISALLOW_COPY_AND_ASSIGN(SettingLevelBubbleView);
};

}  

#endif  
