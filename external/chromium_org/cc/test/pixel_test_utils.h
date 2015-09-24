// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_TEST_PIXEL_TEST_UTILS_H_
#define CC_TEST_PIXEL_TEST_UTILS_H_

#include "base/files/file_path.h"
#include "cc/test/pixel_comparator.h"

class SkBitmap;

namespace cc {

bool WritePNGFile(const SkBitmap& bitmap, const base::FilePath& file_path,
    bool discard_transparency);

bool ReadPNGFile(const base::FilePath& file_path, SkBitmap* bitmap);

bool MatchesPNGFile(const SkBitmap& gen_bmp,
                    base::FilePath ref_img_path,
                    const PixelComparator& comparator);

}  

#endif  
