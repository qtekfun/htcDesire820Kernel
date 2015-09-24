// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DNS_ADDRESS_SORTER_POSIX_H_
#define NET_DNS_ADDRESS_SORTER_POSIX_H_

#include <map>
#include <vector>

#include "base/threading/non_thread_safe.h"
#include "net/base/address_list.h"
#include "net/base/net_errors.h"
#include "net/base/net_export.h"
#include "net/base/net_util.h"
#include "net/base/network_change_notifier.h"
#include "net/dns/address_sorter.h"

namespace net {

class ClientSocketFactory;

class NET_EXPORT_PRIVATE AddressSorterPosix
    : public AddressSorter,
      public base::NonThreadSafe,
      public NetworkChangeNotifier::IPAddressObserver {
 public:
  
  struct PolicyEntry {
    
    unsigned char prefix[kIPv6AddressSize];
    unsigned prefix_length;
    unsigned value;
  };

  typedef std::vector<PolicyEntry> PolicyTable;

  enum AddressScope {
    SCOPE_UNDEFINED = 0,
    SCOPE_NODELOCAL = 1,
    SCOPE_LINKLOCAL = 2,
    SCOPE_SITELOCAL = 5,
    SCOPE_ORGLOCAL = 8,
    SCOPE_GLOBAL = 14,
  };

  struct SourceAddressInfo {
    
    AddressScope scope;
    unsigned label;

    
    unsigned prefix_length;
    bool deprecated;  
    bool home;        
    bool native;
  };

  typedef std::map<IPAddressNumber, SourceAddressInfo> SourceAddressMap;

  explicit AddressSorterPosix(ClientSocketFactory* socket_factory);
  virtual ~AddressSorterPosix();

  
  virtual void Sort(const AddressList& list,
                    const CallbackType& callback) const OVERRIDE;

 private:
  friend class AddressSorterPosixTest;

  
  virtual void OnIPAddressChanged() OVERRIDE;

  
  void FillPolicy(const IPAddressNumber& address,
                  SourceAddressInfo* info) const;

  
  
  mutable SourceAddressMap source_map_;

  ClientSocketFactory* socket_factory_;
  PolicyTable precedence_table_;
  PolicyTable label_table_;
  PolicyTable ipv4_scope_table_;

  DISALLOW_COPY_AND_ASSIGN(AddressSorterPosix);
};

}  

#endif  
