// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <string>

#include "chrome/common/remoting/chromoting_host_info.h"
#include "ipc/ipc_channel_handle.h"
#include "ipc/ipc_message_macros.h"

#define IPC_MESSAGE_START ServiceMsgStart

IPC_STRUCT_TRAITS_BEGIN(remoting::ChromotingHostInfo)
  IPC_STRUCT_TRAITS_MEMBER(host_id)
  IPC_STRUCT_TRAITS_MEMBER(hostname)
  IPC_STRUCT_TRAITS_MEMBER(public_key)
  IPC_STRUCT_TRAITS_MEMBER(enabled)
  IPC_STRUCT_TRAITS_MEMBER(login)
IPC_STRUCT_TRAITS_END()

IPC_MESSAGE_CONTROL1(ServiceMsg_EnableCloudPrintProxy,
                     std::string )
IPC_MESSAGE_CONTROL2(ServiceMsg_EnableCloudPrintProxyWithTokens,
                     std::string, 
                     std::string  )
IPC_MESSAGE_CONTROL0(ServiceMsg_DisableCloudPrintProxy)

IPC_MESSAGE_CONTROL0(ServiceMsg_IsCloudPrintProxyEnabled)

IPC_MESSAGE_CONTROL2(ServiceMsg_SetRemotingHostCredentials,
                     std::string, 
                     std::string  )

IPC_MESSAGE_CONTROL0(ServiceMsg_EnableRemotingHost)

IPC_MESSAGE_CONTROL0(ServiceMsg_DisableRemotingHost)

IPC_MESSAGE_CONTROL0(ServiceMsg_GetRemotingHostInfo)

IPC_MESSAGE_CONTROL0(ServiceMsg_Shutdown)

IPC_MESSAGE_CONTROL0(ServiceMsg_UpdateAvailable)

IPC_MESSAGE_CONTROL0(ServiceHostMsg_CloudPrintProxy_AuthError)

IPC_MESSAGE_CONTROL2(ServiceHostMsg_CloudPrintProxy_IsEnabled,
                     bool,       
                     std::string )

IPC_MESSAGE_CONTROL1(ServiceHostMsg_RemotingHost_HostInfo,
                     remoting::ChromotingHostInfo )

