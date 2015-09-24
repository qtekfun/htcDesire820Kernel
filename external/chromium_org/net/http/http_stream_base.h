// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_STREAM_BASE_H_
#define NET_HTTP_HTTP_STREAM_BASE_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"
#include "net/base/request_priority.h"
#include "net/base/upload_progress.h"

namespace net {

class BoundNetLog;
class HttpNetworkSession;
class HttpRequestHeaders;
struct HttpRequestInfo;
class HttpResponseInfo;
class IOBuffer;
struct LoadTimingInfo;
class SSLCertRequestInfo;
class SSLInfo;

class NET_EXPORT_PRIVATE HttpStreamBase {
 public:
  HttpStreamBase() {}
  virtual ~HttpStreamBase() {}

  
  
  
  virtual int InitializeStream(const HttpRequestInfo* request_info,
                               RequestPriority priority,
                               const BoundNetLog& net_log,
                               const CompletionCallback& callback) = 0;

  
  
  
  
  
  virtual int SendRequest(const HttpRequestHeaders& request_headers,
                          HttpResponseInfo* response,
                          const CompletionCallback& callback) = 0;

  
  
  
  
  
  virtual int ReadResponseHeaders(const CompletionCallback& callback) = 0;

  
  virtual const HttpResponseInfo* GetResponseInfo() const = 0;

  
  
  
  
  
  
  
  
  
  
  virtual int ReadResponseBody(IOBuffer* buf, int buf_len,
                               const CompletionCallback& callback) = 0;

  
  
  
  
  
  
  
  
  
  virtual void Close(bool not_reusable) = 0;

  
  virtual bool IsResponseBodyComplete() const = 0;

  
  
  
  
  
  
  
  virtual bool CanFindEndOfResponse() const = 0;

  
  
  
  
  virtual bool IsConnectionReused() const = 0;
  virtual void SetConnectionReused() = 0;

  
  
  virtual bool IsConnectionReusable() const = 0;

  
  virtual int64 GetTotalReceivedBytes() const = 0;

  
  
  
  
  
  
  
  
  
  virtual bool GetLoadTimingInfo(LoadTimingInfo* load_timing_info) const = 0;

  
  
  
  virtual void GetSSLInfo(SSLInfo* ssl_info) = 0;

  
  
  
  virtual void GetSSLCertRequestInfo(SSLCertRequestInfo* cert_request_info) = 0;

  
  
  virtual bool IsSpdyHttpStream() const = 0;

  
  
  
  
  virtual void Drain(HttpNetworkSession* session) = 0;

  
  virtual void SetPriority(RequestPriority priority) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(HttpStreamBase);
};

}  

#endif  
