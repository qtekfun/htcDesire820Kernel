// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SKIA_EXT_SKIA_UTILS_WIN_H_
#define SKIA_EXT_SKIA_UTILS_WIN_H_

#include "third_party/skia/include/core/SkColor.h"

struct SkIRect;
struct SkPoint;
struct SkRect;
typedef unsigned long DWORD;
typedef DWORD COLORREF;
typedef struct tagPOINT POINT;
typedef struct tagRECT RECT;

namespace skia {

POINT SkPointToPOINT(const SkPoint& point);

SkRect RECTToSkRect(const RECT& rect);

inline const SkIRect& RECTToSkIRect(const RECT& rect) {
  return reinterpret_cast<const SkIRect&>(rect);
}

inline const RECT& SkIRectToRECT(const SkIRect& rect) {
  return reinterpret_cast<const RECT&>(rect);
}

SK_API SkColor COLORREFToSkColor(COLORREF color);

SK_API COLORREF SkColorToCOLORREF(SkColor color);

}  

#endif  

