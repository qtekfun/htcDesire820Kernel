// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_STREAM_SOCKET_STREAM_H_
#define NET_SOCKET_STREAM_SOCKET_STREAM_H_

#include <deque>
#include <map>
#include <string>

#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/address_list.h"
#include "net/base/completion_callback.h"
#include "net/base/io_buffer.h"
#include "net/base/net_errors.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/base/privacy_mode.h"
#include "net/proxy/proxy_service.h"
#include "net/ssl/ssl_config_service.h"
#include "net/url_request/url_request.h"

namespace net {

class AuthChallengeInfo;
class CertVerifier;
class ClientSocketFactory;
class ClientSocketHandle;
class CookieOptions;
class HostResolver;
class HttpAuthController;
class SSLInfo;
class ServerBoundCertService;
class SingleRequestHostResolver;
class SocketStreamMetrics;
class TransportSecurityState;
class URLRequestContext;

class NET_EXPORT SocketStream
    : public base::RefCountedThreadSafe<SocketStream> {
 public:
  
  
  
  class UserData {
   public:
    UserData() {}
    virtual ~UserData() {}
  };

  class NET_EXPORT Delegate {
   public:
    virtual int OnStartOpenConnection(SocketStream* socket,
                                      const CompletionCallback& callback);

    
    
    
    
    
    virtual void OnConnected(SocketStream* socket,
                             int max_pending_send_allowed) = 0;

    
    virtual void OnSentData(SocketStream* socket,
                            int amount_sent) = 0;

    
    virtual void OnReceivedData(SocketStream* socket,
                                const char* data, int len) = 0;

    
    virtual void OnClose(SocketStream* socket) = 0;

    
    
    
    virtual void OnAuthRequired(SocketStream* socket,
                                AuthChallengeInfo* auth_info);

    
    
    
    virtual void OnSSLCertificateError(SocketStream* socket,
                                       const SSLInfo& ssl_info,
                                       bool fatal);

    
    
    
    virtual void OnError(const SocketStream* socket, int error) {}

    
    
    virtual bool CanGetCookies(SocketStream* socket, const GURL& url);

    
    
    virtual bool CanSetCookie(SocketStream* request,
                              const GURL& url,
                              const std::string& cookie_line,
                              CookieOptions* options);

   protected:
    virtual ~Delegate() {}
  };

  SocketStream(const GURL& url, Delegate* delegate);

  
  
  
  
  UserData* GetUserData(const void* key) const;
  void SetUserData(const void* key, UserData* data);

  const GURL& url() const { return url_; }
  bool is_secure() const;
  const AddressList& address_list() const { return addresses_; }
  Delegate* delegate() const { return delegate_; }
  int max_pending_send_allowed() const { return max_pending_send_allowed_; }

  URLRequestContext* context() { return context_; }
  
  
  void set_context(URLRequestContext* context);

  const SSLConfig& server_ssl_config() const { return server_ssl_config_; }
  PrivacyMode privacy_mode() const { return privacy_mode_; }
  void CheckPrivacyMode();

  BoundNetLog* net_log() { return &net_log_; }

  
  
  virtual void Connect();

  
  
  
  virtual bool SendData(const char* data, int len);

  
  
  virtual void Close();

  
  
  virtual void RestartWithAuth(const AuthCredentials& credentials);

  
  
  
  virtual void DetachDelegate();

  const ProxyServer& proxy_server() const;

  
  
  void SetClientSocketFactory(ClientSocketFactory* factory);

  
  
  void CancelWithError(int error);

  
  void CancelWithSSLError(const SSLInfo& ssl_info);

  
  
  
  void ContinueDespiteError();

 protected:
  friend class base::RefCountedThreadSafe<SocketStream>;
  virtual ~SocketStream();

  Delegate* delegate_;

 private:
  FRIEND_TEST_ALL_PREFIXES(SocketStreamTest, IOPending);
  FRIEND_TEST_ALL_PREFIXES(SocketStreamTest, SwitchAfterPending);
  FRIEND_TEST_ALL_PREFIXES(SocketStreamTest,
                           NullContextSocketStreamShouldNotCrash);

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
     virtual ~RequestHeaders();
  };

  class ResponseHeaders : public IOBuffer {
   public:
    ResponseHeaders();

    void SetDataOffset(size_t offset) { data_ = headers_.get() + offset; }
    char* headers() const { return headers_.get(); }
    void Reset() { headers_.reset(); }
    void Realloc(size_t new_size);

   private:
     virtual ~ResponseHeaders();

    scoped_ptr_malloc<char> headers_;
  };

