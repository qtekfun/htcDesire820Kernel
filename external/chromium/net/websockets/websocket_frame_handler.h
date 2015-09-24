// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_WEBSOCKETS_WEBSOCKET_FRAME_HANDLER_H_
#define NET_WEBSOCKETS_WEBSOCKET_FRAME_HANDLER_H_
#pragma once

#include <deque>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"

namespace net {

class IOBuffer;
class IOBufferWithSize;

class WebSocketFrameHandler {
 public:
  struct FrameInfo {
    const char* frame_start;
    int frame_length;
    const char* message_start;
    int message_length;
  };

  WebSocketFrameHandler();
  ~WebSocketFrameHandler();

  
  
  
  void AppendData(const char* data, int len);

  
  
  
  
  
  int UpdateCurrentBuffer(bool buffered);

  
  
  
  
  IOBuffer* GetCurrentBuffer() { return current_buffer_.get(); }
  int GetCurrentBufferSize() const { return current_buffer_size_; }

  
  
  
  int GetOriginalBufferSize() const { return original_current_buffer_size_; }

  
  void ReleaseCurrentBuffer();

  
  
  
  static int ParseWebSocketFrame(const char* buffer, int size,
                                 std::vector<FrameInfo>* frame_info);

 private:
  typedef std::deque< scoped_refptr<IOBufferWithSize> > PendingDataQueue;

  scoped_refptr<IOBuffer> current_buffer_;
  int current_buffer_size_;

  int original_current_buffer_size_;

  
  PendingDataQueue pending_buffers_;

  DISALLOW_COPY_AND_ASSIGN(WebSocketFrameHandler);
};

}  

#endif  
