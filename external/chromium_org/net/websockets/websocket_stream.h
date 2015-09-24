// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_WEBSOCKETS_WEBSOCKET_STREAM_H_
#define NET_WEBSOCKETS_WEBSOCKET_STREAM_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"

class GURL;

namespace net {

class BoundNetLog;
class URLRequestContext;
struct WebSocketFrame;

class NET_EXPORT_PRIVATE WebSocketStreamRequest {
 public:
  virtual ~WebSocketStreamRequest();
};


class NET_EXPORT_PRIVATE WebSocketStream {
 public:
  
  
  class NET_EXPORT_PRIVATE ConnectDelegate {
   public:
    virtual ~ConnectDelegate();
    
    
    virtual void OnSuccess(scoped_ptr<WebSocketStream> stream) = 0;

    
    
    
    virtual void OnFailure(unsigned short websocket_error) = 0;
  };

  
  
  
  
  
  
  
  
  
  
  
  
  static scoped_ptr<WebSocketStreamRequest> CreateAndConnectStream(
      const GURL& socket_url,
      const std::vector<std::string>& requested_subprotocols,
      const GURL& origin,
      URLRequestContext* url_request_context,
      const BoundNetLog& net_log,
      scoped_ptr<ConnectDelegate> connect_delegate);

  
  
  virtual ~WebSocketStream();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int ReadFrames(ScopedVector<WebSocketFrame>* frames,
                         const CompletionCallback& callback) = 0;

  
  
  
  
  
  
  
  // This method will only return OK if all frames were written completely.
  
  
  
  
  
  
  virtual int WriteFrames(ScopedVector<WebSocketFrame>* frames,
                          const CompletionCallback& callback) = 0;

  
  
  virtual void Close() = 0;

  
  
  virtual std::string GetSubProtocol() const = 0;

  
  
  
  
  
  
  
  
  
  virtual std::string GetExtensions() const = 0;

 protected:
  WebSocketStream();

 private:
  DISALLOW_COPY_AND_ASSIGN(WebSocketStream);
};

}  

#endif  
