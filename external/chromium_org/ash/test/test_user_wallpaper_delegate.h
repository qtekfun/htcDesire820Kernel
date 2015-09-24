// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_TEST_TEST_USER_WALLPAPER_DELEGATE_H_
#define ASH_TEST_TEST_USER_WALLPAPER_DELEGATE_H_

#include "ash/default_user_wallpaper_delegate.h"

#include "ui/gfx/image/image_skia.h"

namespace ash {
namespace test {

class TestUserWallpaperDelegate : public DefaultUserWallpaperDelegate {
 public:
  TestUserWallpaperDelegate();
  virtual ~TestUserWallpaperDelegate();

  void set_custom_wallpaper(const gfx::ImageSkia& wallpaper) {
    custom_wallpaper_ = wallpaper;
  }

  
  virtual void UpdateWallpaper() OVERRIDE;

  
  int GetUpdateWallpaperCountAndReset();

 private:
  
  int update_wallpaper_count_;

  
  gfx::ImageSkia custom_wallpaper_;

  DISALLOW_COPY_AND_ASSIGN(TestUserWallpaperDelegate);
};

}  
}  

#endif  
