// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_FTP_FTP_TRANSACTION_FACTORY_H_
#define NET_FTP_FTP_TRANSACTION_FACTORY_H_

#include "net/base/net_export.h"

namespace net {

class FtpTransaction;

class NET_EXPORT FtpTransactionFactory {
 public:
  virtual ~FtpTransactionFactory() {}

  
  virtual FtpTransaction* CreateTransaction() = 0;

  
  
  virtual void Suspend(bool suspend) = 0;
};

}  

#endif  
