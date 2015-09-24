// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_CACHEINVALIDATION_DEPS_RANDOM_H_
#define GOOGLE_CACHEINVALIDATION_DEPS_RANDOM_H_

#include "base/rand_util.h"

namespace invalidation {

class Random {
 public:
  
  explicit Random(int64 seed) {}

  virtual ~Random() {}

  
  virtual double RandDouble();

  virtual uint64 RandUint64();
};

}  

#endif  
