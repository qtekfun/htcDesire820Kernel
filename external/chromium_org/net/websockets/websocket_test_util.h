// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_WEBSOCKETS_WEBSOCKET_TEST_UTIL_H_
#define NET_WEBSOCKETS_WEBSOCKET_TEST_UTIL_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "net/url_request/url_request_test_util.h"
#include "net/websockets/websocket_stream.h"

class GURL;

namespace net {

class BoundNetLog;
class DeterministicSocketData;
class URLRequestContext;
class WebSocketHandshakeStreamCreateHelper;
class DeterministicMockClientSocketFactory;

class LinearCongruentialGenerator {
 public:
  explicit LinearCongruentialGenerator(uint32 seed);
  uint32 Generate();

 private:
  uint64 current_;
};

NET_EXPORT_PRIVATE extern scoped_ptr<WebSocketStreamRequest>
    CreateAndConnectStreamForTesting(
        const GURL& socket_url,
        scoped_ptr<WebSocketHandshakeStreamCreateHelper> create_helper,
        const GURL& origin,
        URLRequestContext* url_request_context,
        const BoundNetLog& net_log,
        scoped_ptr<WebSocketStream::ConnectDelegate> connect_delegate);

extern std::string WebSocketStandardRequest(const std::string& path,
                                            const std::string& origin,
                                            const std::string& extra_headers);

extern std::string WebSocketStandardResponse(const std::string& extra_headers);

class WebSocketDeterministicMockClientSocketFactoryMaker {
 public:
  WebSocketDeterministicMockClientSocketFactoryMaker();
  ~WebSocketDeterministicMockClientSocketFactoryMaker();

  // The socket created by the factory will expect |expect_written| to be
  // written to the socket, and will respond with |return_to_read|. The test
  // will fail if the expected text is not written, or all the bytes are not
  
  void SetExpectations(const std::string& expect_written,
                       const std::string& return_to_read);

  
  void SetRawExpectations(scoped_ptr<DeterministicSocketData> socket_data);

  
  DeterministicMockClientSocketFactory* factory();

 private:
  struct Detail;
  scoped_ptr<Detail> detail_;

  DISALLOW_COPY_AND_ASSIGN(WebSocketDeterministicMockClientSocketFactoryMaker);
};

struct WebSocketTestURLRequestContextHost {
 public:
  WebSocketTestURLRequestContextHost();
  ~WebSocketTestURLRequestContextHost();

  void SetExpectations(const std::string& expect_written,
                       const std::string& return_to_read) {
    maker_.SetExpectations(expect_written, return_to_read);
  }

  void SetRawExpectations(scoped_ptr<DeterministicSocketData> socket_data);

  
  
  
  TestURLRequestContext* GetURLRequestContext();

 private:
  WebSocketDeterministicMockClientSocketFactoryMaker maker_;
  TestURLRequestContext url_request_context_;
  TestNetworkDelegate network_delegate_;

  DISALLOW_COPY_AND_ASSIGN(WebSocketTestURLRequestContextHost);
};

}  

#endif  
