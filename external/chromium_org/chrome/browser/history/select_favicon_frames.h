// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_SELECT_FAVICON_FRAMES_H_
#define CHROME_BROWSER_HISTORY_SELECT_FAVICON_FRAMES_H_

#include <vector>

#include "ui/base/layout.h"

class SkBitmap;

namespace gfx {
class ImageSkia;
class Size;
}

extern const float kSelectFaviconFramesInvalidScore;

gfx::ImageSkia SelectFaviconFrames(
    const std::vector<SkBitmap>& bitmaps,
    const std::vector<gfx::Size>& original_sizes,
    const std::vector<ui::ScaleFactor>& scale_factors,
    int desired_size,
    float* score);

void SelectFaviconFrameIndices(
    const std::vector<gfx::Size>& frame_pixel_sizes,
    const std::vector<ui::ScaleFactor>& scale_factors,
    int desired_size,
    std::vector<size_t>* best_indices,
    float* score);

#endif  
