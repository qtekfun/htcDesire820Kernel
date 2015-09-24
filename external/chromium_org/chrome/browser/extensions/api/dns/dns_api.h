// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_DNS_DNS_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_DNS_DNS_API_H_

#include <string>

#include "chrome/browser/io_thread.h"
#include "extensions/browser/extension_function.h"
#include "net/base/address_list.h"
#include "net/base/completion_callback.h"
#include "net/dns/host_resolver.h"

class IOThread;

namespace extensions {

class DnsResolveFunction : public AsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("dns.resolve", DNS_RESOLVE)

  DnsResolveFunction();

 protected:
  virtual ~DnsResolveFunction();

  
  virtual bool RunImpl() OVERRIDE;

  void WorkOnIOThread();
  void RespondOnUIThread();

 private:
  void OnLookupFinished(int result);

  std::string hostname_;

  bool response_;  

  
  
  
  IOThread* io_thread_;

  scoped_ptr<net::HostResolver::RequestHandle> request_handle_;
  scoped_ptr<net::AddressList> addresses_;
};

}  

#endif  
