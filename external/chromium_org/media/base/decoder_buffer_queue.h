// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_DECODER_BUFFER_QUEUE_H_
#define MEDIA_BASE_DECODER_BUFFER_QUEUE_H_

#include <deque>

#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "media/base/media_export.h"

namespace media {

class DecoderBuffer;

class MEDIA_EXPORT DecoderBufferQueue {
 public:
  DecoderBufferQueue();
  ~DecoderBufferQueue();

  
  
  
  
  void Push(const scoped_refptr<DecoderBuffer>& buffer);

  
  
  
  scoped_refptr<DecoderBuffer> Pop();

  
  void Clear();

  
  bool IsEmpty();

  
  
  
  
  
  base::TimeDelta Duration();

 private:
  typedef std::deque<scoped_refptr<DecoderBuffer> > Queue;
  Queue queue_;

  
  
  
  Queue in_order_queue_;

  base::TimeDelta earliest_valid_timestamp_;

  DISALLOW_COPY_AND_ASSIGN(DecoderBufferQueue);
};

}  

#endif  
