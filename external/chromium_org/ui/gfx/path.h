// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_PATH_H_
#define UI_GFX_PATH_H_

#include "base/basictypes.h"
#include "third_party/skia/include/core/SkPath.h"
#include "ui/gfx/gfx_export.h"
#include "ui/gfx/native_widget_types.h"

namespace gfx {

class GFX_EXPORT Path : public SkPath {
 public:
  
  struct Point {
    int x;
    int y;
  };
  struct PointF {
    float x;
    float y;
  };

  Path();

  
  Path(const Point* points, size_t count);
  Path(const PointF* points, size_t count);

  ~Path();

#if defined(USE_AURA) || defined(OS_WIN) || defined(USE_X11)
  
  
  NativeRegion CreateNativeRegion() const;

  
  
  static gfx::NativeRegion IntersectRegions(gfx::NativeRegion r1,
                                            gfx::NativeRegion r2);

  
  static gfx::NativeRegion CombineRegions(gfx::NativeRegion r1,
                                          gfx::NativeRegion r2);

  
  
  static gfx::NativeRegion SubtractRegion(gfx::NativeRegion r1,
                                          gfx::NativeRegion r2);
#endif

 private:
  DISALLOW_COPY_AND_ASSIGN(Path);
};

}

#endif  
