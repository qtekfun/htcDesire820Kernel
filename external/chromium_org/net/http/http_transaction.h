// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_TRANSACTION_H_
#define NET_HTTP_HTTP_TRANSACTION_H_

#include "net/base/completion_callback.h"
#include "net/base/load_states.h"
#include "net/base/net_export.h"
#include "net/base/request_priority.h"
#include "net/base/upload_progress.h"
#include "net/websockets/websocket_handshake_stream_base.h"

namespace net {

class AuthCredentials;
class BoundNetLog;
class HttpRequestHeaders;
struct HttpRequestInfo;
class HttpResponseInfo;
class IOBuffer;
struct LoadTimingInfo;
class X509Certificate;

class NET_EXPORT_PRIVATE HttpTransaction {
 public:
  
  virtual ~HttpTransaction() {}

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int Start(const HttpRequestInfo* request_info,
                    const CompletionCallback& callback,
                    const BoundNetLog& net_log) = 0;

  
  
  
  
  
  
  
  
  
  
  virtual int RestartIgnoringLastError(const CompletionCallback& callback) = 0;

  
  virtual int RestartWithCertificate(X509Certificate* client_cert,
                                     const CompletionCallback& callback) = 0;

  
  virtual int RestartWithAuth(const AuthCredentials& credentials,
                              const CompletionCallback& callback) = 0;

  
  
  
  
  
  
  virtual bool IsReadyToRestartForAuth() = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int Read(IOBuffer* buf, int buf_len,
                   const CompletionCallback& callback) = 0;

  
  virtual void StopCaching() = 0;

  
  
  
  
  
  
  virtual bool GetFullRequestHeaders(HttpRequestHeaders* headers) const = 0;

  
  
  
  
  
  
  
  virtual void DoneReading() = 0;

  
  
  virtual const HttpResponseInfo* GetResponseInfo() const = 0;

  
  virtual LoadState GetLoadState() const = 0;

  
  
  virtual UploadProgress GetUploadProgress() const = 0;

  
  
  
  
  
  virtual bool GetLoadTimingInfo(LoadTimingInfo* load_timing_info) const = 0;

  
  virtual void SetPriority(RequestPriority priority) = 0;

  
  
  
  virtual void SetWebSocketHandshakeStreamCreateHelper(
      WebSocketHandshakeStreamBase::CreateHelper* create_helper) = 0;
};

}  

#endif  
