// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_MEDIA_VIDEO_CAPTURE_BUFFER_POOL_H_
#define CONTENT_BROWSER_RENDERER_HOST_MEDIA_VIDEO_CAPTURE_BUFFER_POOL_H_

#include <map>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/shared_memory.h"
#include "base/process/process.h"
#include "base/synchronization/lock.h"
#include "content/common/content_export.h"
#include "ui/gfx/size.h"

namespace media {

class VideoFrame;

}  

namespace content {

class CONTENT_EXPORT VideoCaptureBufferPool
    : public base::RefCountedThreadSafe<VideoCaptureBufferPool> {
 public:
  static const int kInvalidId;
  explicit VideoCaptureBufferPool(int count);

  
  
  
  base::SharedMemoryHandle ShareToProcess(int buffer_id,
                                          base::ProcessHandle process_handle,
                                          size_t* memory_size);

  
  
  bool GetBufferInfo(int buffer_id, void** memory, size_t* size);

  
  
  
  
  
  
  
  
  
  
  
  
  
  int ReserveForProducer(size_t size, int* buffer_id_to_drop);

  
  
  
  void RelinquishProducerReservation(int buffer_id);

  
  
  
  void HoldForConsumers(int buffer_id, int num_clients);

  
  
  
  void RelinquishConsumerHold(int buffer_id, int num_clients);

  int count() const { return count_; }

 private:
  friend class base::RefCountedThreadSafe<VideoCaptureBufferPool>;

  
  struct Buffer {
    Buffer();

    
    base::SharedMemory shared_memory;

    
    bool held_by_producer;

    
    int consumer_hold_count;
  };

  typedef std::map<int, Buffer*> BufferMap;

  virtual ~VideoCaptureBufferPool();

  int ReserveForProducerInternal(size_t size, int* buffer_id_to_drop);

  Buffer* GetBuffer(int buffer_id);

  
  const int count_;

  
  base::Lock lock_;

  
  int next_buffer_id_;

  
  BufferMap buffers_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(VideoCaptureBufferPool);
};

}  

#endif  
