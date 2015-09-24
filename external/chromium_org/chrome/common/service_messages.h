// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <string>
#include <vector>

#include "base/values.h"
#include "chrome/common/cloud_print/cloud_print_proxy_info.h"
#include "ipc/ipc_channel_handle.h"
#include "ipc/ipc_message_macros.h"

#define IPC_MESSAGE_START ServiceMsgStart

IPC_STRUCT_TRAITS_BEGIN(cloud_print::CloudPrintProxyInfo)
  IPC_STRUCT_TRAITS_MEMBER(enabled)
  IPC_STRUCT_TRAITS_MEMBER(email)
  IPC_STRUCT_TRAITS_MEMBER(proxy_id)
IPC_STRUCT_TRAITS_END()

IPC_MESSAGE_CONTROL4(ServiceMsg_EnableCloudPrintProxyWithRobot,
                     std::string ,
                     std::string ,
                     std::string ,
                     base::DictionaryValue )

IPC_MESSAGE_CONTROL0(ServiceMsg_DisableCloudPrintProxy)

IPC_MESSAGE_CONTROL0(ServiceMsg_GetCloudPrintProxyInfo)

IPC_MESSAGE_CONTROL0(ServiceMsg_GetHistograms)

IPC_MESSAGE_CONTROL0(ServiceMsg_Shutdown)

IPC_MESSAGE_CONTROL0(ServiceMsg_UpdateAvailable)

IPC_MESSAGE_CONTROL1(ServiceHostMsg_CloudPrintProxy_Info,
                     cloud_print::CloudPrintProxyInfo )

IPC_MESSAGE_CONTROL1(ServiceHostMsg_Histograms,
                     std::vector<std::string> )
