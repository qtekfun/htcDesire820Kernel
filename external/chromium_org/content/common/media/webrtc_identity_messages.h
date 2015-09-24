// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "content/common/content_export.h"
#include "ipc/ipc_message_macros.h"
#include "url/gurl.h"

#define IPC_MESSAGE_START WebRTCIdentityMsgStart
#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT

IPC_MESSAGE_CONTROL4(WebRTCIdentityMsg_RequestIdentity,
                     int ,
                     GURL ,
                     std::string ,
                     std::string )
IPC_MESSAGE_CONTROL0(WebRTCIdentityMsg_CancelRequest)

IPC_MESSAGE_CONTROL3(WebRTCIdentityHostMsg_IdentityReady,
                     int ,
                     std::string ,
                     std::string )
IPC_MESSAGE_CONTROL2(WebRTCIdentityHostMsg_RequestFailed,
                     int ,
                     int )
