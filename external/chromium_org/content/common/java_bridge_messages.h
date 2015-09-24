// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#if defined(OS_ANDROID)

#include "content/child/plugin_param_traits.h"
#include "ipc/ipc_channel_handle.h"
#include "ipc/ipc_message_macros.h"

#define IPC_MESSAGE_START JavaBridgeMsgStart


IPC_MESSAGE_ROUTED2(JavaBridgeMsg_AddNamedObject,
                    base::string16 ,
                    content::NPVariant_Param) 

IPC_MESSAGE_ROUTED1(JavaBridgeMsg_RemoveNamedObject,
                    base::string16 )

IPC_SYNC_MESSAGE_CONTROL0_1(JavaBridgeMsg_GenerateRouteID,
                            int )

IPC_SYNC_MESSAGE_ROUTED0_1(JavaBridgeHostMsg_GetChannelHandle,
                           IPC::ChannelHandle) 

#endif  
