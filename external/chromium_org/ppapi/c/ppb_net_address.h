/* Copyright 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_NET_ADDRESS_H_
#define PPAPI_C_PPB_NET_ADDRESS_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_NETADDRESS_INTERFACE_1_0 "PPB_NetAddress;1.0"
#define PPB_NETADDRESS_INTERFACE PPB_NETADDRESS_INTERFACE_1_0



typedef enum {
  PP_NETADDRESS_FAMILY_UNSPECIFIED = 0,
  PP_NETADDRESS_FAMILY_IPV4 = 1,
  PP_NETADDRESS_FAMILY_IPV6 = 2
} PP_NetAddress_Family;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_NetAddress_Family, 4);

struct PP_NetAddress_IPv4 {
  uint16_t port;
  uint8_t addr[4];
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_NetAddress_IPv4, 6);

struct PP_NetAddress_IPv6 {
  uint16_t port;
  uint8_t addr[16];
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_NetAddress_IPv6, 18);

struct PPB_NetAddress_1_0 {
  PP_Resource (*CreateFromIPv4Address)(
      PP_Instance instance,
      const struct PP_NetAddress_IPv4* ipv4_addr);
  PP_Resource (*CreateFromIPv6Address)(
      PP_Instance instance,
      const struct PP_NetAddress_IPv6* ipv6_addr);
  PP_Bool (*IsNetAddress)(PP_Resource resource);
  PP_NetAddress_Family (*GetFamily)(PP_Resource addr);
  struct PP_Var (*DescribeAsString)(PP_Resource addr, PP_Bool include_port);
  PP_Bool (*DescribeAsIPv4Address)(PP_Resource addr,
                                   struct PP_NetAddress_IPv4* ipv4_addr);
  PP_Bool (*DescribeAsIPv6Address)(PP_Resource addr,
                                   struct PP_NetAddress_IPv6* ipv6_addr);
};

typedef struct PPB_NetAddress_1_0 PPB_NetAddress;

#endif  

