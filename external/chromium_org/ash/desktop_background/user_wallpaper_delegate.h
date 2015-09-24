// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_DESKTOP_BACKGROUND_USER_WALLPAPER_DELEGATE_H_
#define ASH_DESKTOP_BACKGROUND_USER_WALLPAPER_DELEGATE_H_

#include "ash/ash_export.h"
#include "ui/views/corewm/window_animations.h"

namespace ash {

class ASH_EXPORT UserWallpaperDelegate {
 public:
  virtual ~UserWallpaperDelegate() {}

  
  
  virtual int GetAnimationType() = 0;

  
  
  
  virtual bool ShouldShowInitialAnimation() = 0;

  
  
  virtual void UpdateWallpaper() = 0;

  
  virtual void InitializeWallpaper() = 0;

  
  virtual void OpenSetWallpaperPage() = 0;

  
  
  virtual bool CanOpenSetWallpaperPage() = 0;

  
  virtual void OnWallpaperAnimationFinished() = 0;

  
  virtual void OnWallpaperBootAnimationFinished() = 0;
};

}  

#endif  
