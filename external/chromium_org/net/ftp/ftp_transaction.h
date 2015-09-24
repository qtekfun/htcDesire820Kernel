// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_FTP_FTP_TRANSACTION_H_
#define NET_FTP_FTP_TRANSACTION_H_

#include "net/base/completion_callback.h"
#include "net/base/io_buffer.h"
#include "net/base/load_states.h"
#include "net/base/net_export.h"

namespace net {

class AuthCredentials;
class FtpResponseInfo;
class FtpRequestInfo;
class BoundNetLog;

class NET_EXPORT_PRIVATE FtpTransaction {
 public:
  
  virtual ~FtpTransaction() {}

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int Start(const FtpRequestInfo* request_info,
                    const CompletionCallback& callback,
                    const BoundNetLog& net_log) = 0;

  
  virtual int RestartWithAuth(const AuthCredentials& credentials,
                              const CompletionCallback& callback) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int Read(IOBuffer* buf,
                   int buf_len,
                   const CompletionCallback& callback) = 0;

  
  
  virtual const FtpResponseInfo* GetResponseInfo() const = 0;

  
  virtual LoadState GetLoadState() const = 0;

  
  
  virtual uint64 GetUploadProgress() const = 0;
};

}  

#endif  
