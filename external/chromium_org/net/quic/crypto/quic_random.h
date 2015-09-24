// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CRYPTO_QUIC_RANDOM_H_
#define NET_QUIC_CRYPTO_QUIC_RANDOM_H_

#include <stddef.h>

#include "base/basictypes.h"
#include "net/base/net_export.h"

namespace net {

class NET_EXPORT_PRIVATE QuicRandom {
 public:
  virtual ~QuicRandom() {}

  
  
  static QuicRandom* GetInstance();

  
  virtual void RandBytes(void* data, size_t len) = 0;

  
  virtual uint64 RandUint64() = 0;

  
  
  
  virtual void Reseed(const void* additional_entropy, size_t entropy_len) = 0;
};

}  

#endif  
