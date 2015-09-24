// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_THUMBNAILS_CONTENT_ANALYSIS_H_
#define CHROME_BROWSER_THUMBNAILS_CONTENT_ANALYSIS_H_

#include <vector>

#include "base/basictypes.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"

class SkBitmap;

namespace thumbnailing_utils {

void ApplyGaussianGradientMagnitudeFilter(SkBitmap* input_bitmap,
                                          float kernel_sigma);

void ExtractImageProfileInformation(const SkBitmap& input_bitmap,
                                    const gfx::Rect& image_area,
                                    const gfx::Size& target_size,
                                    bool apply_log,
                                    std::vector<float>* rows,
                                    std::vector<float>* columns);

float AutoSegmentPeaks(const std::vector<float>& input);

gfx::Size AdjustClippingSizeToAspectRatio(const gfx::Size& target_size,
                                          const gfx::Size& image_size,
                                          const gfx::Size& computed_size);

void ConstrainedProfileSegmentation(const std::vector<float>& row_profile,
                                    const std::vector<float>& column_profile,
                                    const gfx::Size& target_size,
                                    std::vector<bool>* included_rows,
                                    std::vector<bool>* included_columns);

SkBitmap ComputeDecimatedImage(const SkBitmap& bitmap,
                               const std::vector<bool>& rows,
                               const std::vector<bool>& columns);

SkBitmap CreateRetargetedThumbnailImage(const SkBitmap& source_bitmap,
                                        const gfx::Size& target_size,
                                        float kernel_sigma);

}  

#endif  
