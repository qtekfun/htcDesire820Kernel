// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_PAGE_ZOOM_H_
#define CONTENT_PUBLIC_COMMON_PAGE_ZOOM_H_

#include "content/common/content_export.h"

namespace content {

enum PageZoom {
  PAGE_ZOOM_OUT   = -1,
  PAGE_ZOOM_RESET = 0,
  PAGE_ZOOM_IN    = 1,
};

CONTENT_EXPORT extern const double kMinimumZoomFactor;

CONTENT_EXPORT extern const double kMaximumZoomFactor;

CONTENT_EXPORT extern const double kEpsilon;

CONTENT_EXPORT bool ZoomValuesEqual(double value_a, double value_b);

CONTENT_EXPORT double ZoomLevelToZoomFactor(double zoom_level);
CONTENT_EXPORT double ZoomFactorToZoomLevel(double factor);

}  

#endif  
