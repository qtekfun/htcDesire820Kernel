// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "content/common/content_export.h"
#include "content/public/common/common_param_traits.h"
#include "ipc/ipc_message_macros.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT

#define IPC_MESSAGE_START FrameMsgStart

IPC_SYNC_MESSAGE_CONTROL4_1(FrameHostMsg_CreateChildFrame,
                            int32 ,
                            int64 ,
                            int64 ,
                            std::string ,
                            int )

IPC_MESSAGE_ROUTED2(FrameHostMsg_Detach,
                    int64 ,
                    int64 )

IPC_MESSAGE_ROUTED4(FrameHostMsg_DidStartProvisionalLoadForFrame,
                    int64 ,
                    int64 ,
                    bool ,
                    GURL )

IPC_MESSAGE_ROUTED3(FrameHostMsg_PepperPluginHung,
                    int ,
                    base::FilePath ,
                    bool )

IPC_MESSAGE_ROUTED2(FrameHostMsg_PluginCrashed,
                    base::FilePath ,
                    base::ProcessId )

IPC_SYNC_MESSAGE_CONTROL4_3(FrameHostMsg_GetPluginInfo,
                            int ,
                            GURL ,
                            GURL ,
                            std::string ,
                            bool ,
                            content::WebPluginInfo ,
                            std::string )

IPC_SYNC_MESSAGE_CONTROL4_2(FrameHostMsg_OpenChannelToPlugin,
                            int ,
                            GURL ,
                            GURL ,
                            std::string ,
                            IPC::ChannelHandle ,
                            content::WebPluginInfo )
