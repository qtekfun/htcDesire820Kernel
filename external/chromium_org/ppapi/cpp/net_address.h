// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_NET_ADDRESS_H_
#define PPAPI_CPP_NET_ADDRESS_H_

#include "ppapi/c/ppb_net_address.h"
#include "ppapi/cpp/pass_ref.h"
#include "ppapi/cpp/resource.h"
#include "ppapi/cpp/var.h"

namespace pp {

class InstanceHandle;

class NetAddress : public Resource {
 public:
  
  
  NetAddress();

  
  
  
  
  NetAddress(PassRef, PP_Resource resource);

  
  
  
  
  
  
  NetAddress(const InstanceHandle& instance,
             const PP_NetAddress_IPv4& ipv4_addr);

  
  
  
  
  
  
  NetAddress(const InstanceHandle& instance,
             const PP_NetAddress_IPv6& ipv6_addr);

  
  
  
  NetAddress(const NetAddress& other);

  
  virtual ~NetAddress();

  
  
  
  
  
  NetAddress& operator=(const NetAddress& other);

  
  
  
  
  static bool IsAvailable();

  
  
  
  
  PP_NetAddress_Family GetFamily() const;

  
  
  
  
  
  
  
  
  
  
  Var DescribeAsString(bool include_port) const;

  
  
  
  
  
  
  
  
  
  
  bool DescribeAsIPv4Address(PP_NetAddress_IPv4* ipv4_addr) const;

  
  
  
  
  
  
  
  
  
  
  bool DescribeAsIPv6Address(PP_NetAddress_IPv6* ipv6_addr) const;
};

}  

#endif  
