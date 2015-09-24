// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_DEV_CRYPTO_DEV_H_
#define PPAPI_CPP_DEV_CRYPTO_DEV_H_

#include "ppapi/c/pp_stdint.h"


namespace pp {

class Crypto_Dev {
 public:
  Crypto_Dev() {}

  
  
  
  
  
  
  bool GetRandomBytes(char* buffer, uint32_t num_bytes);
};

}  

#endif  
