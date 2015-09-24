// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SKIA_EXT_VECTOR_CANVAS_H_
#define SKIA_EXT_VECTOR_CANVAS_H_

#include "base/compiler_specific.h"
#include "skia/ext/platform_canvas.h"

#include "third_party/skia/include/core/SkDevice.h"

namespace skia {

class SK_API VectorCanvas : public PlatformCanvas {
 public:
  
  explicit VectorCanvas(SkBaseDevice* device);
  virtual ~VectorCanvas();

  virtual SkBounder* setBounder(SkBounder* bounder) OVERRIDE;
  virtual SkDrawFilter* setDrawFilter(SkDrawFilter* filter) OVERRIDE;

 private:
  
  bool IsTopDeviceVectorial() const;

  
  VectorCanvas(const VectorCanvas&);
  const VectorCanvas& operator=(const VectorCanvas&);
};

}  

#endif  

