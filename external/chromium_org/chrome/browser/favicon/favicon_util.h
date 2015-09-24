// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_FAVICON_FAVICON_UTIL_H_
#define CHROME_BROWSER_FAVICON_FAVICON_UTIL_H_

#include <vector>

#include "ui/base/layout.h"

namespace chrome {
struct FaviconBitmapResult;
}

namespace gfx {
class Image;
}

class FaviconUtil {
 public:
  
  
  
  
  
  
  static std::vector<ui::ScaleFactor> GetFaviconScaleFactors();

  
  
  
  static void SetFaviconColorSpace(gfx::Image* image);

  
  
  static gfx::Image SelectFaviconFramesFromPNGs(
      const std::vector<chrome::FaviconBitmapResult>& png_data,
      const std::vector<ui::ScaleFactor>& scale_factors,
      int favicon_size);
};

#endif  
