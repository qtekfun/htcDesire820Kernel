// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DNS_DNS_TRANSACTION_H_
#define NET_DNS_DNS_TRANSACTION_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"

namespace net {

class BoundNetLog;
class DnsResponse;
class DnsSession;

class NET_EXPORT_PRIVATE DnsTransaction {
 public:
  virtual ~DnsTransaction() {}

  
  virtual const std::string& GetHostname() const = 0;

  
  virtual uint16 GetType() const = 0;

  
  virtual void Start() = 0;
};

class NET_EXPORT_PRIVATE DnsTransactionFactory {
 public:
  
  
  
  typedef base::Callback<void(DnsTransaction* transaction,
                              int neterror,
                              const DnsResponse* response)> CallbackType;

  virtual ~DnsTransactionFactory() {}

  
  
  
  
  
  
  
  virtual scoped_ptr<DnsTransaction> CreateTransaction(
      const std::string& hostname,
      uint16 qtype,
      const CallbackType& callback,
      const BoundNetLog& net_log) WARN_UNUSED_RESULT = 0;

  
  
  static scoped_ptr<DnsTransactionFactory> CreateFactory(
      DnsSession* session) WARN_UNUSED_RESULT;
};

}  

#endif  

