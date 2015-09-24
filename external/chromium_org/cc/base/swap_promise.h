// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_BASE_SWAP_PROMISE_H_
#define CC_BASE_SWAP_PROMISE_H_

#include "cc/output/compositor_frame_metadata.h"

namespace cc {

const unsigned int kMaxQueuedSwapPromiseNumber = 100;

class CC_EXPORT SwapPromise {
 public:
  enum DidNotSwapReason {
    DID_NOT_SWAP_UNKNOWN,
    SWAP_FAILS,
    COMMIT_FAILS,
    SWAP_PROMISE_LIST_OVERFLOW,
  };

  SwapPromise() {}
  virtual ~SwapPromise() {}

  virtual void DidSwap(CompositorFrameMetadata* metadata) = 0;
  virtual void DidNotSwap(DidNotSwapReason reason) = 0;
};

}  

#endif  
