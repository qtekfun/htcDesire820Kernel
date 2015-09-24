// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_ADDRESS_LIST_H_
#define NET_BASE_ADDRESS_LIST_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "net/base/ip_endpoint.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/base/net_util.h"

struct addrinfo;

namespace net {

class NET_EXPORT AddressList
    : NON_EXPORTED_BASE(private std::vector<IPEndPoint>) {
 public:
  AddressList();
  ~AddressList();

  
  explicit AddressList(const IPEndPoint& endpoint);

  static AddressList CreateFromIPAddress(const IPAddressNumber& address,
                                         uint16 port);

  static AddressList CreateFromIPAddressList(
      const IPAddressList& addresses,
      const std::string& canonical_name);

  
  static AddressList CreateFromAddrinfo(const struct addrinfo* head);

  
  static AddressList CopyWithPort(const AddressList& list, uint16 port);

  
  const std::string& canonical_name() const {
    return canonical_name_;
  }

  void set_canonical_name(const std::string& canonical_name) {
    canonical_name_ = canonical_name;
  }

  
  void SetDefaultCanonicalName();

  
  
  
  NetLog::ParametersCallback CreateNetLogCallback() const;

  
  using std::vector<IPEndPoint>::size;
  using std::vector<IPEndPoint>::empty;
  using std::vector<IPEndPoint>::clear;
  using std::vector<IPEndPoint>::reserve;
  using std::vector<IPEndPoint>::capacity;
  using std::vector<IPEndPoint>::operator[];
  using std::vector<IPEndPoint>::front;
  using std::vector<IPEndPoint>::back;
  using std::vector<IPEndPoint>::push_back;
  using std::vector<IPEndPoint>::insert;
  using std::vector<IPEndPoint>::erase;
  using std::vector<IPEndPoint>::iterator;
  using std::vector<IPEndPoint>::const_iterator;
  using std::vector<IPEndPoint>::begin;
  using std::vector<IPEndPoint>::end;
  using std::vector<IPEndPoint>::rbegin;
  using std::vector<IPEndPoint>::rend;

 private:
  
  std::string canonical_name_;
};

}  

#endif  
