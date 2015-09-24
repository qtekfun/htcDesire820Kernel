// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/sync_socket.h"
#include "ipc/ipc_channel_handle.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_message_utils.h"
#include "ipc/ipc_platform_file.h"

#define IPC_MESSAGE_START TracingMsgStart

IPC_MESSAGE_CONTROL3(TracingMsg_BeginTracing,
                     std::string ,
                     base::TimeTicks ,
                     int )

IPC_MESSAGE_CONTROL0(TracingMsg_EndTracing)

IPC_MESSAGE_CONTROL3(TracingMsg_EnableMonitoring,
                     std::string ,
                     base::TimeTicks ,
                     int )

IPC_MESSAGE_CONTROL0(TracingMsg_DisableMonitoring)

IPC_MESSAGE_CONTROL0(TracingMsg_CaptureMonitoringSnapshot)

IPC_MESSAGE_CONTROL0(TracingMsg_GetTraceBufferPercentFull)

IPC_MESSAGE_CONTROL2(TracingMsg_SetWatchEvent,
                     std::string ,
                     std::string )

IPC_MESSAGE_CONTROL0(TracingMsg_CancelWatchEvent)

IPC_MESSAGE_CONTROL0(TracingHostMsg_WatchEventMatched);

IPC_MESSAGE_CONTROL0(TracingHostMsg_ChildSupportsTracing)

IPC_MESSAGE_CONTROL1(TracingHostMsg_EndTracingAck,
                     std::vector<std::string> )

IPC_MESSAGE_CONTROL0(TracingHostMsg_CaptureMonitoringSnapshotAck)

IPC_MESSAGE_CONTROL1(TracingHostMsg_TraceDataCollected,
                     std::string )

IPC_MESSAGE_CONTROL1(TracingHostMsg_MonitoringTraceDataCollected,
                     std::string )

IPC_MESSAGE_CONTROL1(TracingHostMsg_TraceBufferPercentFullReply,
                     float )

