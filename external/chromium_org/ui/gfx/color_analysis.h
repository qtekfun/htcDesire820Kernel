// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_COLOR_ANALYSIS_H_
#define UI_GFX_COLOR_ANALYSIS_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/ref_counted_memory.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/gfx_export.h"
#include "ui/gfx/matrix3_f.h"

class SkBitmap;

namespace color_utils {

class GFX_EXPORT KMeanImageSampler {
 public:
  virtual int GetSample(int width, int height) = 0;

 protected:
  KMeanImageSampler();
  virtual ~KMeanImageSampler();
};

class GFX_EXPORT GridSampler : public KMeanImageSampler {
  public:
   GridSampler();
   virtual ~GridSampler();

   virtual int GetSample(int width, int height) OVERRIDE;

  private:
   
   int calls_;
};

GFX_EXPORT SkColor FindClosestColor(const uint8_t* image, int width, int height,
                                    SkColor color);

GFX_EXPORT SkColor CalculateKMeanColorOfPNG(
    scoped_refptr<base::RefCountedMemory> png,
    uint32_t darkness_limit,
    uint32_t brightness_limit,
    KMeanImageSampler* sampler);

GFX_EXPORT SkColor CalculateKMeanColorOfBitmap(const SkBitmap& bitmap);

GFX_EXPORT gfx::Matrix3F ComputeColorCovariance(const SkBitmap& bitmap);

GFX_EXPORT bool ApplyColorReduction(const SkBitmap& source_bitmap,
                                   const gfx::Vector3dF& color_transform,
                                   bool fit_to_range,
                                   SkBitmap* target_bitmap);

GFX_EXPORT bool ComputePrincipalComponentImage(const SkBitmap& source_bitmap,
                                              SkBitmap* target_bitmap);

}  

#endif  
