/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPB_NET_ADDRESS_PRIVATE_H_
#define PPAPI_C_PRIVATE_PPB_NET_ADDRESS_PRIVATE_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_NETADDRESS_PRIVATE_INTERFACE_0_1 "PPB_NetAddress_Private;0.1"
#define PPB_NETADDRESS_PRIVATE_INTERFACE_1_0 "PPB_NetAddress_Private;1.0"
#define PPB_NETADDRESS_PRIVATE_INTERFACE_1_1 "PPB_NetAddress_Private;1.1"
#define PPB_NETADDRESS_PRIVATE_INTERFACE PPB_NETADDRESS_PRIVATE_INTERFACE_1_1



typedef enum {
  PP_NETADDRESSFAMILY_PRIVATE_UNSPECIFIED = 0,
  PP_NETADDRESSFAMILY_PRIVATE_IPV4 = 1,
  PP_NETADDRESSFAMILY_PRIVATE_IPV6 = 2
} PP_NetAddressFamily_Private;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_NetAddressFamily_Private, 4);

struct PP_NetAddress_Private {
  uint32_t size;
  char data[128];
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_NetAddress_Private, 132);

struct PPB_NetAddress_Private_1_1 {
  PP_Bool (*AreEqual)(const struct PP_NetAddress_Private* addr1,
                      const struct PP_NetAddress_Private* addr2);
  PP_Bool (*AreHostsEqual)(const struct PP_NetAddress_Private* addr1,
                           const struct PP_NetAddress_Private* addr2);
  struct PP_Var (*Describe)(PP_Module module,
                            const struct PP_NetAddress_Private* addr,
                            PP_Bool include_port);
  PP_Bool (*ReplacePort)(const struct PP_NetAddress_Private* src_addr,
                         uint16_t port,
                         struct PP_NetAddress_Private* addr_out);
  void (*GetAnyAddress)(PP_Bool is_ipv6, struct PP_NetAddress_Private* addr);
  PP_NetAddressFamily_Private (*GetFamily)(
      const struct PP_NetAddress_Private* addr);
  uint16_t (*GetPort)(const struct PP_NetAddress_Private* addr);
  PP_Bool (*GetAddress)(const struct PP_NetAddress_Private* addr,
                        void* address,
                        uint16_t address_size);
  uint32_t (*GetScopeID)(const struct PP_NetAddress_Private* addr);
  void (*CreateFromIPv4Address)(const uint8_t ip[4],
                                uint16_t port,
                                struct PP_NetAddress_Private* addr_out);
  void (*CreateFromIPv6Address)(const uint8_t ip[16],
                                uint32_t scope_id,
                                uint16_t port,
                                struct PP_NetAddress_Private* addr_out);
};

typedef struct PPB_NetAddress_Private_1_1 PPB_NetAddress_Private;

struct PPB_NetAddress_Private_0_1 {
  PP_Bool (*AreEqual)(const struct PP_NetAddress_Private* addr1,
                      const struct PP_NetAddress_Private* addr2);
  PP_Bool (*AreHostsEqual)(const struct PP_NetAddress_Private* addr1,
                           const struct PP_NetAddress_Private* addr2);
  struct PP_Var (*Describe)(PP_Module module,
                            const struct PP_NetAddress_Private* addr,
                            PP_Bool include_port);
  PP_Bool (*ReplacePort)(const struct PP_NetAddress_Private* src_addr,
                         uint16_t port,
                         struct PP_NetAddress_Private* addr_out);
  void (*GetAnyAddress)(PP_Bool is_ipv6, struct PP_NetAddress_Private* addr);
};

struct PPB_NetAddress_Private_1_0 {
  PP_Bool (*AreEqual)(const struct PP_NetAddress_Private* addr1,
                      const struct PP_NetAddress_Private* addr2);
  PP_Bool (*AreHostsEqual)(const struct PP_NetAddress_Private* addr1,
                           const struct PP_NetAddress_Private* addr2);
  struct PP_Var (*Describe)(PP_Module module,
                            const struct PP_NetAddress_Private* addr,
                            PP_Bool include_port);
  PP_Bool (*ReplacePort)(const struct PP_NetAddress_Private* src_addr,
                         uint16_t port,
                         struct PP_NetAddress_Private* addr_out);
  void (*GetAnyAddress)(PP_Bool is_ipv6, struct PP_NetAddress_Private* addr);
  PP_NetAddressFamily_Private (*GetFamily)(
      const struct PP_NetAddress_Private* addr);
  uint16_t (*GetPort)(const struct PP_NetAddress_Private* addr);
  PP_Bool (*GetAddress)(const struct PP_NetAddress_Private* addr,
                        void* address,
                        uint16_t address_size);
};

#endif  

