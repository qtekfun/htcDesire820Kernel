// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_TRANSACTION_FACTORY_H__
#define NET_HTTP_HTTP_TRANSACTION_FACTORY_H__
#pragma once

#include "base/memory/scoped_ptr.h"

namespace net {

class HttpCache;
class HttpNetworkSession;
class HttpTransaction;


class HttpTransactionFactory {
 public:
  virtual ~HttpTransactionFactory() {}

  
  
  virtual int CreateTransaction(scoped_ptr<HttpTransaction>* trans) = 0;

  
  virtual HttpCache* GetCache() = 0;

  
  virtual HttpNetworkSession* GetSession() = 0;

  
  
  virtual void Suspend(bool suspend) = 0;
};

}  

#endif  
