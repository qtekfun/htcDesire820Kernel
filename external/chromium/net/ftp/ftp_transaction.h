// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_FTP_FTP_TRANSACTION_H_
#define NET_FTP_FTP_TRANSACTION_H_
#pragma once

#include "base/string16.h"
#include "net/base/completion_callback.h"
#include "net/base/io_buffer.h"
#include "net/base/load_states.h"

namespace net {

class FtpResponseInfo;
class FtpRequestInfo;
class BoundNetLog;

class FtpTransaction {
 public:
  
  virtual ~FtpTransaction() {}

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int Start(const FtpRequestInfo* request_info,
                    CompletionCallback* callback,
                    const BoundNetLog& net_log) = 0;

  
  virtual int RestartWithAuth(const string16& username,
                              const string16& password,
                              CompletionCallback* callback) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int Read(IOBuffer* buf,
                   int buf_len,
                   CompletionCallback* callback) = 0;

  
  
  virtual const FtpResponseInfo* GetResponseInfo() const = 0;

  
  virtual LoadState GetLoadState() const = 0;

  
  
  virtual uint64 GetUploadProgress() const = 0;
};

}  

#endif  
