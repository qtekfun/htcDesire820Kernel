// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "base/time/time.h"
#include "content/common/content_export.h"
#include "content/public/common/p2p_socket_type.h"
#include "ipc/ipc_message_macros.h"
#include "net/base/ip_endpoint.h"
#include "net/base/net_util.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT
#define IPC_MESSAGE_START P2PMsgStart

IPC_ENUM_TRAITS(content::P2PSocketType)
IPC_ENUM_TRAITS(net::DiffServCodePoint)

IPC_STRUCT_TRAITS_BEGIN(net::NetworkInterface)
  IPC_STRUCT_TRAITS_MEMBER(name)
  IPC_STRUCT_TRAITS_MEMBER(address)
IPC_STRUCT_TRAITS_END()


IPC_MESSAGE_CONTROL1(P2PMsg_NetworkListChanged,
                     net::NetworkInterfaceList )

IPC_MESSAGE_CONTROL2(P2PMsg_GetHostAddressResult,
                     int32 ,
                     net::IPAddressNumber )

IPC_MESSAGE_CONTROL2(P2PMsg_OnSocketCreated,
                     int ,
                     net::IPEndPoint )

IPC_MESSAGE_CONTROL1(P2PMsg_OnSendComplete,
                     int )

IPC_MESSAGE_CONTROL1(P2PMsg_OnError,
                     int )

IPC_MESSAGE_CONTROL2(P2PMsg_OnIncomingTcpConnection,
                     int ,
                     net::IPEndPoint )

IPC_MESSAGE_CONTROL4(P2PMsg_OnDataReceived,
                     int ,
                     net::IPEndPoint ,
                     std::vector<char> ,
                     base::TimeTicks  )


IPC_MESSAGE_CONTROL0(P2PHostMsg_StartNetworkNotifications)
IPC_MESSAGE_CONTROL0(P2PHostMsg_StopNetworkNotifications)

IPC_MESSAGE_CONTROL2(P2PHostMsg_GetHostAddress,
                    std::string ,
                    int32 )

IPC_MESSAGE_CONTROL4(P2PHostMsg_CreateSocket,
                     content::P2PSocketType ,
                     int ,
                     net::IPEndPoint ,
                     net::IPEndPoint )

IPC_MESSAGE_CONTROL3(P2PHostMsg_AcceptIncomingTcpConnection,
                    int ,
                    net::IPEndPoint ,
                    int )

IPC_MESSAGE_CONTROL5(P2PHostMsg_Send,
                     int ,
                     net::IPEndPoint ,
                     std::vector<char> ,
                     net::DiffServCodePoint ,
                     uint64 )

IPC_MESSAGE_CONTROL1(P2PHostMsg_DestroySocket,
                     int )
