// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_SHARED_MEMORY_SEQLOCK_BUFFER_H_
#define CONTENT_COMMON_SHARED_MEMORY_SEQLOCK_BUFFER_H_

#include "content/common/one_writer_seqlock.h"

namespace content {


template<class Data>
class SharedMemorySeqLockBuffer {
 public:
  OneWriterSeqLock seqlock;
  Data data;
};

}  

#endif  
