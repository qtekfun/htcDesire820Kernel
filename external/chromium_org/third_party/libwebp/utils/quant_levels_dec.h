// Copyright 2013 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#ifndef WEBP_UTILS_QUANT_LEVELS_DEC_H_
#define WEBP_UTILS_QUANT_LEVELS_DEC_H_

#include "../webp/types.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

int DequantizeLevels(uint8_t* const data, int width, int height);

#if defined(__cplusplus) || defined(c_plusplus)
}    
#endif

#endif  
