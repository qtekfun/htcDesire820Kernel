// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_DNS_PROBE_SERVICE_H_
#define CHROME_BROWSER_NET_DNS_PROBE_SERVICE_H_

#include <vector>

#include "base/basictypes.h"
#include "base/bind.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "chrome/browser/net/dns_probe_runner.h"
#include "chrome/common/net/net_error_info.h"
#include "net/base/network_change_notifier.h"

namespace net {
class DnsClient;
struct DnsConfig;
}

namespace chrome_browser_net {

class DnsProbeService : public net::NetworkChangeNotifier::DNSObserver {
 public:
  typedef base::Callback<void(chrome_common_net::DnsProbeStatus result)>
      ProbeCallback;

  DnsProbeService();
  virtual ~DnsProbeService();

  virtual void ProbeDns(const ProbeCallback& callback);

  
  virtual void OnDNSChanged() OVERRIDE;

  void SetSystemClientForTesting(scoped_ptr<net::DnsClient> system_client);
  void SetPublicClientForTesting(scoped_ptr<net::DnsClient> public_client);
  void ClearCachedResultForTesting();

 private:
  enum State {
    STATE_NO_RESULT,
    STATE_PROBE_RUNNING,
    STATE_RESULT_CACHED,
  };

  void SetSystemClientToCurrentConfig();
  void SetPublicClientToGooglePublicDns();

  
  void StartProbes();
  void OnProbeComplete();
  
  void CallCallbacks();
  
  void ClearCachedResult();

  bool CachedResultIsExpired() const;

  State state_;
  std::vector<ProbeCallback> pending_callbacks_;
  base::Time probe_start_time_;
  chrome_common_net::DnsProbeStatus cached_result_;

  
  DnsProbeRunner system_runner_;
  DnsProbeRunner public_runner_;

  DISALLOW_COPY_AND_ASSIGN(DnsProbeService);
};

}  

#endif  
