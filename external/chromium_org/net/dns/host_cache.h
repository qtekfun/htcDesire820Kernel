// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DNS_HOST_CACHE_H_
#define NET_DNS_HOST_CACHE_H_

#include <functional>
#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "net/base/address_family.h"
#include "net/base/address_list.h"
#include "net/base/expiring_cache.h"
#include "net/base/net_export.h"

namespace net {

class NET_EXPORT HostCache : NON_EXPORTED_BASE(public base::NonThreadSafe) {
 public:
  
  struct NET_EXPORT Entry {
    Entry(int error, const AddressList& addrlist, base::TimeDelta ttl);
    
    Entry(int error, const AddressList& addrlist);
    ~Entry();

    bool has_ttl() const { return ttl >= base::TimeDelta(); }

    
    int error;
    AddressList addrlist;
    
    base::TimeDelta ttl;
  };

  struct Key {
    Key(const std::string& hostname, AddressFamily address_family,
        HostResolverFlags host_resolver_flags)
        : hostname(hostname),
          address_family(address_family),
          host_resolver_flags(host_resolver_flags) {}

    bool operator<(const Key& other) const {
      
      
      
      if (address_family != other.address_family)
        return address_family < other.address_family;
      if (host_resolver_flags != other.host_resolver_flags)
        return host_resolver_flags < other.host_resolver_flags;
      return hostname < other.hostname;
    }

    std::string hostname;
    AddressFamily address_family;
    HostResolverFlags host_resolver_flags;
  };

  struct EvictionHandler {
    void Handle(const Key& key,
                const Entry& entry,
                const base::TimeTicks& expiration,
                const base::TimeTicks& now,
                bool onGet) const;
  };

  typedef ExpiringCache<Key, Entry, base::TimeTicks,
                        std::less<base::TimeTicks>,
                        EvictionHandler> EntryMap;

  
  explicit HostCache(size_t max_entries);

  ~HostCache();

  
  
  const Entry* Lookup(const Key& key, base::TimeTicks now);

  
  
  
  void Set(const Key& key,
           const Entry& entry,
           base::TimeTicks now,
           base::TimeDelta ttl);

  
  void clear();

  
  size_t size() const;

  
  size_t max_entries() const;

  const EntryMap& entries() const;

  
  static scoped_ptr<HostCache> CreateDefaultCache();

 private:
  FRIEND_TEST_ALL_PREFIXES(HostCacheTest, NoCache);

  
  bool caching_is_disabled() const {
    return entries_.max_entries() == 0;
  }

  
  
  EntryMap entries_;

  DISALLOW_COPY_AND_ASSIGN(HostCache);
};

}  

#endif  
