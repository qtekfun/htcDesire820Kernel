// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_THUMBNAILS_CONTENT_BASED_THUMBNAILING_ALGORITHM_H_
#define CHROME_BROWSER_THUMBNAILS_CONTENT_BASED_THUMBNAILING_ALGORITHM_H_

#include "chrome/browser/thumbnails/thumbnailing_algorithm.h"

namespace thumbnails {

class ContentBasedThumbnailingAlgorithm : public ThumbnailingAlgorithm {
 public:
  explicit ContentBasedThumbnailingAlgorithm(const gfx::Size& target_size);

  virtual ClipResult GetCanvasCopyInfo(const gfx::Size& source_size,
                                       ui::ScaleFactor scale_factor,
                                       gfx::Rect* clipping_rect,
                                       gfx::Size* target_size) const OVERRIDE;

  virtual void ProcessBitmap(scoped_refptr<ThumbnailingContext> context,
                             const ConsumerCallback& callback,
                             const SkBitmap& bitmap) OVERRIDE;

  
  
  
  static SkBitmap PrepareSourceBitmap(const SkBitmap& received_bitmap,
                                      const gfx::Size& thumbnail_size,
                                      ThumbnailingContext* context);

  
  
  
  static void CreateRetargetedThumbnail(
      const SkBitmap& source_bitmap,
      const gfx::Size& thumbnail_size,
      scoped_refptr<ThumbnailingContext> context,
      const ConsumerCallback& callback);

 protected:
  virtual ~ContentBasedThumbnailingAlgorithm();

 private:
  static gfx::Rect GetClippingRect(const gfx::Size& source_size,
                                   const gfx::Size& thumbnail_size,
                                   gfx::Size* target_size,
                                   ClipResult* clip_result);

  const gfx::Size target_size_;

  DISALLOW_COPY_AND_ASSIGN(ContentBasedThumbnailingAlgorithm);
};

}  

#endif  
