// Copyright 2011 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#ifndef WEBP_UTILS_QUANT_LEVELS_H_
#define WEBP_UTILS_QUANT_LEVELS_H_

#include <stdlib.h>

#include "../webp/types.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

int QuantizeLevels(uint8_t* const data, int width, int height, int num_levels,
                   uint64_t* const sse);

#if defined(__cplusplus) || defined(c_plusplus)
}    
#endif

#endif  
