// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_WEBSOCKETS_WEBSOCKET_DEFLATE_PREDICTOR_H_
#define NET_WEBSOCKETS_WEBSOCKET_DEFLATE_PREDICTOR_H_

#include "base/basictypes.h"
#include "base/memory/scoped_vector.h"
#include "net/base/net_export.h"

namespace net {

struct WebSocketFrame;

class NET_EXPORT_PRIVATE WebSocketDeflatePredictor {
 public:
  enum Result {
    
    DEFLATE,
    
    DO_NOT_DEFLATE,
    
    
    
    
    TRY_DEFLATE,
  };

  virtual ~WebSocketDeflatePredictor() {}

  
  
  
  
  
  
  
  virtual Result Predict(const ScopedVector<WebSocketFrame>& frames,
                         size_t frame_index) = 0;

  
  
  
  virtual void RecordInputDataFrame(const WebSocketFrame* frame) = 0;

  
  
  // All data frames written by the stream must be recorded in order
  
  virtual void RecordWrittenDataFrame(const WebSocketFrame* frame) = 0;
};

}  

#endif  
