// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DNS_DNS_HOSTS_H_
#define NET_DNS_DNS_HOSTS_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/files/file_path.h"
#include "net/base/address_family.h"
#include "net/base/net_export.h"
#include "net/base/net_util.h"  

namespace net {
  typedef std::pair<std::string, AddressFamily> DnsHostsKey;
};

namespace BASE_HASH_NAMESPACE {
#if defined(COMPILER_GCC)

template<>
struct hash<net::DnsHostsKey> {
  std::size_t operator()(const net::DnsHostsKey& key) const {
    hash<base::StringPiece> string_piece_hash;
    return string_piece_hash(key.first) + key.second;
  }
};

#elif defined(COMPILER_MSVC)

inline size_t hash_value(const net::DnsHostsKey& key) {
  return hash_value(key.first) + key.second;
}

#endif  

}  

namespace net {

#if !defined(OS_ANDROID)
typedef base::hash_map<DnsHostsKey, IPAddressNumber> DnsHosts;
#else
typedef std::map<DnsHostsKey, IPAddressNumber> DnsHosts;
#endif

void NET_EXPORT_PRIVATE ParseHosts(const std::string& contents,
                                   DnsHosts* dns_hosts);

bool NET_EXPORT_PRIVATE ParseHostsFile(const base::FilePath& path,
                                       DnsHosts* dns_hosts);



}  

#endif  

