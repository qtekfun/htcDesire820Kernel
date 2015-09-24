// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "base/process.h"
#include "chrome/common/nacl_types.h"
#include "ipc/ipc_message_macros.h"

#ifndef CHROME_COMMON_NACL_MESSAGES_H_
#define CHROME_COMMON_NACL_MESSAGES_H_

#endif  

#define IPC_MESSAGE_START NaClMsgStart

IPC_MESSAGE_CONTROL1(NaClProcessMsg_Start,
                     std::vector<nacl::FileDescriptor> )

IPC_MESSAGE_CONTROL1(NaClProcessMsg_LaunchLoaderThroughBroker,
                     std::wstring )

IPC_MESSAGE_CONTROL2(NaClProcessMsg_LoaderLaunched,
                     std::wstring,  
                     base::ProcessHandle )

IPC_MESSAGE_CONTROL0(NaClProcessMsg_StopBroker)

