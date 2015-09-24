// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_CODEC_JPEG_CODEC_H_
#define UI_GFX_CODEC_JPEG_CODEC_H_

#include <stddef.h>
#include <vector>

#include "ui/gfx/gfx_export.h"

class SkBitmap;

namespace gfx {

class GFX_EXPORT JPEGCodec {
 public:
  enum ColorFormat {
    
    
    FORMAT_RGB,

    
    FORMAT_RGBA,

    
    
    FORMAT_BGRA,

    
    
    FORMAT_SkBitmap
  };

  enum LibraryVariant {
    SYSTEM_LIBJPEG = 0,
    LIBJPEG_TURBO,
    IJG_LIBJPEG,
  };

  
  static LibraryVariant JpegLibraryVariant();

  
  // given in 'format'. The encoded JPEG data will be written into the supplied
  
  
  
  
  
  
  
  
  static bool Encode(const unsigned char* input, ColorFormat format,
                     int w, int h, int row_byte_width,
                     int quality, std::vector<unsigned char>* output);

  
  
  // on success (returns true). This data will be written in the'format'
  
  static bool Decode(const unsigned char* input, size_t input_size,
                     ColorFormat format, std::vector<unsigned char>* output,
                     int* w, int* h);

  
  
  
  static SkBitmap* Decode(const unsigned char* input, size_t input_size);
};

}  

#endif  
