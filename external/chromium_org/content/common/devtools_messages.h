// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include <map>
#include <string>

#include "content/common/content_export.h"
#include "content/public/common/common_param_traits.h"
#include "content/public/common/console_message_level.h"
#include "ipc/ipc_message_macros.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT

#define IPC_MESSAGE_START DevToolsMsgStart

IPC_MESSAGE_ROUTED1(DevToolsClientMsg_DispatchOnInspectorFrontend,
                    std::string )

IPC_MESSAGE_ROUTED0(DevToolsAgentMsg_Attach)

IPC_MESSAGE_ROUTED1(DevToolsAgentMsg_Reattach,
                    std::string )

IPC_MESSAGE_ROUTED0(DevToolsAgentMsg_Detach)

IPC_MESSAGE_ROUTED1(DevToolsAgentMsg_DispatchOnInspectorBackend,
                    std::string )

IPC_MESSAGE_ROUTED2(DevToolsAgentMsg_InspectElement,
                    int ,
                    int )

IPC_MESSAGE_ROUTED2(DevToolsAgentMsg_AddMessageToConsole,
                    content::ConsoleMessageLevel ,
                    std::string )

IPC_MESSAGE_ROUTED0(DevToolsAgentMsg_PauseWorkerContextOnStart)

IPC_MESSAGE_ROUTED0(DevToolsAgentMsg_ResumeWorkerContext)


IPC_MESSAGE_ROUTED0(DevToolsMsg_SetupDevToolsClient)



IPC_MESSAGE_ROUTED1(DevToolsHostMsg_DispatchOnEmbedder,
                    std::string )

IPC_MESSAGE_ROUTED1(DevToolsHostMsg_SaveAgentRuntimeState,
                    std::string )

IPC_MESSAGE_ROUTED0(DevToolsHostMsg_ClearBrowserCache)

IPC_MESSAGE_ROUTED0(DevToolsHostMsg_ClearBrowserCookies)


IPC_STRUCT_BEGIN(GpuTaskInfo)
  IPC_STRUCT_MEMBER(double, timestamp)
  IPC_STRUCT_MEMBER(int, phase)
  IPC_STRUCT_MEMBER(bool, foreign)
  IPC_STRUCT_MEMBER(uint64, used_gpu_memory_bytes)
IPC_STRUCT_END()

IPC_MESSAGE_ROUTED1(DevToolsAgentMsg_GpuTasksChunk,
                    std::vector<GpuTaskInfo> )

