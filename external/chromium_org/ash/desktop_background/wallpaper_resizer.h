// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_DESKTOP_BACKGROUND_DESKTOP_WALLPAPER_RESIZER_H_
#define ASH_DESKTOP_BACKGROUND_DESKTOP_WALLPAPER_RESIZER_H_

#include "ash/ash_export.h"
#include "ash/desktop_background/desktop_background_controller.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "skia/ext/image_operations.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/size.h"

namespace ash {

class WallpaperResizerObserver;

class ASH_EXPORT WallpaperResizer {
 public:
  
  
  
  static uint32_t GetImageId(const gfx::ImageSkia& image);

  WallpaperResizer(int image_resource_id,
                   const gfx::Size& target_size,
                   WallpaperLayout layout);

  WallpaperResizer(const gfx::ImageSkia& image,
                   const gfx::Size& target_size,
                   WallpaperLayout layout);

  ~WallpaperResizer();

  const gfx::ImageSkia& image() const { return image_; }
  uint32_t original_image_id() const { return original_image_id_; }
  WallpaperLayout layout() const { return layout_; }

  
  
  void StartResize();

  
  void AddObserver(WallpaperResizerObserver* observer);
  void RemoveObserver(WallpaperResizerObserver* observer);

 private:
  
  
  void OnResizeFinished(SkBitmap* resized_bitmap);

  ObserverList<WallpaperResizerObserver> observers_;

  
  
  
  gfx::ImageSkia image_;

  
  uint32_t original_image_id_;

  gfx::Size target_size_;

  WallpaperLayout layout_;

  base::WeakPtrFactory<WallpaperResizer> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(WallpaperResizer);
};

}  


#endif  
