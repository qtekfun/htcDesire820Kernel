// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_STREAM_SOCKET_STREAM_H_
#define NET_SOCKET_STREAM_SOCKET_STREAM_H_
#pragma once

#include <deque>
#include <map>
#include <string>

#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "base/task.h"
#include "net/base/address_list.h"
#include "net/base/completion_callback.h"
#include "net/base/io_buffer.h"
#include "net/base/net_log.h"
#include "net/base/net_errors.h"
#include "net/base/ssl_config_service.h"
#include "net/http/http_auth.h"
#include "net/http/http_auth_cache.h"
#include "net/http/http_auth_handler.h"
#include "net/proxy/proxy_service.h"
#include "net/socket/tcp_client_socket.h"
#include "net/url_request/url_request_context.h"
#include "net/base/net_export.h"

namespace net {

class AuthChallengeInfo;
class ClientSocketFactory;
class HostResolver;
class HttpAuthHandlerFactory;
class SSLConfigService;
class SingleRequestHostResolver;
class SocketStreamMetrics;

class NET_EXPORT SocketStream : public base::RefCountedThreadSafe<SocketStream> {
 public:
  
  
  
  class UserData {
   public:
    UserData() {}
    virtual ~UserData() {}
  };

  class Delegate {
   public:
    virtual ~Delegate() {}

    virtual int OnStartOpenConnection(SocketStream* socket,
                                      CompletionCallback* callback) {
      return OK;
    }

    
    
    
    
    virtual void OnConnected(SocketStream* socket,
                             int max_pending_send_allowed) = 0;

    
    virtual void OnSentData(SocketStream* socket,
                            int amount_sent) = 0;

    
    virtual void OnReceivedData(SocketStream* socket,
                                const char* data, int len) = 0;

    
    virtual void OnClose(SocketStream* socket) = 0;

    
    
    
    virtual void OnAuthRequired(SocketStream* socket,
                                AuthChallengeInfo* auth_info) {
      
      socket->Close();
    }

    
    
    
    virtual void OnError(const SocketStream* socket, int error) {}
  };

  SocketStream(const GURL& url, Delegate* delegate);

  
  
  
  
  UserData* GetUserData(const void* key) const;
  void SetUserData(const void* key, UserData* data);

  const GURL& url() const { return url_; }
  bool is_secure() const;
  const AddressList& address_list() const { return addresses_; }
  Delegate* delegate() const { return delegate_; }
  int max_pending_send_allowed() const { return max_pending_send_allowed_; }

  URLRequestContext* context() const { return context_.get(); }
  void set_context(URLRequestContext* context);

  BoundNetLog* net_log() { return &net_log_; }

  
  
  virtual void Connect();

  
  
  
  
  virtual bool SendData(const char* data, int len);

  
  
  virtual void Close();

  
  
  virtual void RestartWithAuth(
      const string16& username,
      const string16& password);

  
  
  
  virtual void DetachDelegate();

  
  void SetHostResolver(HostResolver* host_resolver);

  
  
  void SetClientSocketFactory(ClientSocketFactory* factory);

 protected:
  friend class base::RefCountedThreadSafe<SocketStream>;
  virtual ~SocketStream();

  Delegate* delegate_;

 private:
  friend class WebSocketThrottleTest;

  typedef std::map<const void*, linked_ptr<UserData> > UserDataMap;
  typedef std::deque< scoped_refptr<IOBufferWithSize> > PendingDataQueue;

  class RequestHeaders : public IOBuffer {
   public:
    RequestHeaders() : IOBuffer() {}

    void SetDataOffset(size_t offset) {
      data_ = const_cast<char*>(headers_.data()) + offset;
    }

    std::string headers_;

    private:
     ~RequestHeaders() { data_ = NULL; }
  };

  class ResponseHeaders : public IOBuffer {
   public:
    ResponseHeaders();

    void SetDataOffset(size_t offset) { data_ = headers_.get() + offset; }
    char* headers() const { return headers_.get(); }
    void Reset() { headers_.reset(); }
    void Realloc(size_t new_size);

   private:
     ~ResponseHeaders();

    scoped_ptr_malloc<char> headers_;
  };

