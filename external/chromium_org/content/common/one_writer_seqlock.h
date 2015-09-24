// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_ONE_WRITER_SEQLOCK_H_
#define CONTENT_COMMON_ONE_WRITER_SEQLOCK_H_

#include "base/atomicops.h"
#include "base/threading/platform_thread.h"
#include "content/common/content_export.h"

namespace content {

class CONTENT_EXPORT OneWriterSeqLock {
 public:
  OneWriterSeqLock();
  base::subtle::Atomic32 ReadBegin();
  bool ReadRetry(base::subtle::Atomic32 version);
  void WriteBegin();
  void WriteEnd();

 private:
  base::subtle::Atomic32 sequence_;
  DISALLOW_COPY_AND_ASSIGN(OneWriterSeqLock);
};

}  

#endif  
