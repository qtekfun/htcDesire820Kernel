// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_PIPELINED_STREAM_H_
#define NET_HTTP_HTTP_PIPELINED_STREAM_H_

#include <string>

#include "base/basictypes.h"
#include "net/base/net_log.h"
#include "net/http/http_stream.h"
#include "net/socket/ssl_client_socket.h"

namespace net {

class BoundNetLog;
class HttpPipelinedConnectionImpl;
class HttpResponseInfo;
class HttpRequestHeaders;
struct HttpRequestInfo;
class IOBuffer;
class ProxyInfo;
struct SSLConfig;

class HttpPipelinedStream : public HttpStream {
 public:
  HttpPipelinedStream(HttpPipelinedConnectionImpl* pipeline,
                      int pipeline_id);
  virtual ~HttpPipelinedStream();

  
  virtual int InitializeStream(const HttpRequestInfo* request_info,
                               RequestPriority priority,
                               const BoundNetLog& net_log,
                               const CompletionCallback& callback) OVERRIDE;

  virtual int SendRequest(const HttpRequestHeaders& headers,
                          HttpResponseInfo* response,
                          const CompletionCallback& callback) OVERRIDE;

  virtual UploadProgress GetUploadProgress() const OVERRIDE;

  virtual int ReadResponseHeaders(const CompletionCallback& callback) OVERRIDE;

  virtual const HttpResponseInfo* GetResponseInfo() const OVERRIDE;

  virtual int ReadResponseBody(IOBuffer* buf, int buf_len,
                               const CompletionCallback& callback) OVERRIDE;

  virtual void Close(bool not_reusable) OVERRIDE;

  virtual HttpStream* RenewStreamForAuth() OVERRIDE;

  virtual bool IsResponseBodyComplete() const OVERRIDE;

  virtual bool CanFindEndOfResponse() const OVERRIDE;

  virtual bool IsConnectionReused() const OVERRIDE;

  virtual void SetConnectionReused() OVERRIDE;

  virtual bool IsConnectionReusable() const OVERRIDE;

  virtual int64 GetTotalReceivedBytes() const OVERRIDE;

  virtual bool GetLoadTimingInfo(
      LoadTimingInfo* load_timing_info) const OVERRIDE;

  virtual void GetSSLInfo(SSLInfo* ssl_info) OVERRIDE;

  virtual void GetSSLCertRequestInfo(
      SSLCertRequestInfo* cert_request_info) OVERRIDE;

  virtual bool IsSpdyHttpStream() const OVERRIDE;

  virtual void Drain(HttpNetworkSession* session) OVERRIDE;

  virtual void SetPriority(RequestPriority priority) OVERRIDE;

  
  const SSLConfig& used_ssl_config() const;

  
  const ProxyInfo& used_proxy_info() const;

  
  const BoundNetLog& net_log() const;

  
  bool was_npn_negotiated() const;

  
  NextProto protocol_negotiated() const;

 private:
  HttpPipelinedConnectionImpl* pipeline_;

  int pipeline_id_;

  const HttpRequestInfo* request_info_;

  DISALLOW_COPY_AND_ASSIGN(HttpPipelinedStream);
};

}  

#endif  
