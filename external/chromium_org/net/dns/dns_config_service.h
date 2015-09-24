// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DNS_DNS_CONFIG_SERVICE_H_
#define NET_DNS_DNS_CONFIG_SERVICE_H_

#include <map>
#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "net/base/address_list.h"
#include "net/base/ip_endpoint.h"  
#include "net/base/net_export.h"
#include "net/dns/dns_hosts.h"

namespace base {
class Value;
}

namespace net {

const unsigned kDnsTimeoutSeconds = 1;

struct NET_EXPORT_PRIVATE DnsConfig {
  DnsConfig();
  virtual ~DnsConfig();

  bool Equals(const DnsConfig& d) const;

  bool EqualsIgnoreHosts(const DnsConfig& d) const;

  void CopyIgnoreHosts(const DnsConfig& src);

  
  
  
  base::Value* ToValue() const;

  bool IsValid() const {
    return !nameservers.empty();
  }

  
  std::vector<IPEndPoint> nameservers;
  
  
  std::vector<std::string> search;

  DnsHosts hosts;

  
  
  bool unhandled_options;

  
  
  bool append_to_multi_label_name;

  
  
  bool randomize_ports;

  

  
  int ndots;
  
  base::TimeDelta timeout;
  
  int attempts;
  
  bool rotate;
  
  bool edns0;

  
  
  
  bool use_local_ipv6;
};


class NET_EXPORT_PRIVATE DnsConfigService
    : NON_EXPORTED_BASE(public base::NonThreadSafe) {
 public:
  
  
  typedef base::Callback<void(const DnsConfig& config)> CallbackType;

  
  static scoped_ptr<DnsConfigService> CreateSystemService();

  DnsConfigService();
  virtual ~DnsConfigService();

  
  
  void ReadConfig(const CallbackType& callback);

  
  
  
  
  void WatchConfig(const CallbackType& callback);

 protected:
  enum WatchStatus {
    DNS_CONFIG_WATCH_STARTED = 0,
    DNS_CONFIG_WATCH_FAILED_TO_START_CONFIG,
    DNS_CONFIG_WATCH_FAILED_TO_START_HOSTS,
    DNS_CONFIG_WATCH_FAILED_CONFIG,
    DNS_CONFIG_WATCH_FAILED_HOSTS,
    DNS_CONFIG_WATCH_MAX,
  };

 
  virtual void ReadNow() = 0;
  
  virtual bool StartWatching() = 0;

  
  void InvalidateConfig();
  
  void InvalidateHosts();

  
  void OnConfigRead(const DnsConfig& config);
  
  void OnHostsRead(const DnsHosts& hosts);

  void set_watch_failed(bool value) { watch_failed_ = value; }

 private:
  
  void StartTimer();
  void OnTimeout();
  
  void OnCompleteConfig();

  CallbackType callback_;

  DnsConfig dns_config_;

  
  
  bool watch_failed_;
  
  bool have_config_;
  bool have_hosts_;
  
  bool need_update_;
  
  
  bool last_sent_empty_;

  
  base::TimeTicks last_invalidate_config_time_;
  base::TimeTicks last_invalidate_hosts_time_;
  
  base::TimeTicks last_sent_empty_time_;

  
  base::OneShotTimer<DnsConfigService> timer_;

  DISALLOW_COPY_AND_ASSIGN(DnsConfigService);
};

}  

#endif  
