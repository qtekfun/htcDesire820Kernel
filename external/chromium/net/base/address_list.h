// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_ADDRESS_LIST_H_
#define NET_BASE_ADDRESS_LIST_H_
#pragma once

#include <string>

#include "base/memory/ref_counted.h"
#include "net/base/net_util.h"

struct addrinfo;

namespace net {

class AddressList {
 public:
  
  AddressList();

  
  
  
  AddressList(const IPAddressNumber& address, int port, bool canonicalize_name);

  AddressList(const AddressList& addresslist);
  ~AddressList();
  AddressList& operator=(const AddressList& addresslist);

  
  
  
  
  void Adopt(struct addrinfo* head);

  
  
  
  void Copy(const struct addrinfo* head, bool recursive);

  
  
  void Append(const struct addrinfo* head);

  
  
  void SetPort(int port);

  
  
  
  int GetPort() const;

  
  
  
  void SetFrom(const AddressList& src, int port);

  
  
  
  
  
  bool GetCanonicalName(std::string* canonical_name) const;

  
  
  void Reset();

  
  const struct addrinfo* head() const;

  
  
  
  
  static AddressList* CreateAddressListFromSockaddr(
      const struct sockaddr* address,
      socklen_t address_length,
      int socket_type,
      int protocol);

 private:
  struct Data;

  explicit AddressList(Data* data);

  scoped_refptr<Data> data_;
};

}  

#endif  
