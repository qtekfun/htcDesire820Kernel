// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_ASH_CONSTANTS_H_
#define ASH_ASH_CONSTANTS_H_

#include "ash/ash_export.h"
#include "ui/aura/window.h"
#include "ui/base/ui_base_types.h"

typedef unsigned int SkColor;

namespace ash {

ASH_EXPORT extern const int kResizeAreaCornerSize;

ASH_EXPORT extern const int kResizeOutsideBoundsSize;
ASH_EXPORT extern const int kResizeOutsideBoundsScaleForTouch;
ASH_EXPORT extern const int kResizeInsideBoundsSize;

#if defined(OS_CHROMEOS)
extern const SkColor kChromeOsBootColor;
#endif

extern const SkColor kFocusBorderColor;

} 

#endif  
