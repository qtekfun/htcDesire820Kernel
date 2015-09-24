// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_IMAGE_IMAGE_UTIL_H_
#define UI_GFX_IMAGE_IMAGE_UTIL_H_

#include <vector>

#include "base/basictypes.h"
#include "ui/gfx/gfx_export.h"

namespace gfx {
class Image;
}

namespace gfx {

GFX_EXPORT Image ImageFrom1xJPEGEncodedData(const unsigned char* input,
                                            size_t input_size);

GFX_EXPORT bool JPEG1xEncodedDataFromImage(const Image& image,
                                           int quality,
                                           std::vector<unsigned char>* dst);

}  

#endif  
