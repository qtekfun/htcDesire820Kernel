// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_WRITE_QUEUE_H_
#define NET_SPDY_SPDY_WRITE_QUEUE_H_

#include <deque>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "net/base/net_export.h"
#include "net/base/request_priority.h"
#include "net/spdy/spdy_protocol.h"

namespace net {

class SpdyBuffer;
class SpdyBufferProducer;
class SpdyStream;

class NET_EXPORT_PRIVATE SpdyWriteQueue {
 public:
  SpdyWriteQueue();
  ~SpdyWriteQueue();

  
  
  bool IsEmpty() const;

  
  
  
  
  
  void Enqueue(RequestPriority priority,
               SpdyFrameType frame_type,
               scoped_ptr<SpdyBufferProducer> frame_producer,
               const base::WeakPtr<SpdyStream>& stream);

  
  
  
  
  bool Dequeue(SpdyFrameType* frame_type,
               scoped_ptr<SpdyBufferProducer>* frame_producer,
               base::WeakPtr<SpdyStream>* stream);

  
  
  void RemovePendingWritesForStream(const base::WeakPtr<SpdyStream>& stream);

  
  
  void RemovePendingWritesForStreamsAfter(SpdyStreamId last_good_stream_id);

  
  void Clear();

 private:
  
  struct PendingWrite {
    SpdyFrameType frame_type;
    
    
    SpdyBufferProducer* frame_producer;
    base::WeakPtr<SpdyStream> stream;
    
    bool has_stream;

    PendingWrite();
    PendingWrite(SpdyFrameType frame_type,
                 SpdyBufferProducer* frame_producer,
                 const base::WeakPtr<SpdyStream>& stream);
    ~PendingWrite();
  };

  
  std::deque<PendingWrite> queue_[NUM_PRIORITIES];

  DISALLOW_COPY_AND_ASSIGN(SpdyWriteQueue);
};

}  

#endif  
