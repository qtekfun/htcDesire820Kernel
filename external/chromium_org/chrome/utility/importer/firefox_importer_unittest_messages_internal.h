// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ipc/ipc_message_macros.h"

#define IPC_MESSAGE_START FirefoxImporterUnittestMsgStart


IPC_MESSAGE_CONTROL2(Msg_Decryptor_Init,
                     base::FilePath ,
                     base::FilePath )
IPC_MESSAGE_CONTROL1(Msg_Decryptor_InitReturnCode,
                     bool )

IPC_MESSAGE_CONTROL1(Msg_Decrypt,
                     std::string )
IPC_MESSAGE_CONTROL1(Msg_Decryptor_Response,
                     string16 )

IPC_MESSAGE_CONTROL0(Msg_Decryptor_Quit)
