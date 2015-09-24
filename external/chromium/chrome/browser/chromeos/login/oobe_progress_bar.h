// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_OOBE_PROGRESS_BAR_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_OOBE_PROGRESS_BAR_H_
#pragma once

#include <vector>

#include "base/basictypes.h"
#include "ui/gfx/font.h"
#include "views/view.h"

class SkBitmap;

namespace chromeos {

class OobeProgressBar : public views::View {
 public:
  
  
  explicit OobeProgressBar(const std::vector<int>& steps);

  
  virtual void OnPaint(gfx::Canvas* canvas);

  
  
  void SetStep(int step);

 protected:
  
  virtual void OnLocaleChanged();

 private:
  static void InitClass();

  
  static SkBitmap* dot_current_;
  static SkBitmap* dot_empty_;
  static SkBitmap* dot_filled_;
  static SkBitmap* line_;
  static SkBitmap* line_left_;
  static SkBitmap* line_right_;

  gfx::Font font_;

  
  
  std::vector<int> steps_;

  
  size_t progress_;

  DISALLOW_COPY_AND_ASSIGN(OobeProgressBar);
};

}  

#endif  
