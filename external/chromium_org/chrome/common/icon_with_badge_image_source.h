// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_ICON_WITH_BADGE_IMAGE_SOURCE_H_
#define CHROME_COMMON_ICON_WITH_BADGE_IMAGE_SOURCE_H_

#include <string>

#include "chrome/common/extensions/api/extension_action/action_info.h"
#include "ui/gfx/image/canvas_image_source.h"
#include "ui/gfx/image/image_skia.h"

namespace gfx {
class Size;
}

class IconWithBadgeImageSource
    : public gfx::CanvasImageSource {
 public:
  IconWithBadgeImageSource(
      const gfx::ImageSkia& icon,
      const gfx::Size& icon_size,
      const gfx::Size& spacing,
      const std::string& text,
      const SkColor& text_color,
      const SkColor& background_color,
      extensions::ActionInfo::Type action_type);
  virtual ~IconWithBadgeImageSource();

 private:
  virtual void Draw(gfx::Canvas* canvas) OVERRIDE;

  
  gfx::ImageSkia icon_;

  
  gfx::Size spacing_;

  
  std::string text_;

  
  SkColor text_color_;

  
  SkColor background_color_;

  
  extensions::ActionInfo::Type action_type_;

  DISALLOW_COPY_AND_ASSIGN(IconWithBadgeImageSource);
};

#endif  
