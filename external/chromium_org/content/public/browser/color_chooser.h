// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_COLOR_CHOOSER_H_
#define CONTENT_PUBLIC_BROWSER_COLOR_CHOOSER_H_

#include "third_party/skia/include/core/SkColor.h"

namespace content {

class ColorChooser {
 public:
  virtual ~ColorChooser() {}

  
  
  virtual void End() = 0;

  
  virtual void SetSelectedColor(SkColor color) = 0;
};

}  

#endif  
