// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "base/memory/shared_memory.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_param_traits.h"
#include "ipc/ipc_platform_file.h"

#define IPC_MESSAGE_START GamepadMsgStart


IPC_SYNC_MESSAGE_CONTROL0_1(GamepadHostMsg_StartPolling,
                            base::SharedMemoryHandle )

IPC_SYNC_MESSAGE_CONTROL0_0(GamepadHostMsg_StopPolling)
