// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_IMAGEDIFF_IMAGE_DIFF_PNG_H_
#define TOOLS_IMAGEDIFF_IMAGE_DIFF_PNG_H_

#include <string>
#include <vector>

namespace image_diff_png {

bool DecodePNG(const unsigned char* input, size_t input_size,
               std::vector<unsigned char>* output,
               int* width, int* height);

bool EncodeRGBAPNG(const unsigned char* input,
                   int width,
                   int height,
                   int row_byte_width,
                   std::vector<unsigned char>* output);

bool EncodeBGRAPNG(const unsigned char* input,
                   int width,
                   int height,
                   int row_byte_width,
                   bool discard_transparency,
                   std::vector<unsigned char>* output);

}  

#endif  
