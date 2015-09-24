// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TEST_EMBEDDED_TEST_SERVER_HTTP_CONNECTION_H_
#define NET_TEST_EMBEDDED_TEST_SERVER_HTTP_CONNECTION_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/strings/string_piece.h"
#include "net/test/embedded_test_server/http_request.h"

namespace net {

class StreamListenSocket;

namespace test_server {

class HttpConnection;
class HttpResponse;

typedef base::Callback<void(HttpConnection* connection,
                            scoped_ptr<HttpRequest> request)>
    HandleRequestCallback;

class HttpConnection {
 public:
  HttpConnection(scoped_ptr<StreamListenSocket> socket,
                 const HandleRequestCallback& callback);
  ~HttpConnection();

  
  void SendResponse(scoped_ptr<HttpResponse> response) const;

 private:
  friend class EmbeddedTestServer;

  
  
  
  void ReceiveData(const base::StringPiece& data);

  scoped_ptr<StreamListenSocket> socket_;
  const HandleRequestCallback callback_;
  HttpRequestParser request_parser_;

  DISALLOW_COPY_AND_ASSIGN(HttpConnection);
};

}  
}  

#endif  
