// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SKIA_EXT_LAZY_PIXEL_REF_H_
#define SKIA_EXT_LAZY_PIXEL_REF_H_

#include "third_party/skia/include/core/SkPixelRef.h"
#include "third_party/skia/include/core/SkRect.h"

namespace skia {

class SK_API LazyPixelRef : public SkPixelRef {
 public:
#ifdef SK_SUPPORT_LEGACY_PIXELREF_CONSTRUCTOR
  
  LazyPixelRef();
#endif

  explicit LazyPixelRef(const SkImageInfo& info);
  virtual ~LazyPixelRef();

  struct PrepareParams {
    
    SkIRect clip_rect;
  };

  
  
  virtual bool PrepareToDecode(const PrepareParams& params) = 0;

  
  
  virtual bool MaybeDecoded() = 0;

  
  virtual void Decode() = 0;
};

}  

#endif  
