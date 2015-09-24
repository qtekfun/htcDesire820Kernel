// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_WEBSOCKETS_WEBSOCKET_INFLATER_H_
#define NET_WEBSOCKETS_WEBSOCKET_INFLATER_H_

#include <deque>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"

extern "C" struct z_stream_s;

namespace net {

class IOBufferWithSize;

class NET_EXPORT_PRIVATE WebSocketInflater {
 public:
  WebSocketInflater();
  
  
  WebSocketInflater(size_t input_queue_capacity, size_t output_buffer_capacity);
  ~WebSocketInflater();

  
  
  
  
  bool Initialize(int window_bits);

  
  
  
  
  
  bool AddBytes(const char* data, size_t size);

  
  
  bool Finish();

  
  
  
  
  
  
  
  
  
  
  scoped_refptr<IOBufferWithSize> GetOutput(size_t size);

  
  size_t CurrentOutputSize() const { return output_buffer_.Size(); }

  static const size_t kDefaultBufferCapacity = 512;
  static const size_t kDefaultInputIOBufferCapacity = 512;

 private:
  
  class OutputBuffer {
   public:
    explicit OutputBuffer(size_t capacity);
    ~OutputBuffer();

    size_t Size() const;
    
    
    
    std::pair<char*, size_t> GetTail();
    void Read(char* dest, size_t size);
    void AdvanceTail(size_t advance);

   private:
    void AdvanceHead(size_t advance);

    const size_t capacity_;
    std::vector<char> buffer_;
    size_t head_;
    size_t tail_;
  };

  class InputQueue {
   public:
    
    
    explicit InputQueue(size_t capacity);
    ~InputQueue();

    
    
    
    std::pair<char*, size_t> Top();
    bool IsEmpty() const { return buffers_.empty(); }
    void Push(const char* data, size_t size);
    
    
    void Consume(size_t size);

   private:
    size_t PushToLastBuffer(const char* data, size_t size);

    const size_t capacity_;
    size_t head_of_first_buffer_;
    size_t tail_of_last_buffer_;
    std::deque<scoped_refptr<IOBufferWithSize> > buffers_;
  };

  int InflateWithFlush(const char* next_in, size_t avail_in);
  int Inflate(const char* next_in, size_t avail_in, int flush);
  int InflateChokedInput();

  scoped_ptr<z_stream_s> stream_;
  InputQueue input_queue_;
  OutputBuffer output_buffer_;

  DISALLOW_COPY_AND_ASSIGN(WebSocketInflater);
};

}  

#endif  
