// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "base/files/file_path.h"
#include "content/public/common/common_param_traits.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_param_traits.h"

#define IPC_MESSAGE_START MimeRegistryMsgStart


IPC_SYNC_MESSAGE_CONTROL1_1(MimeRegistryMsg_GetMimeTypeFromExtension,
                            base::FilePath::StringType ,
                            std::string )
IPC_SYNC_MESSAGE_CONTROL1_1(MimeRegistryMsg_GetMimeTypeFromFile,
                            base::FilePath ,
                            std::string )

