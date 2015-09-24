// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_THUMBNAILS_SIMPLE_THUMBNAIL_CROP_H_
#define CHROME_BROWSER_THUMBNAILS_SIMPLE_THUMBNAIL_CROP_H_

#include "chrome/browser/thumbnails/thumbnailing_algorithm.h"

namespace thumbnails {

class SimpleThumbnailCrop : public ThumbnailingAlgorithm {
 public:
  explicit SimpleThumbnailCrop(const gfx::Size& target_size);

  virtual ClipResult GetCanvasCopyInfo(const gfx::Size& source_size,
                                       ui::ScaleFactor scale_factor,
                                       gfx::Rect* clipping_rect,
                                       gfx::Size* target_size) const OVERRIDE;

  virtual void ProcessBitmap(scoped_refptr<ThumbnailingContext> context,
                             const ConsumerCallback& callback,
                             const SkBitmap& bitmap) OVERRIDE;

  
  
  
  
  
  static double CalculateBoringScore(const SkBitmap& bitmap);

  
  
  
  
  
  
  static SkBitmap GetClippedBitmap(const SkBitmap& bitmap,
                                   int desired_width,
                                   int desired_height,
                                   thumbnails::ClipResult* clip_result);
  static gfx::Size GetCopySizeForThumbnail(ui::ScaleFactor scale_factor,
                                           const gfx::Size& thumbnail_size);
  static gfx::Rect GetClippingRect(const gfx::Size& source_size,
                                   const gfx::Size& desired_size,
                                   ClipResult* clip_result);

  
  
  
  
  static gfx::Size ComputeTargetSizeAtMaximumScale(const gfx::Size& given_size);

 protected:
  virtual ~SimpleThumbnailCrop();

 private:
  static SkBitmap CreateThumbnail(const SkBitmap& bitmap,
                                  const gfx::Size& desired_size,
                                  ClipResult* clip_result);

  const gfx::Size target_size_;

  DISALLOW_COPY_AND_ASSIGN(SimpleThumbnailCrop);
};

}

#endif  
