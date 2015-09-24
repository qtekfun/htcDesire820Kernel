// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_USERNAME_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_USERNAME_VIEW_H_
#pragma once

#include <string>

#include "views/controls/label.h"
#include "third_party/skia/include/core/SkBitmap.h"

namespace gfx {
  class Rect;
}

namespace chromeos {
class UsernameView : public views::Label {
 public:
  virtual ~UsernameView() {}

  
  
  
  
  
  static UsernameView* CreateShapedUsernameView(const std::wstring& username,
                                                bool use_small_shape);

 protected:
  
  
  
  UsernameView(const std::wstring& username, bool use_small_shape);

  
  virtual void OnPaint(gfx::Canvas* canvas);

  
  
  bool use_small_shape() const { return use_small_shape_; }

 private:
  
  virtual gfx::NativeCursor GetCursorForPoint(
      ui::EventType event_type,
      const gfx::Point& p);
  virtual void OnLocaleChanged();

  
  void PaintUsername(const gfx::Rect& bounds);

  
  scoped_ptr<SkBitmap> text_image_;

  
  int margin_width_;

  
  bool use_small_shape_;

  
  bool is_guest_;

  DISALLOW_COPY_AND_ASSIGN(UsernameView);
};

}  

#endif  
