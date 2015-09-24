// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_TRANSACTION_H_
#define NET_HTTP_HTTP_TRANSACTION_H_
#pragma once

#include "base/string16.h"
#include "net/base/completion_callback.h"
#include "net/base/load_states.h"

namespace net {

class BoundNetLog;
struct HttpRequestInfo;
class HttpResponseInfo;
class IOBuffer;
class X509Certificate;
class SSLHostInfo;

class HttpTransaction {
 public:
  
  virtual ~HttpTransaction() {}

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int Start(const HttpRequestInfo* request_info,
                    CompletionCallback* callback,
                    const BoundNetLog& net_log) = 0;

  
  
  
  
  
  
  
  
  
  
  virtual int RestartIgnoringLastError(CompletionCallback* callback) = 0;

  
  virtual int RestartWithCertificate(X509Certificate* client_cert,
                                     CompletionCallback* callback) = 0;

  
  virtual int RestartWithAuth(const string16& username,
                              const string16& password,
                              CompletionCallback* callback) = 0;

  
  
  
  
  
  
  virtual bool IsReadyToRestartForAuth() = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int Read(IOBuffer* buf, int buf_len,
                   CompletionCallback* callback) = 0;

  
  virtual void StopCaching() = 0;

  
  
  virtual const HttpResponseInfo* GetResponseInfo() const = 0;

  
  virtual LoadState GetLoadState() const = 0;

  
  
  virtual uint64 GetUploadProgress() const = 0;

  
  
  
  virtual void SetSSLHostInfo(SSLHostInfo*) { };
};

}  

#endif  
