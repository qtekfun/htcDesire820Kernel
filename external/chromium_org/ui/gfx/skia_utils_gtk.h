// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_SKIA_UTILS_GTK_H_
#define UI_GFX_SKIA_UTILS_GTK_H_

#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/gfx_export.h"

typedef struct _GdkColor GdkColor;

namespace gfx {

GFX_EXPORT SkColor GdkColorToSkColor(GdkColor color);

GFX_EXPORT GdkColor SkColorToGdkColor(SkColor color);

}  

#endif  
