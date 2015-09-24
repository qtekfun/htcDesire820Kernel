// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_BADGE_UTIL_H_
#define CHROME_COMMON_BADGE_UTIL_H_
#pragma once

#include "base/string16.h"
#include "third_party/skia/include/core/SkBitmap.h"

class SkPaint;

namespace badge_util {

SkPaint* GetBadgeTextPaintSingleton();

SkBitmap DrawBadgeIconOverlay(const SkBitmap& icon,
                              float font_size_in_pixels,
                              const string16& text,
                              const string16& fallback);

}  

#endif  
