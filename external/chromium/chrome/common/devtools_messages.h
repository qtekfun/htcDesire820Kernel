// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include <map>
#include <string>

#include "ipc/ipc_message_macros.h"

#ifndef CHROME_COMMON_DEVTOOLS_MESSAGES_H_
#define CHROME_COMMON_DEVTOOLS_MESSAGES_H_

typedef std::map<std::string, std::string> DevToolsRuntimeProperties;

#endif  

#define IPC_MESSAGE_START DevToolsMsgStart

IPC_MESSAGE_CONTROL1(DevToolsClientMsg_DispatchOnInspectorFrontend,
                     std::string )

IPC_MESSAGE_CONTROL1(DevToolsClientMsg_DebuggerOutput,
                     std::string )


IPC_MESSAGE_CONTROL1(DevToolsAgentMsg_Attach,
                     DevToolsRuntimeProperties )

IPC_MESSAGE_CONTROL0(DevToolsAgentMsg_Detach)

IPC_MESSAGE_CONTROL0(DevToolsAgentMsg_FrontendLoaded)

IPC_MESSAGE_CONTROL1(DevToolsAgentMsg_DispatchOnInspectorBackend,
                     std::string )

IPC_MESSAGE_CONTROL1(DevToolsAgentMsg_DebuggerCommand,
                     std::string  )

IPC_MESSAGE_CONTROL2(DevToolsAgentMsg_InspectElement,
                     int ,
                     int )



IPC_MESSAGE_ROUTED0(DevToolsMsg_SetupDevToolsClient)



IPC_MESSAGE_ROUTED1(DevToolsHostMsg_ForwardToClient,
                    IPC::Message )

IPC_MESSAGE_ROUTED1(DevToolsHostMsg_ForwardToAgent,
                    IPC::Message )

IPC_MESSAGE_ROUTED0(DevToolsHostMsg_ActivateWindow)

IPC_MESSAGE_ROUTED0(DevToolsHostMsg_CloseWindow)

IPC_MESSAGE_ROUTED0(DevToolsHostMsg_RequestDockWindow)

IPC_MESSAGE_ROUTED0(DevToolsHostMsg_RequestUndockWindow)

IPC_MESSAGE_ROUTED2(DevToolsHostMsg_RuntimePropertyChanged,
                    std::string ,
                    std::string )
