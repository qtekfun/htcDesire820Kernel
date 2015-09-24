// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_HOST_CACHE_H_
#define NET_BASE_HOST_CACHE_H_
#pragma once

#include <map>
#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/threading/non_thread_safe.h"
#include "base/time.h"
#include "net/base/address_family.h"
#include "net/base/address_list.h"

namespace net {

class HostCache : public base::NonThreadSafe {
 public:
  
  struct Entry : public base::RefCounted<Entry> {
    Entry(int error, const AddressList& addrlist, base::TimeTicks expiration);

    
    int error;
    AddressList addrlist;

    
    base::TimeTicks expiration;

   private:
    friend class base::RefCounted<Entry>;

    ~Entry();
  };

  struct Key {
    Key(const std::string& hostname, AddressFamily address_family,
        HostResolverFlags host_resolver_flags)
        : hostname(hostname),
          address_family(address_family),
          host_resolver_flags(host_resolver_flags) {}

    bool operator==(const Key& other) const {
      
      
      
      return (other.address_family == address_family &&
              other.host_resolver_flags == host_resolver_flags &&
              other.hostname == hostname);
    }

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

  typedef std::map<Key, scoped_refptr<Entry> > EntryMap;

  
  
  
  HostCache(size_t max_entries,
            base::TimeDelta success_entry_ttl,
            base::TimeDelta failure_entry_ttl);

  ~HostCache();

  
  
  const Entry* Lookup(const Key& key, base::TimeTicks now) const;

  
  
  
  
  Entry* Set(const Key& key,
             int error,
             const AddressList& addrlist,
             base::TimeTicks now);

  
  void clear();

  
  size_t size() const;

  
  size_t max_entries() const;

  base::TimeDelta success_entry_ttl() const;

  base::TimeDelta failure_entry_ttl() const;

  
  const EntryMap& entries() const;

 private:
  FRIEND_TEST_ALL_PREFIXES(HostCacheTest, Compact);
  FRIEND_TEST_ALL_PREFIXES(HostCacheTest, NoCache);

  
  static bool CanUseEntry(const Entry* entry, const base::TimeTicks now);

  
  
  void Compact(base::TimeTicks now, const Entry* pinned_entry);

  
  bool caching_is_disabled() const {
    return max_entries_ == 0;
  }

  
  size_t max_entries_;

  
  base::TimeDelta success_entry_ttl_;
  base::TimeDelta failure_entry_ttl_;

  
  
  EntryMap entries_;

  DISALLOW_COPY_AND_ASSIGN(HostCache);
};

}  

#endif  
