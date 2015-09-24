// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_WEBSOCKET_STREAM_H_
#define NET_SPDY_SPDY_WEBSOCKET_STREAM_H_

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "net/base/completion_callback.h"
#include "net/base/request_priority.h"
#include "net/spdy/spdy_framer.h"
#include "net/spdy/spdy_header_block.h"
#include "net/spdy/spdy_session.h"
#include "net/spdy/spdy_stream.h"

namespace net {

class NET_EXPORT_PRIVATE SpdyWebSocketStream
    : public SpdyStream::Delegate {
 public:
  
  class NET_EXPORT_PRIVATE Delegate {
   public:
    
    
    
    virtual void OnCreatedSpdyStream(int status) = 0;

    
    
    virtual void OnSentSpdyHeaders() = 0;

    
    
    
    
    virtual void OnSpdyResponseHeadersUpdated(
        const SpdyHeaderBlock& response_headers) = 0;

    
    virtual void OnSentSpdyData(size_t bytes_sent) = 0;

    
    virtual void OnReceivedSpdyData(scoped_ptr<SpdyBuffer> buffer) = 0;

    
    virtual void OnCloseSpdyStream() = 0;

   protected:
    virtual ~Delegate() {}
  };

  SpdyWebSocketStream(const base::WeakPtr<SpdySession>& spdy_session,
                      Delegate* delegate);
  virtual ~SpdyWebSocketStream();

  
  
  
  
  int InitializeStream(const GURL& url,
                       RequestPriority request_priority,
                       const BoundNetLog& stream_net_log);

  int SendRequest(scoped_ptr<SpdyHeaderBlock> headers);
  int SendData(const char* data, int length);
  void Close();

  
  virtual void OnRequestHeadersSent() OVERRIDE;
  virtual SpdyResponseHeadersStatus OnResponseHeadersUpdated(
      const SpdyHeaderBlock& response_headers) OVERRIDE;
  virtual void OnDataReceived(scoped_ptr<SpdyBuffer> buffer) OVERRIDE;
  virtual void OnDataSent() OVERRIDE;
  virtual void OnClose(int status) OVERRIDE;

 private:
  friend class SpdyWebSocketStreamTest;
  FRIEND_TEST_ALL_PREFIXES(SpdyWebSocketStreamTest, Basic);

  void OnSpdyStreamCreated(int status);

  base::WeakPtrFactory<SpdyWebSocketStream> weak_ptr_factory_;
  SpdyStreamRequest stream_request_;
  base::WeakPtr<SpdyStream> stream_;
  const base::WeakPtr<SpdySession> spdy_session_;
  size_t pending_send_data_length_;
  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(SpdyWebSocketStream);
};

}  

#endif  
