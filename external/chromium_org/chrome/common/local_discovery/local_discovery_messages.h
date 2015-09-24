// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include <vector>

#include "base/file_descriptor_posix.h"
#include "chrome/common/local_discovery/service_discovery_client.h"
#include "ipc/ipc_message_macros.h"

#ifndef CHROME_COMMON_LOCAL_DISCOVERY_LOCAL_DISCOVERY_MESSAGES_H_
#define CHROME_COMMON_LOCAL_DISCOVERY_LOCAL_DISCOVERY_MESSAGES_H_

#if defined(OS_POSIX)
struct LocalDiscoveryMsg_SocketInfo {
  LocalDiscoveryMsg_SocketInfo()
      : address_family(net::ADDRESS_FAMILY_UNSPECIFIED),
        interface_index(0) {
  }

  base::FileDescriptor descriptor;
  net::AddressFamily address_family;
  uint32 interface_index;
};
#endif  

#endif  

#define IPC_MESSAGE_START LocalDiscoveryMsgStart

IPC_STRUCT_TRAITS_BEGIN(local_discovery::ServiceDescription)
  IPC_STRUCT_TRAITS_MEMBER(service_name)
  IPC_STRUCT_TRAITS_MEMBER(address)
  IPC_STRUCT_TRAITS_MEMBER(metadata)
  IPC_STRUCT_TRAITS_MEMBER(ip_address)
  IPC_STRUCT_TRAITS_MEMBER(last_seen)
IPC_STRUCT_TRAITS_END()

IPC_ENUM_TRAITS(local_discovery::ServiceWatcher::UpdateType)
IPC_ENUM_TRAITS(local_discovery::ServiceResolver::RequestStatus)
IPC_ENUM_TRAITS(net::AddressFamily)

#if defined(OS_POSIX)
IPC_STRUCT_TRAITS_BEGIN(LocalDiscoveryMsg_SocketInfo)
  IPC_STRUCT_TRAITS_MEMBER(descriptor)
  IPC_STRUCT_TRAITS_MEMBER(address_family)
  IPC_STRUCT_TRAITS_MEMBER(interface_index)
IPC_STRUCT_TRAITS_END()
#endif  

#if defined(OS_POSIX)
IPC_MESSAGE_CONTROL1(LocalDiscoveryMsg_SetSockets,
                     std::vector<LocalDiscoveryMsg_SocketInfo> )
#endif  

IPC_MESSAGE_CONTROL2(LocalDiscoveryMsg_StartWatcher,
                     uint64 ,
                     std::string )

IPC_MESSAGE_CONTROL2(LocalDiscoveryMsg_DiscoverServices,
                     uint64 ,
                     bool )

IPC_MESSAGE_CONTROL1(LocalDiscoveryMsg_DestroyWatcher,
                     uint64 )

IPC_MESSAGE_CONTROL2(LocalDiscoveryMsg_ResolveService,
                     uint64 ,
                     std::string )

IPC_MESSAGE_CONTROL1(LocalDiscoveryMsg_DestroyResolver,
                     uint64 )

IPC_MESSAGE_CONTROL3(LocalDiscoveryMsg_ResolveLocalDomain,
                     uint64 ,
                     std::string ,
                     net::AddressFamily )

IPC_MESSAGE_CONTROL1(LocalDiscoveryMsg_DestroyLocalDomainResolver,
                     uint64 )

IPC_MESSAGE_CONTROL0(LocalDiscoveryMsg_ShutdownLocalDiscovery)



IPC_MESSAGE_CONTROL0(LocalDiscoveryHostMsg_Error)

IPC_MESSAGE_CONTROL3(LocalDiscoveryHostMsg_WatcherCallback,
                     uint64 ,
                     local_discovery::ServiceWatcher::UpdateType ,
                     std::string )

IPC_MESSAGE_CONTROL3(
    LocalDiscoveryHostMsg_ResolverCallback,
    uint64 ,
    local_discovery::ServiceResolver::RequestStatus ,
    local_discovery::ServiceDescription )

IPC_MESSAGE_CONTROL4(
    LocalDiscoveryHostMsg_LocalDomainResolverCallback,
    uint64 ,
    bool ,
    net::IPAddressNumber ,
    net::IPAddressNumber )
