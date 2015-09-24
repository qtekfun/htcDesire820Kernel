// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include <string>

#include "base/memory/shared_memory.h"
#include "ipc/ipc_message_macros.h"

#define IPC_MESSAGE_START WebRtcLoggingMsgStart


IPC_MESSAGE_CONTROL1(WebRtcLoggingMsg_AddLogMessage,
                     std::string )

IPC_MESSAGE_CONTROL0(WebRtcLoggingMsg_LoggingStopped)


IPC_MESSAGE_CONTROL0(WebRtcLoggingMsg_StartLogging)

IPC_MESSAGE_CONTROL0(WebRtcLoggingMsg_StopLogging)
