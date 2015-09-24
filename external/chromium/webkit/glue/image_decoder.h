// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/basictypes.h"
#include "ui/gfx/size.h"

class SkBitmap;

namespace webkit_glue {

class ImageDecoder {
 public:
  
  
  
  ImageDecoder();
  ImageDecoder(const gfx::Size& desired_icon_size);
  ~ImageDecoder();

  
  
  SkBitmap Decode(const unsigned char* data, size_t size) const;

 private:
  
  gfx::Size desired_icon_size_;

  DISALLOW_COPY_AND_ASSIGN(ImageDecoder);
};

}  
