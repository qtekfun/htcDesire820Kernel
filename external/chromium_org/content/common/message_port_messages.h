// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "content/common/content_export.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_message_utils.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT
#define IPC_MESSAGE_START MessagePortMsgStart

#ifndef CONTENT_COMMON_MESSAGE_PORT_MESSAGES_H_
#define CONTENT_COMMON_MESSAGE_PORT_MESSAGES_H_

typedef std::pair<base::string16, std::vector<int> > QueuedMessage;

#endif  


IPC_MESSAGE_ROUTED3(MessagePortMsg_Message,
                    base::string16 ,
                    std::vector<int> ,
                    std::vector<int> )

IPC_MESSAGE_ROUTED0(MessagePortMsg_MessagesQueued)


IPC_SYNC_MESSAGE_CONTROL0_2(MessagePortHostMsg_CreateMessagePort,
                            int ,
                            int )

IPC_MESSAGE_CONTROL1(MessagePortHostMsg_DestroyMessagePort,
                     int )

IPC_MESSAGE_CONTROL3(MessagePortHostMsg_PostMessage,
                     int ,
                     base::string16 ,
                     std::vector<int> )

IPC_MESSAGE_CONTROL2(MessagePortHostMsg_Entangle,
                     int ,
                     int )

IPC_MESSAGE_CONTROL1(MessagePortHostMsg_QueueMessages,
                     int )

IPC_MESSAGE_CONTROL2(MessagePortHostMsg_SendQueuedMessages,
                     int ,
                     std::vector<QueuedMessage> )
