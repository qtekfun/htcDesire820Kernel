// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#include <string>
#include <vector>

#include "base/basictypes.h"
#include "content/common/content_export.h"
#include "content/common/websocket.h"
#include "ipc/ipc_message_macros.h"
#include "url/gurl.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT
#define IPC_MESSAGE_START WebSocketMsgStart

IPC_STRUCT_TRAITS_BEGIN(content::WebSocketHandshakeRequest)
  IPC_STRUCT_TRAITS_MEMBER(url)
  IPC_STRUCT_TRAITS_MEMBER(headers)
  IPC_STRUCT_TRAITS_MEMBER(request_time)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(content::WebSocketHandshakeResponse)
  IPC_STRUCT_TRAITS_MEMBER(url)
  IPC_STRUCT_TRAITS_MEMBER(status_code)
  IPC_STRUCT_TRAITS_MEMBER(status_text)
  IPC_STRUCT_TRAITS_MEMBER(headers)
  IPC_STRUCT_TRAITS_MEMBER(response_time)
IPC_STRUCT_TRAITS_END()


IPC_MESSAGE_ROUTED3(WebSocketHostMsg_AddChannelRequest,
                    GURL ,
                    std::vector<std::string> ,
                    GURL )


IPC_MESSAGE_ROUTED3(WebSocketMsg_AddChannelResponse,
                    bool ,
                    std::string ,
                    std::string )

IPC_MESSAGE_ROUTED1(WebSocketMsg_NotifyStartOpeningHandshake,
                    content::WebSocketHandshakeRequest )

IPC_MESSAGE_ROUTED1(WebSocketMsg_NotifyFinishOpeningHandshake,
                    content::WebSocketHandshakeResponse )

IPC_MESSAGE_ROUTED1(WebSocketMsg_NotifyFailure,
                    std::string )


IPC_ENUM_TRAITS(content::WebSocketMessageType)

IPC_MESSAGE_ROUTED3(WebSocketMsg_SendFrame,
                    bool ,
                    content::WebSocketMessageType ,
                    std::vector<char> )

IPC_MESSAGE_ROUTED1(WebSocketMsg_FlowControl,
                    int64 )

IPC_MESSAGE_ROUTED3(WebSocketMsg_DropChannel,
                    bool ,
                    unsigned short ,
                    std::string )
