// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_FONT_RENDER_PARAMS_LINUX_H_
#define UI_GFX_FONT_RENDER_PARAMS_LINUX_H_

#include "ui/gfx/gfx_export.h"

namespace gfx {

struct GFX_EXPORT FontRenderParams {
  

  
  enum Hinting {
    HINTING_NONE = 0,
    HINTING_SLIGHT,
    HINTING_MEDIUM,
    HINTING_FULL,
  };

  
  enum SubpixelRendering {
    SUBPIXEL_RENDERING_NONE = 0,
    SUBPIXEL_RENDERING_RGB,
    SUBPIXEL_RENDERING_BGR,
    SUBPIXEL_RENDERING_VRGB,
    SUBPIXEL_RENDERING_VBGR,
  };

  
  
  bool antialiasing;

  
  
  bool subpixel_positioning;

  
  
  bool autohinter;

  
  bool use_bitmaps;

  
  Hinting hinting;

  
  
  SubpixelRendering subpixel_rendering;
};

GFX_EXPORT const FontRenderParams& GetDefaultFontRenderParams();

GFX_EXPORT const FontRenderParams& GetDefaultWebKitFontRenderParams();

GFX_EXPORT bool GetDefaultWebkitSubpixelPositioning();

}  

#endif  
