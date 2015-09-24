// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <vector>

#include "content/common/content_export.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_param_traits.h"
#include "url/gurl.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT
#define IPC_MESSAGE_START SocketStreamMsgStart


IPC_MESSAGE_CONTROL3(SocketStreamHostMsg_Connect,
                     int ,
                     GURL ,
                     int )

IPC_MESSAGE_CONTROL2(SocketStreamHostMsg_SendData,
                     int ,
                     std::vector<char> )

IPC_MESSAGE_CONTROL1(SocketStreamHostMsg_Close,
                     int )



IPC_MESSAGE_CONTROL2(SocketStreamMsg_Connected,
                     int ,
                     int )

IPC_MESSAGE_CONTROL2(SocketStreamMsg_ReceivedData,
                     int ,
                     std::vector<char> )

IPC_MESSAGE_CONTROL2(SocketStreamMsg_SentData,
                     int ,
                     int )

IPC_MESSAGE_CONTROL1(SocketStreamMsg_Closed,
                     int )

IPC_MESSAGE_CONTROL2(SocketStreamMsg_Failed,
                     int ,
                     int )
