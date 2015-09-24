// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_WEBSOCKETS_WEBSOCKET_DEFLATER_H_
#define NET_WEBSOCKETS_WEBSOCKET_DEFLATER_H_

#include <deque>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"

extern "C" struct z_stream_s;

namespace net {

class IOBufferWithSize;

class NET_EXPORT_PRIVATE WebSocketDeflater {
 public:
  enum ContextTakeOverMode {
    DO_NOT_TAKE_OVER_CONTEXT,
    TAKE_OVER_CONTEXT,
  };

  explicit WebSocketDeflater(ContextTakeOverMode mode);
  ~WebSocketDeflater();

  
  
  
  
  bool Initialize(int window_bits);

  
  
  bool AddBytes(const char* data, size_t size);

  
  
  bool Finish();

  
  void PushSyncMark();

  
  
  
  
  
  scoped_refptr<IOBufferWithSize> GetOutput(size_t size);

  
  size_t CurrentOutputSize() const { return buffer_.size(); }

 private:
  void ResetContext();
  int Deflate(int flush);

  scoped_ptr<z_stream_s> stream_;
  ContextTakeOverMode mode_;
  std::deque<char> buffer_;
  std::vector<char> fixed_buffer_;
  
  bool are_bytes_added_;

  DISALLOW_COPY_AND_ASSIGN(WebSocketDeflater);
};

}  

#endif  
