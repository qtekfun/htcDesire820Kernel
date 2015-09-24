// Copyright 2011 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#include <assert.h>
#include <stdlib.h>

#include "./vp8i.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif


int VP8DecodeLayer(VP8Decoder* const dec) {
  assert(dec);
  assert(dec->layer_data_size_ > 0);
  (void)dec;

  

  return 1;
}

#if defined(__cplusplus) || defined(c_plusplus)
}    
#endif
