// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include <string>

#include "base/basictypes.h"
#include "build/build_config.h"

#include "ipc/ipc_message_macros.h"
#include "ui/gfx/native_widget_types.h"

#define IPC_MESSAGE_START ChromeBenchmarkingMsgStart

IPC_SYNC_MESSAGE_CONTROL0_0(ChromeViewHostMsg_CloseCurrentConnections)

IPC_MESSAGE_CONTROL1(ChromeViewHostMsg_SetCacheMode,
                     bool )

IPC_SYNC_MESSAGE_CONTROL0_1(ChromeViewHostMsg_ClearCache,
                            int )

IPC_SYNC_MESSAGE_CONTROL0_1(ChromeViewHostMsg_ClearHostResolverCache,
                            int )

IPC_MESSAGE_CONTROL1(ChromeViewHostMsg_EnableSpdy,
                     bool )

IPC_SYNC_MESSAGE_CONTROL0_1(ChromeViewHostMsg_ClearPredictorCache,
                            int )
