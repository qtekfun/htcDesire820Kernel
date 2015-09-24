// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TEST_EMBEDDED_TEST_SERVER_EMBEDDED_TEST_SERVER_H_
#define NET_TEST_EMBEDDED_TEST_SERVER_EMBEDDED_TEST_SERVER_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/thread.h"
#include "base/threading/thread_checker.h"
#include "net/socket/tcp_listen_socket.h"
#include "url/gurl.h"

namespace base {
class FilePath;
}

namespace net {
namespace test_server {

class HttpConnection;
class HttpResponse;
struct HttpRequest;

class HttpListenSocket : public TCPListenSocket {
 public:
  HttpListenSocket(const SocketDescriptor socket_descriptor,
                   StreamListenSocket::Delegate* delegate);
  virtual ~HttpListenSocket();
  virtual void Listen();

 private:
  friend class EmbeddedTestServer;

  
  void DetachFromThread();

  base::ThreadChecker thread_checker_;
};

class EmbeddedTestServer : public StreamListenSocket::Delegate {
 public:
  typedef base::Callback<scoped_ptr<HttpResponse>(
      const HttpRequest& request)> HandleRequestCallback;

  
  
  EmbeddedTestServer();
  virtual ~EmbeddedTestServer();

  
  bool InitializeAndWaitUntilReady() WARN_UNUSED_RESULT;

  
  bool ShutdownAndWaitUntilComplete() WARN_UNUSED_RESULT;

  
  bool Started() const {
    return listen_socket_.get() != NULL;
  }

  
  
  
  const GURL& base_url() const { return base_url_; }

  
  
  
  GURL GetURL(const std::string& relative_url) const;

  
  int port() const { return port_; }

  
  
  
  
  void ServeFilesFromDirectory(const base::FilePath& directory);

  
  
  
  void RegisterRequestHandler(const HandleRequestCallback& callback);

  
  void StopThread();

  
  void RestartThreadAndListen();

 private:
  void StartThread();

  
  
  void InitializeOnIOThread();
  void ListenOnIOThread();

  
  void ShutdownOnIOThread();

  
  
  void HandleRequest(HttpConnection* connection,
                     scoped_ptr<HttpRequest> request);

  
  virtual void DidAccept(StreamListenSocket* server,
                         scoped_ptr<StreamListenSocket> connection) OVERRIDE;
  virtual void DidRead(StreamListenSocket* connection,
                       const char* data,
                       int length) OVERRIDE;
  virtual void DidClose(StreamListenSocket* connection) OVERRIDE;

  HttpConnection* FindConnection(StreamListenSocket* socket);

  
  bool PostTaskToIOThreadAndWait(
      const base::Closure& closure) WARN_UNUSED_RESULT;

  scoped_ptr<base::Thread> io_thread_;

  scoped_ptr<HttpListenSocket> listen_socket_;
  int port_;
  GURL base_url_;

  
  std::map<StreamListenSocket*, HttpConnection*> connections_;

  
  std::vector<HandleRequestCallback> request_handlers_;

  
  
  base::WeakPtrFactory<EmbeddedTestServer> weak_factory_;

  base::ThreadChecker thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(EmbeddedTestServer);
};

}  
}  

#endif  
