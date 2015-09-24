// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_PROFILE_TAG_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_PROFILE_TAG_VIEW_H_
#pragma once

#include "third_party/skia/include/core/SkBitmap.h"
#include "views/view.h"

class BrowserFrame;

namespace gfx {
class Canvas;
}

namespace views {

class ProfileMenuButton;


class ProfileTagView : public View {
 public:
  
  static const int kProfileTagHeight = 20;

  ProfileTagView(BrowserFrame* frame,
                 views::ProfileMenuButton* profile_menu_button);
  ~ProfileTagView() {}

  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

 private:
  
  void CreateProfileTagBitmaps();

  
  bool profile_tag_bitmaps_created_;

  
  SkBitmap active_profile_tag_center_;
  SkBitmap active_profile_tag_left_;
  SkBitmap active_profile_tag_right_;
  SkBitmap inactive_profile_tag_center_;
  SkBitmap inactive_profile_tag_left_;
  SkBitmap inactive_profile_tag_right_;

  
  BrowserFrame* frame_;

  
  views::ProfileMenuButton* profile_menu_button_;

  DISALLOW_COPY_AND_ASSIGN(ProfileTagView);
};

}  

#endif  