  enum State {
    STATE_NONE,
    STATE_BEFORE_CONNECT,
    STATE_BEFORE_CONNECT_COMPLETE,
    STATE_RESOLVE_PROXY,
    STATE_RESOLVE_PROXY_COMPLETE,
    STATE_RESOLVE_HOST,
    STATE_RESOLVE_HOST_COMPLETE,
    STATE_RESOLVE_PROTOCOL,
    STATE_RESOLVE_PROTOCOL_COMPLETE,
    STATE_TCP_CONNECT,
    STATE_TCP_CONNECT_COMPLETE,
    STATE_GENERATE_PROXY_AUTH_TOKEN,
    STATE_GENERATE_PROXY_AUTH_TOKEN_COMPLETE,
    STATE_WRITE_TUNNEL_HEADERS,
    STATE_WRITE_TUNNEL_HEADERS_COMPLETE,
    STATE_READ_TUNNEL_HEADERS,
    STATE_READ_TUNNEL_HEADERS_COMPLETE,
    STATE_SOCKS_CONNECT,
    STATE_SOCKS_CONNECT_COMPLETE,
    STATE_SECURE_PROXY_CONNECT,
    STATE_SECURE_PROXY_CONNECT_COMPLETE,
    STATE_SECURE_PROXY_HANDLE_CERT_ERROR,
    STATE_SECURE_PROXY_HANDLE_CERT_ERROR_COMPLETE,
    STATE_SSL_CONNECT,
    STATE_SSL_CONNECT_COMPLETE,
    STATE_SSL_HANDLE_CERT_ERROR,
    STATE_SSL_HANDLE_CERT_ERROR_COMPLETE,
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

  
  void set_addresses(const AddressList& addresses);

  void DoClose();

  
  
  
  void Finish(int result);

  int DidEstablishConnection();
  int DidReceiveData(int result);
  
  
  
  
  
  
  void DidSendData(int result);

  void OnIOCompleted(int result);
  void OnReadCompleted(int result);
  void OnWriteCompleted(int result);

  void DoLoop(int result);

  int DoBeforeConnect();
  int DoBeforeConnectComplete(int result);
  int DoResolveProxy();
  int DoResolveProxyComplete(int result);
  int DoResolveHost();
  int DoResolveHostComplete(int result);
  int DoResolveProtocol(int result);
  int DoResolveProtocolComplete(int result);
  int DoTcpConnect(int result);
  int DoTcpConnectComplete(int result);
  int DoGenerateProxyAuthToken();
  int DoGenerateProxyAuthTokenComplete(int result);
  int DoWriteTunnelHeaders();
  int DoWriteTunnelHeadersComplete(int result);
  int DoReadTunnelHeaders();
  int DoReadTunnelHeadersComplete(int result);
  int DoSOCKSConnect();
  int DoSOCKSConnectComplete(int result);
  int DoSecureProxyConnect();
  int DoSecureProxyConnectComplete(int result);
  int DoSecureProxyHandleCertError(int result);
  int DoSecureProxyHandleCertErrorComplete(int result);
  int DoSSLConnect();
  int DoSSLConnectComplete(int result);
  int DoSSLHandleCertError(int result);
  int DoSSLHandleCertErrorComplete(int result);
  int DoReadWrite(int result);

  GURL ProxyAuthOrigin() const;
  int HandleAuthChallenge(const HttpResponseHeaders* headers);
  int HandleCertificateRequest(int result, SSLConfig* ssl_config);
  void DoAuthRequired();
  void DoRestartWithAuth();

  int HandleCertificateError(int result);
  int AllowCertErrorForReconnection(SSLConfig* ssl_config);

  
  size_t GetTotalSizeOfPendingWriteBufs() const;

  BoundNetLog net_log_;

  GURL url_;
  
  
  
  
  
  int max_pending_send_allowed_;
  URLRequestContext* context_;

  UserDataMap user_data_;

  State next_state_;
  ClientSocketFactory* factory_;

  ProxyMode proxy_mode_;

  GURL proxy_url_;
  ProxyService::PacRequest* pac_request_;
  ProxyInfo proxy_info_;

  scoped_refptr<HttpAuthController> proxy_auth_controller_;

  scoped_refptr<RequestHeaders> tunnel_request_headers_;
  size_t tunnel_request_headers_bytes_sent_;
  scoped_refptr<ResponseHeaders> tunnel_response_headers_;
  int tunnel_response_headers_capacity_;
  int tunnel_response_headers_len_;

  scoped_ptr<SingleRequestHostResolver> resolver_;
  AddressList addresses_;
  scoped_ptr<ClientSocketHandle> connection_;

  SSLConfig server_ssl_config_;
  SSLConfig proxy_ssl_config_;
  PrivacyMode privacy_mode_;

  CompletionCallback io_callback_;

  scoped_refptr<IOBuffer> read_buf_;
  int read_buf_size_;

  
  scoped_refptr<DrainableIOBuffer> current_write_buf_;
  
  
  bool waiting_for_write_completion_;
  PendingDataQueue pending_write_bufs_;

  bool closing_;
  bool server_closed_;

  scoped_ptr<SocketStreamMetrics> metrics_;

  DISALLOW_COPY_AND_ASSIGN(SocketStream);
};

}  

#endif  
