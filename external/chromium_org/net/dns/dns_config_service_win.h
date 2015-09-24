// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DNS_DNS_CONFIG_SERVICE_WIN_H_
#define NET_DNS_DNS_CONFIG_SERVICE_WIN_H_

#include <winsock2.h>
#include <iphlpapi.h>

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "net/base/net_export.h"
#include "net/dns/dns_config_service.h"


namespace net {

namespace internal {

bool NET_EXPORT_PRIVATE ParseSearchList(const base::string16& value,
                                        std::vector<std::string>* out);

struct NET_EXPORT_PRIVATE DnsSystemSettings {
  
  struct RegString {
    bool set;
    base::string16 value;
  };

  struct RegDword {
    bool set;
    DWORD value;
  };

  struct DevolutionSetting {
    
    RegDword enabled;
    
    RegDword level;
  };

  
  
  scoped_ptr_malloc<IP_ADAPTER_ADDRESSES> addresses;

  
  RegString policy_search_list;
  
  RegString tcpip_search_list;
  
  RegString tcpip_domain;
  
  RegString primary_dns_suffix;

  
  DevolutionSetting policy_devolution;
  
  DevolutionSetting dnscache_devolution;
  
  DevolutionSetting tcpip_devolution;

  
  RegDword append_to_multi_label_name;

  
  
  bool have_name_resolution_policy;
};

enum ConfigParseWinResult {
  CONFIG_PARSE_WIN_OK = 0,
  CONFIG_PARSE_WIN_READ_IPHELPER,
  CONFIG_PARSE_WIN_READ_POLICY_SEARCHLIST,
  CONFIG_PARSE_WIN_READ_TCPIP_SEARCHLIST,
  CONFIG_PARSE_WIN_READ_DOMAIN,
  CONFIG_PARSE_WIN_READ_POLICY_DEVOLUTION,
  CONFIG_PARSE_WIN_READ_DNSCACHE_DEVOLUTION,
  CONFIG_PARSE_WIN_READ_TCPIP_DEVOLUTION,
  CONFIG_PARSE_WIN_READ_APPEND_MULTILABEL,
  CONFIG_PARSE_WIN_READ_PRIMARY_SUFFIX,
  CONFIG_PARSE_WIN_BAD_ADDRESS,
  CONFIG_PARSE_WIN_NO_NAMESERVERS,
  CONFIG_PARSE_WIN_UNHANDLED_OPTIONS,
  CONFIG_PARSE_WIN_MAX  
};

ConfigParseWinResult NET_EXPORT_PRIVATE ConvertSettingsToDnsConfig(
    const DnsSystemSettings& settings,
    DnsConfig* dns_config);

class NET_EXPORT_PRIVATE DnsConfigServiceWin : public DnsConfigService {
 public:
  DnsConfigServiceWin();
  virtual ~DnsConfigServiceWin();

 private:
  class Watcher;
  class ConfigReader;
  class HostsReader;

  
  virtual void ReadNow() OVERRIDE;
  virtual bool StartWatching() OVERRIDE;

  void OnConfigChanged(bool succeeded);
  void OnHostsChanged(bool succeeded);

  scoped_ptr<Watcher> watcher_;
  scoped_refptr<ConfigReader> config_reader_;
  scoped_refptr<HostsReader> hosts_reader_;

  DISALLOW_COPY_AND_ASSIGN(DnsConfigServiceWin);
};

}  

}  

#endif  

