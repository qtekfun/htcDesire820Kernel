// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_THUMBNAILS_THUMBNAILING_ALGORITHM_H_
#define CHROME_BROWSER_THUMBNAILS_THUMBNAILING_ALGORITHM_H_

#include "base/memory/ref_counted.h"
#include "chrome/browser/thumbnails/thumbnailing_context.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"

class SkBitmap;

namespace thumbnails {

class ThumbnailingAlgorithm
    : public base::RefCountedThreadSafe<ThumbnailingAlgorithm> {
 public:
  typedef base::Callback<void(const ThumbnailingContext&, const SkBitmap&)>
      ConsumerCallback;
  
  
  
  
  
  
  virtual ClipResult GetCanvasCopyInfo(const gfx::Size& source_size,
                                       ui::ScaleFactor scale_factor,
                                       gfx::Rect* clipping_rect,
                                       gfx::Size* target_size) const = 0;

  
  
  
  
  
  
  
  
  virtual void ProcessBitmap(scoped_refptr<ThumbnailingContext> context,
                             const ConsumerCallback& callback,
                             const SkBitmap& bitmap) = 0;

 protected:
  virtual ~ThumbnailingAlgorithm() {}
  friend class base::RefCountedThreadSafe<ThumbnailingAlgorithm>;
};

}

#endif  
