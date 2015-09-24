// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_STREAM_H_
#define NET_HTTP_HTTP_STREAM_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "net/base/completion_callback.h"

namespace net {

class BoundNetLog;
class HttpRequestHeaders;
struct HttpRequestInfo;
class HttpResponseInfo;
class IOBuffer;
class SSLCertRequestInfo;
class SSLInfo;
class UploadDataStream;

class HttpStream {
 public:
  HttpStream() {}
  virtual ~HttpStream() {}

  
  
  virtual int InitializeStream(const HttpRequestInfo* request_info,
                               const BoundNetLog& net_log,
                               CompletionCallback* callback) = 0;

  
  
  
  
  
  virtual int SendRequest(const HttpRequestHeaders& request_headers,
                          UploadDataStream* request_body,
                          HttpResponseInfo* response,
                          CompletionCallback* callback) = 0;

  
  virtual uint64 GetUploadProgress() const = 0;

  
  
  
  
  
  virtual int ReadResponseHeaders(CompletionCallback* callback) = 0;

  
  virtual const HttpResponseInfo* GetResponseInfo() const = 0;

  
  
  
  
  
  
  
  
  
  
  virtual int ReadResponseBody(IOBuffer* buf, int buf_len,
                               CompletionCallback* callback) = 0;

  
  
  
  
  
  
  
  
  
  virtual void Close(bool not_reusable) = 0;

  
  
  
  
  
  virtual HttpStream* RenewStreamForAuth() = 0;

  
  virtual bool IsResponseBodyComplete() const = 0;

  
  
  
  
  virtual bool CanFindEndOfResponse() const = 0;

  
  
  
  virtual bool IsMoreDataBuffered() const = 0;

  
  
  
  
  virtual bool IsConnectionReused() const = 0;
  virtual void SetConnectionReused() = 0;

  
  
  virtual bool IsConnectionReusable() const = 0;

  
  
  
  virtual void GetSSLInfo(SSLInfo* ssl_info) = 0;

  
  
  
  virtual void GetSSLCertRequestInfo(SSLCertRequestInfo* cert_request_info) = 0;

  
  
  virtual bool IsSpdyHttpStream() const = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(HttpStream);
};

}  

#endif  
