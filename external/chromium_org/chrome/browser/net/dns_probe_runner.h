// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_DNS_PROBE_RUNNER_H_
#define CHROME_BROWSER_NET_DNS_PROBE_RUNNER_H_

#include "base/basictypes.h"
#include "base/bind.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"

namespace net {
class DnsClient;
class DnsResponse;
class DnsTransaction;
}

namespace chrome_browser_net {

class DnsProbeRunner {
 public:
  static const char* kKnownGoodHostname;

  
  enum Result {
    UNKNOWN,
    CORRECT,     
    INCORRECT,   
    FAILING,     
    UNREACHABLE  
  };

  DnsProbeRunner();
  ~DnsProbeRunner();

  
  
  
  void SetClient(scoped_ptr<net::DnsClient> client);

  
  
  
  
  
  void RunProbe(const base::Closure& callback);

  
  
  bool IsRunning() const;

  
  Result result() const { return result_; }

 private:
  void OnTransactionComplete(net::DnsTransaction* transaction,
                             int net_error,
                             const net::DnsResponse* response);
  void CallCallback();

  base::WeakPtrFactory<DnsProbeRunner> weak_factory_;

  scoped_ptr<net::DnsClient> client_;

  
  
  base::Closure callback_;

  
  
  scoped_ptr<net::DnsTransaction> transaction_;

  Result result_;

  DISALLOW_COPY_AND_ASSIGN(DnsProbeRunner);
};

}  

#endif  
