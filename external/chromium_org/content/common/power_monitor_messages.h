// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_param_traits.h"
#include "ipc/ipc_platform_file.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT
#define IPC_MESSAGE_START PowerMonitorMsgStart


IPC_MESSAGE_CONTROL1(PowerMonitorMsg_PowerStateChange,
    bool )

IPC_MESSAGE_CONTROL0(PowerMonitorMsg_Suspend)

IPC_MESSAGE_CONTROL0(PowerMonitorMsg_Resume)
