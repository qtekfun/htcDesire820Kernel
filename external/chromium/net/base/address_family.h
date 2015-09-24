// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_ADDRESS_FAMILY_H_
#define NET_BASE_ADDRESS_FAMILY_H_
#pragma once

namespace net {

enum AddressFamily {
  ADDRESS_FAMILY_UNSPECIFIED,   
  ADDRESS_FAMILY_IPV4,          
  ADDRESS_FAMILY_IPV6,          
};

enum {
  HOST_RESOLVER_CANONNAME = 1 << 0,  
  
  HOST_RESOLVER_LOOPBACK_ONLY = 1 << 1,
  
  HOST_RESOLVER_DEFAULT_FAMILY_SET_DUE_TO_NO_IPV6 = 1 << 2,
};
typedef int HostResolverFlags;

}  

#endif  
