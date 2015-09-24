// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_SOCKET_STREAM_HOST_H_
#define CONTENT_BROWSER_RENDERER_HOST_SOCKET_STREAM_HOST_H_

#include <vector>

#include "base/memory/ref_counted.h"
#include "net/socket_stream/socket_stream.h"

class GURL;

namespace net {
class SocketStreamJob;
class URLRequestContext;
class SSLInfo;
}  

namespace content {

class SocketStreamHost {
 public:
  SocketStreamHost(net::SocketStream::Delegate* delegate,
                   int render_view_id,
                   int socket_id);
  ~SocketStreamHost();

  
  static int SocketIdFromSocketStream(const net::SocketStream* socket);

  int render_view_id() const { return render_view_id_; }
  int socket_id() const { return socket_id_; }

  
  void Connect(const GURL& url, net::URLRequestContext* request_context);

  
  
  
  
  
  bool SendData(const std::vector<char>& data);

  
  void Close();

  
  
  
  
  void CancelWithError(int error);

  
  void CancelWithSSLError(const net::SSLInfo& ssl_info);

  
  void ContinueDespiteError();

 private:
  net::SocketStream::Delegate* delegate_;
  int render_view_id_;
  int socket_id_;

  scoped_refptr<net::SocketStreamJob> job_;

  DISALLOW_COPY_AND_ASSIGN(SocketStreamHost);
};

}  

#endif  
