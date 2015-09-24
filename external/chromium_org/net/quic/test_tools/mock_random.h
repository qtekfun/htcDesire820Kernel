// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_TEST_TOOLS_MOCK_RANDOM_H_
#define NET_QUIC_TEST_TOOLS_MOCK_RANDOM_H_

#include "base/compiler_specific.h"
#include "net/quic/crypto/quic_random.h"

namespace net {

class MockRandom : public QuicRandom {
 public:
  
  MockRandom();
  MockRandom(uint32 base);

  
  
  virtual void RandBytes(void* data, size_t len) OVERRIDE;
  
  virtual uint64 RandUint64() OVERRIDE;
  
  virtual void Reseed(const void* additional_entropy,
                      size_t entropy_len) OVERRIDE;

  
  
  void ChangeValue();

 private:
  uint32 base_;
  uint8 increment_;
};

}  

#endif  
