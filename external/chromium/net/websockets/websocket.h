// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_WEBSOCKETS_WEBSOCKET_H_
#define NET_WEBSOCKETS_WEBSOCKET_H_
#pragma once

#include <deque>
#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "googleurl/src/gurl.h"
#include "net/base/io_buffer.h"
#include "net/socket_stream/socket_stream.h"
#include "net/url_request/url_request_context.h"

class MessageLoop;

namespace net {

class ClientSocketFactory;
class HostResolver;

class WebSocket;
class WebSocketHandshake;

class WebSocketDelegate {
 public:
  virtual ~WebSocketDelegate() {}

  
  virtual void OnOpen(WebSocket* socket) = 0;

  
  
  virtual void OnMessage(WebSocket* socket, const std::string& msg) = 0;

  
  virtual void OnError(WebSocket* socket) {}

  
  virtual void OnClose(WebSocket* socket, bool was_clean) = 0;

  
  virtual void OnSocketError(const WebSocket* socket, int error) {}
};

class WebSocket : public base::RefCountedThreadSafe<WebSocket>,
                  public SocketStream::Delegate {
 public:
  enum State {
    INITIALIZED = -1,
    CONNECTING = 0,
    OPEN = 1,
    CLOSING = 2,
    CLOSED = 3,
  };
  enum ProtocolVersion {
    DEFAULT_VERSION = 0,
    DRAFT75 = 1,
  };
  class Request {
   public:
    Request(const GURL& url, const std::string protocol,
            const std::string origin, const std::string location,
            ProtocolVersion version,
            net::URLRequestContext* context)
        : url_(url),
          protocol_(protocol),
          origin_(origin),
          location_(location),
          version_(version),
          context_(context),
          host_resolver_(NULL),
          client_socket_factory_(NULL) {}
    ~Request() {}

    const GURL& url() const { return url_; }
    const std::string& protocol() const { return protocol_; }
    const std::string& origin() const { return origin_; }
    const std::string& location() const { return location_; }
    ProtocolVersion version() const { return version_; }
    net::URLRequestContext* context() const { return context_; }

    
    void SetHostResolver(HostResolver* host_resolver) {
      host_resolver_ = host_resolver;
    }
    HostResolver* host_resolver() const { return host_resolver_; }

    
    
    void SetClientSocketFactory(ClientSocketFactory* factory) {
      client_socket_factory_ = factory;
    }
    ClientSocketFactory* client_socket_factory() const {
      return client_socket_factory_;
    }

   private:
    GURL url_;
    std::string protocol_;
    std::string origin_;
    std::string location_;
    ProtocolVersion version_;
    scoped_refptr<net::URLRequestContext> context_;

    HostResolver* host_resolver_;
    ClientSocketFactory* client_socket_factory_;

    DISALLOW_COPY_AND_ASSIGN(Request);
  };

  
  
  
  WebSocket(Request* req, WebSocketDelegate* delegate);

  const Request* request() const { return request_.get(); }
  WebSocketDelegate* delegate() const { return delegate_; }

  State ready_state() const { return ready_state_; }

  
  void Connect();

  
  
  void Send(const std::string& msg);

  
  void Close();

  
  
  
  void DetachDelegate();

  
  
  virtual void OnConnected(SocketStream* socket_stream,
                           int max_pending_send_allowed);
  virtual void OnSentData(SocketStream* socket_stream, int amount_sent);
  virtual void OnReceivedData(SocketStream* socket_stream,
                              const char* data, int len);
  virtual void OnClose(SocketStream* socket);
  virtual void OnError(const SocketStream* socket, int error);

 private:
  typedef std::deque< scoped_refptr<IOBufferWithSize> > PendingDataQueue;

  friend class WebSocketTest;

  friend class base::RefCountedThreadSafe<WebSocket>;
  virtual ~WebSocket();

  
  void SendPending();

  
  void DoReceivedData();

  
  void ProcessFrameData();

  
  void AddToReadBuffer(const char* data, int len);

  
  void SkipReadBuffer(int len);

  void StartClosingHandshake();
  void DoForceCloseConnection();
  void FailConnection();
  
  void DoClose();

  
  void DoSocketError(int error);

  State ready_state_;
  scoped_ptr<Request> request_;
  scoped_ptr<WebSocketHandshake> handshake_;
  WebSocketDelegate* delegate_;
  MessageLoop* origin_loop_;

  scoped_refptr<SocketStream> socket_stream_;
  int max_pending_send_allowed_;

  
  
  
  
  scoped_refptr<GrowableIOBuffer> current_read_buf_;
  int read_consumed_len_;

  
  
  
  scoped_refptr<DrainableIOBuffer> current_write_buf_;

  
  
  PendingDataQueue pending_write_bufs_;

  
  bool server_closing_handshake_;
  
  bool client_closing_handshake_;
  
  bool closing_handshake_started_;
  
  
  CancelableTask* force_close_task_;
  int64 closing_handshake_timeout_;

  DISALLOW_COPY_AND_ASSIGN(WebSocket);
};

}  

#endif  
