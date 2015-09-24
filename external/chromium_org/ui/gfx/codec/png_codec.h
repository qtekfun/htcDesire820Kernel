// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_CODEC_PNG_CODEC_H_
#define UI_GFX_CODEC_PNG_CODEC_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "ui/gfx/gfx_export.h"

class SkBitmap;

namespace gfx {

class Size;

class GFX_EXPORT PNGCodec {
 public:
  enum ColorFormat {
    
    
    FORMAT_RGB,

    
    FORMAT_RGBA,

    
    
    FORMAT_BGRA,

    
    
    FORMAT_SkBitmap
  };

  
  struct GFX_EXPORT Comment {
    Comment(const std::string& k, const std::string& t);
    ~Comment();

    std::string key;
    std::string text;
  };

  
  // given in 'format'. The encoded PNG data will be written into the supplied
  
  
  
  
  
  
  
  
  
  
  
  
  //   written to the resulting file. Otherwise, alpha values in the input
  
  // comments: comments to be written in the png's metadata.
  static bool Encode(const unsigned char* input,
                     ColorFormat format,
                     const Size& size,
                     int row_byte_width,
                     bool discard_transparency,
                     const std::vector<Comment>& comments,
                     std::vector<unsigned char>* output);

  
  
  
  
  static bool EncodeBGRASkBitmap(const SkBitmap& input,
                                 bool discard_transparency,
                                 std::vector<unsigned char>* output);

  
  
  
  static bool FastEncodeBGRASkBitmap(const SkBitmap& input,
                                     bool discard_transparency,
                                     std::vector<unsigned char>* output);

  
  
  // on success (returns true). This data will be written in the 'format'
  
  
  
  
  // really designed to be able to read in something written by Encode() above.
  static bool Decode(const unsigned char* input, size_t input_size,
                     ColorFormat format, std::vector<unsigned char>* output,
                     int* w, int* h);

  
  
  
  
  
  
  
  
  static bool Decode(const unsigned char* input, size_t input_size,
                     SkBitmap* bitmap);

 private:
  DISALLOW_COPY_AND_ASSIGN(PNGCodec);
};

}  

#endif  
