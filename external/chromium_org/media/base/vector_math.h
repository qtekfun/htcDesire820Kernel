// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_VECTOR_MATH_H_
#define MEDIA_BASE_VECTOR_MATH_H_

#include <utility>

#include "media/base/media_export.h"

namespace media {
namespace vector_math {

enum { kRequiredAlignment = 16 };

MEDIA_EXPORT void Initialize();

MEDIA_EXPORT void FMAC(const float src[], float scale, int len, float dest[]);

MEDIA_EXPORT void FMUL(const float src[], float scale, int len, float dest[]);

MEDIA_EXPORT std::pair<float, float> EWMAAndMaxPower(
    float initial_value, const float src[], int len, float smoothing_factor);

}  
}  

#endif  
