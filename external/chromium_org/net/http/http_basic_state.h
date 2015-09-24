// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_BASIC_STATE_H_
#define NET_HTTP_HTTP_BASIC_STATE_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"
#include "net/base/request_priority.h"

namespace net {

class BoundNetLog;
class ClientSocketHandle;
class GrowableIOBuffer;
class HttpStreamParser;
struct HttpRequestInfo;

class NET_EXPORT_PRIVATE HttpBasicState {
 public:
  HttpBasicState(ClientSocketHandle* connection, bool using_proxy);
  ~HttpBasicState();

  
  int Initialize(const HttpRequestInfo* request_info,
                 RequestPriority priority,
                 const BoundNetLog& net_log,
                 const CompletionCallback& callback);

  HttpStreamParser* parser() const { return parser_.get(); }

  bool using_proxy() const { return using_proxy_; }

  
  void DeleteParser();

  ClientSocketHandle* connection() const { return connection_.get(); }

  scoped_ptr<ClientSocketHandle> ReleaseConnection();

  scoped_refptr<GrowableIOBuffer> read_buf() const;

  
  
  std::string GenerateRequestLine() const;

 private:
  scoped_refptr<GrowableIOBuffer> read_buf_;

  scoped_ptr<HttpStreamParser> parser_;

  scoped_ptr<ClientSocketHandle> connection_;

  const bool using_proxy_;

  const HttpRequestInfo* request_info_;

  DISALLOW_COPY_AND_ASSIGN(HttpBasicState);
};

}  

#endif  
