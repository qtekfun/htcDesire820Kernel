// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_WEBSOCKETS_WEBSOCKET_BASIC_HANDSHAKE_STREAM_H_
#define NET_WEBSOCKETS_WEBSOCKET_BASIC_HANDSHAKE_STREAM_H_

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"
#include "net/http/http_basic_state.h"
#include "net/websockets/websocket_handshake_stream_base.h"

namespace net {

class ClientSocketHandle;
class HttpResponseHeaders;
class HttpResponseInfo;
class HttpStreamParser;

class NET_EXPORT_PRIVATE WebSocketBasicHandshakeStream
    : public WebSocketHandshakeStreamBase {
 public:
  WebSocketBasicHandshakeStream(
      scoped_ptr<ClientSocketHandle> connection,
      bool using_proxy,
      std::vector<std::string> requested_sub_protocols,
      std::vector<std::string> requested_extensions);

  virtual ~WebSocketBasicHandshakeStream();

  
  virtual int InitializeStream(const HttpRequestInfo* request_info,
                               RequestPriority priority,
                               const BoundNetLog& net_log,
                               const CompletionCallback& callback) OVERRIDE;
  virtual int SendRequest(const HttpRequestHeaders& request_headers,
                          HttpResponseInfo* response,
                          const CompletionCallback& callback) OVERRIDE;
  virtual int ReadResponseHeaders(const CompletionCallback& callback) OVERRIDE;
  virtual const HttpResponseInfo* GetResponseInfo() const OVERRIDE;
  virtual int ReadResponseBody(IOBuffer* buf,
                               int buf_len,
                               const CompletionCallback& callback) OVERRIDE;
  virtual void Close(bool not_reusable) OVERRIDE;
  virtual bool IsResponseBodyComplete() const OVERRIDE;
  virtual bool CanFindEndOfResponse() const OVERRIDE;
  virtual bool IsConnectionReused() const OVERRIDE;
  virtual void SetConnectionReused() OVERRIDE;
  virtual bool IsConnectionReusable() const OVERRIDE;
  virtual int64 GetTotalReceivedBytes() const OVERRIDE;
  virtual bool GetLoadTimingInfo(LoadTimingInfo* load_timing_info) const
      OVERRIDE;
  virtual void GetSSLInfo(SSLInfo* ssl_info) OVERRIDE;
  virtual void GetSSLCertRequestInfo(
      SSLCertRequestInfo* cert_request_info) OVERRIDE;
  virtual bool IsSpdyHttpStream() const OVERRIDE;
  virtual void Drain(HttpNetworkSession* session) OVERRIDE;
  virtual void SetPriority(RequestPriority priority) OVERRIDE;

  
  
  
  
  virtual scoped_ptr<WebSocketStream> Upgrade() OVERRIDE;

  
  
  
  void SetWebSocketKeyForTesting(const std::string& key);

 private:
  
  
  void ReadResponseHeadersCallback(const CompletionCallback& callback,
                                   int result);

  
  
  int ValidateResponse();

  
  
  int ValidateUpgradeResponse(
      const scoped_refptr<HttpResponseHeaders>& headers);

  HttpStreamParser* parser() const { return state_.parser(); }

  
  HttpBasicState state_;

  
  HttpResponseInfo* http_response_info_;

  
  
  scoped_ptr<std::string> handshake_challenge_for_testing_;

  
  std::string handshake_challenge_response_;

  
  std::vector<std::string> requested_sub_protocols_;

  
  std::vector<std::string> requested_extensions_;

  
  std::string sub_protocol_;

  
  std::string extensions_;

  DISALLOW_COPY_AND_ASSIGN(WebSocketBasicHandshakeStream);
};

}  

#endif  
