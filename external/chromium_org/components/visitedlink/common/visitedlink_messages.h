// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <vector>

#include "base/basictypes.h"
#include "base/memory/shared_memory.h"
#include "content/public/common/common_param_traits_macros.h"
#include "ipc/ipc_message_macros.h"

#define IPC_MESSAGE_START VisitedLinkMsgStart

IPC_MESSAGE_CONTROL1(ChromeViewMsg_VisitedLink_NewTable,
                     base::SharedMemoryHandle)

IPC_MESSAGE_CONTROL1(ChromeViewMsg_VisitedLink_Add, std::vector<uint64>)

IPC_MESSAGE_CONTROL0(ChromeViewMsg_VisitedLink_Reset)

