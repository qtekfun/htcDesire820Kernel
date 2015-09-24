// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_CHROMEDRIVER_NET_TEST_HTTP_SERVER_H_
#define CHROME_TEST_CHROMEDRIVER_NET_TEST_HTTP_SERVER_H_

#include <set>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/thread.h"
#include "net/server/http_server.h"
#include "url/gurl.h"

namespace base {
class WaitableEvent;
}

class TestHttpServer : public net::HttpServer::Delegate {
 public:
  enum WebSocketRequestAction {
    kAccept,
    kNotFound,
    kClose,
  };

  enum WebSocketMessageAction {
    kEchoMessage,
    kCloseOnMessage
  };

  
  
  TestHttpServer();
  virtual ~TestHttpServer();

  
  bool Start();

  
  void Stop();

  
  
  bool WaitForConnectionsToClose();

  
  void SetRequestAction(WebSocketRequestAction action);

  
  void SetMessageAction(WebSocketMessageAction action);

  
  GURL web_socket_url() const;

  
  virtual void OnHttpRequest(int connection_id,
                             const net::HttpServerRequestInfo& info) OVERRIDE {}
  virtual void OnWebSocketRequest(
      int connection_id,
      const net::HttpServerRequestInfo& info) OVERRIDE;
  virtual void OnWebSocketMessage(int connection_id,
                                  const std::string& data) OVERRIDE;
  virtual void OnClose(int connection_id) OVERRIDE;

 private:
  void StartOnServerThread(bool* success, base::WaitableEvent* event);
  void StopOnServerThread(base::WaitableEvent* event);

  base::Thread thread_;

  
  scoped_refptr<net::HttpServer> server_;

  
  std::set<int> connections_;

  base::WaitableEvent all_closed_event_;

  
  mutable base::Lock url_lock_;
  GURL web_socket_url_;

  
  base::Lock action_lock_;
  WebSocketRequestAction request_action_;
  WebSocketMessageAction message_action_;

  DISALLOW_COPY_AND_ASSIGN(TestHttpServer);
};

#endif  