  enum State {
    STATE_NONE,
    STATE_RESOLVE_PROXY,
    STATE_RESOLVE_PROXY_COMPLETE,
    STATE_RESOLVE_HOST,
    STATE_RESOLVE_HOST_COMPLETE,
    STATE_TCP_CONNECT,
    STATE_TCP_CONNECT_COMPLETE,
    STATE_WRITE_TUNNEL_HEADERS,
    STATE_WRITE_TUNNEL_HEADERS_COMPLETE,
    STATE_READ_TUNNEL_HEADERS,
    STATE_READ_TUNNEL_HEADERS_COMPLETE,
    STATE_SOCKS_CONNECT,
    STATE_SOCKS_CONNECT_COMPLETE,
    STATE_SSL_CONNECT,
    STATE_SSL_CONNECT_COMPLETE,
    STATE_READ_WRITE,
    STATE_AUTH_REQUIRED,
    STATE_CLOSE,
  };

  enum ProxyMode {
    kDirectConnection,  
    kTunnelProxy,  
    kSOCKSProxy,  
  };

  
  enum { kMaxTunnelResponseHeadersSize = 32768 };  

  
  
  void CopyAddrInfo(struct addrinfo* head);

  void DoClose();

  
  
  
  void Finish(int result);

  int DidEstablishConnection();
  int DidReceiveData(int result);
  int DidSendData(int result);

  void OnIOCompleted(int result);
  void OnReadCompleted(int result);
  void OnWriteCompleted(int result);

  void DoLoop(int result);

  int DoResolveProxy();
  int DoResolveProxyComplete(int result);
  int DoResolveHost();
  int DoResolveHostComplete(int result);
  int DoTcpConnect(int result);
  int DoTcpConnectComplete(int result);
  int DoWriteTunnelHeaders();
  int DoWriteTunnelHeadersComplete(int result);
  int DoReadTunnelHeaders();
  int DoReadTunnelHeadersComplete(int result);
  int DoSOCKSConnect();
  int DoSOCKSConnectComplete(int result);
  int DoSSLConnect();
  int DoSSLConnectComplete(int result);
  int DoReadWrite(int result);

  GURL ProxyAuthOrigin() const;
  int HandleAuthChallenge(const HttpResponseHeaders* headers);
  void DoAuthRequired();
  void DoRestartWithAuth();

  int HandleCertificateError(int result);

  SSLConfigService* ssl_config_service() const;
  ProxyService* proxy_service() const;

  BoundNetLog net_log_;

  GURL url_;
  int max_pending_send_allowed_;
  scoped_refptr<URLRequestContext> context_;

  UserDataMap user_data_;

  State next_state_;
  HostResolver* host_resolver_;
  CertVerifier* cert_verifier_;
  HttpAuthHandlerFactory* http_auth_handler_factory_;
  ClientSocketFactory* factory_;

  ProxyMode proxy_mode_;

  GURL proxy_url_;
  ProxyService::PacRequest* pac_request_;
  ProxyInfo proxy_info_;

  HttpAuthCache auth_cache_;
  scoped_ptr<HttpAuthHandler> auth_handler_;
  HttpAuth::Identity auth_identity_;
  scoped_refptr<AuthChallengeInfo> auth_info_;

  scoped_refptr<RequestHeaders> tunnel_request_headers_;
  size_t tunnel_request_headers_bytes_sent_;
  scoped_refptr<ResponseHeaders> tunnel_response_headers_;
  int tunnel_response_headers_capacity_;
  int tunnel_response_headers_len_;

  scoped_ptr<SingleRequestHostResolver> resolver_;
  AddressList addresses_;
  scoped_ptr<ClientSocket> socket_;

  SSLConfig ssl_config_;

  CompletionCallbackImpl<SocketStream> io_callback_;
  CompletionCallbackImpl<SocketStream> read_callback_;
  CompletionCallbackImpl<SocketStream> write_callback_;

  scoped_refptr<IOBuffer> read_buf_;
  int read_buf_size_;

  
  
  
  
  
  
  scoped_refptr<IOBuffer> write_buf_;
  scoped_refptr<DrainableIOBuffer> current_write_buf_;
  int write_buf_offset_;
  int write_buf_size_;
  PendingDataQueue pending_write_bufs_;

  bool closing_;
  bool server_closed_;

  scoped_ptr<SocketStreamMetrics> metrics_;

  DISALLOW_COPY_AND_ASSIGN(SocketStream);
};

}  

#endif  
